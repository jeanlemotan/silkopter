#include "stdafx.h"
#include "Numeric_Viewer.h"
#include "HAL.h"
#include "Comms.h"
#include "ui_Numeric_Viewer.h"

Numeric_Viewer::Numeric_Viewer(std::string const& unit, uint32_t sample_rate, QWidget *parent)
    : QWidget(parent)
{
    m_sample_rate = sample_rate;

    m_ui.setupUi(this);

    QFont font;
    font.setPointSize(8);

    //m_ui.plot->setNotAntialiasedElements(QCP::AntialiasedElements(QCP::aeAll));
    m_ui.plot->setPlottingHints(QCP::PlottingHints(QCP::phFastPolylines));
//    m_ui.plot->xAxis->setLabel("Time (s)");
    m_ui.plot->xAxis->setLabelFont(font);
    m_ui.plot->xAxis->setTickLabelFont(font);
    m_ui.plot->yAxis->setLabel(unit.c_str());
    m_ui.plot->yAxis->setLabelFont(font);
    m_ui.plot->yAxis->setTickLabelFont(font);
    m_ui.plot->yAxis->grid()->setSubGridVisible(true);

    //m_ui.fft->setNotAntialiasedElements(QCP::AntialiasedElements(QCP::aeAll));
    m_ui.fft->setPlottingHints(QCP::PlottingHints(QCP::phFastPolylines));
//    m_ui.fft->xAxis->setLabel("Frequency (Hz)");
    m_ui.fft->xAxis->setLabelFont(font);
    m_ui.fft->xAxis->setTickLabelFont(font);
    m_ui.fft->xAxis->grid()->setSubGridVisible(true);
    m_ui.fft->xAxis->setRange(0.1, m_sample_rate / 2);

    m_ui.fft->yAxis->setLabel(unit.c_str());
    m_ui.fft->yAxis->setLabelFont(font);
    m_ui.fft->yAxis->setTickLabelFont(font);
    m_ui.fft->yAxis->grid()->setSubGridVisible(true);
    //m_ui.fft->yAxis->setRange(0, 5.f);

//    connect(customPlot->xAxis, SIGNAL(rangeChanged(QCPRange)), customPlot->xAxis2, SLOT(setRange(QCPRange)));
//    connect(customPlot->xAxis, SIGNAL(rangeChanged(QCPRange)), customPlot->xAxis2, SLOT(setRange(QCPRange)));

//    m_ui.fft->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);

    connect(m_ui.logarithmic, &QCheckBox::toggled, [this](bool yes)
    {
        if (yes)
        {
            m_ui.fft->xAxis->setScaleType(QCPAxis::stLogarithmic);
            m_ui.fft->xAxis->setScaleLogBase(10);
            m_ui.fft->xAxis->setNumberFormat("fb");
            m_ui.fft->xAxis->setNumberPrecision(0);
            m_ui.fft->xAxis->setSubTickCount(10);
        }
        else
        {
            m_ui.fft->xAxis->setScaleType(QCPAxis::stLinear);
            m_ui.fft->xAxis->setNumberFormat(m_ui.fft->yAxis->numberFormat());
            m_ui.fft->xAxis->setNumberPrecision(m_ui.fft->yAxis->numberPrecision());
            m_ui.fft->xAxis->setSubTickCount(m_ui.fft->yAxis->subTickCount());
        }
    });

    m_ui.progress->setValue(m_ui.progress->maximum());
    connect(m_ui.progress, &QScrollBar::valueChanged, [this](int pos)
    {
        if (pos == m_ui.progress->maximum())
        {
            m_view.is_at_end = true;
            update_view();
        }
        else
        {
            m_view.is_at_end = false;
            float ratio = static_cast<float>(pos - m_ui.progress->minimum()) / static_cast<float>(m_ui.progress->maximum() - m_ui.progress->minimum());
            auto end = m_samples.size();
            m_view.end_idx = math::clamp(static_cast<size_t>(ratio * end), size_t(2), end);
            update_view();
        }
    });

    //m_ui.window->setSingleStep(1.0 / double(m_sample_rate));
}

Numeric_Viewer::~Numeric_Viewer()
{
}

void Numeric_Viewer::add_graph(std::string const& name, std::string const& unit, QColor color)
{
    QASSERT(!m_is_started);
    if (m_is_started)
    {
        return;
    }

    auto* plot = m_ui.plot->addGraph();
    plot->setPen(color);
    plot->setAdaptiveSampling(false);
    plot->setName(name.c_str());

    auto* fft_plot = m_ui.fft->addGraph();
    fft_plot->setPen(color);
    fft_plot->setBrush(color);
    fft_plot->setAdaptiveSampling(false);
    fft_plot->setName(name.c_str());

    m_graphs.emplace_back(new Graph());
    Graph& graph = *m_graphs.back();
    graph.name = name;
    graph.unit = unit;
    graph.fft.temp_input.reset(static_cast<float*>(fftwf_malloc(FFT::MAX_INPUT_SIZE * sizeof(float))), fftwf_free);
    graph.fft.temp_output.reset(static_cast<fftwf_complex*>(fftwf_malloc(FFT::MAX_INPUT_SIZE * sizeof(fftwf_complex))), fftwf_free);
    graph.plot = plot;
    graph.fft_plot = fft_plot;

    auto checkbox = new QCheckBox(name.c_str(), m_ui.filters);
    checkbox->setChecked(true);
    checkbox->setMaximumSize(QSize(16777215, 16));
    QFont font;
    font.setPointSize(8);
    checkbox->setFont(font);
    checkbox->setIconSize(QSize(12, 12));
    m_ui.filters->layout()->addWidget(checkbox);
    connect(checkbox, &QCheckBox::toggled, [&graph](bool yes) { graph.is_visible = yes; });
}

void Numeric_Viewer::add_samples(q::Clock::duration dt, float const* src)
{
    Sample sample;
    sample.time = std::chrono::duration<double>(m_time).count();
    size_t off = m_values.size();
    m_values.resize(m_values.size() + m_graphs.size());
    std::copy(src, src + m_graphs.size(), m_values.begin() + off);
    m_temp_samples.emplace_back(std::move(sample));
    m_time += dt;
}

void Numeric_Viewer::process()
{
    //launch any pending task
    if (!m_is_started)
    {
        start_tasks();
        m_is_started = true;
        return;
    }

    //check if all ready
    bool all_ready = true;
    for (auto& g: m_graphs)
    {
        if (!g->future.is_ready())
        {
            all_ready = false;
            break;
        }
    }
    if (all_ready)
    {
        finish_tasks();
        start_tasks();
    }
}

void Numeric_Viewer::copy_samples()
{
    size_t off = m_samples.size();
    m_samples.resize(m_samples.size() + m_temp_samples.size());
    std::copy(m_temp_samples.begin(), m_temp_samples.end(), m_samples.begin() + off);
    m_temp_samples.clear();
}

void Numeric_Viewer::update_view()
{
    //update range
    auto time = m_ui.window->value();
    int sample_count = static_cast<int>(math::round(time * double(m_sample_rate)));
    if (m_view.is_at_end)
    {
        m_view.end_idx = m_samples.size();
    }

    int start_idx = math::max(static_cast<int>(m_view.end_idx) - sample_count, 0);
    m_view.start_idx = static_cast<size_t>(start_idx);

    if (!m_samples.empty())
    {
        m_view.time = m_samples[m_view.end_idx - 1].time - m_samples[m_view.start_idx].time;
    }
    else
    {
        m_view.time = 0.0;
    }
}

void Numeric_Viewer::process_plot_task(size_t graph_idx, View const& view)
{
    Graph& graph = *m_graphs[graph_idx];
    graph.stats.average_value = 0;
    graph.stats.min_value = std::numeric_limits<float>::max();
    graph.stats.max_value = std::numeric_limits<float>::lowest();

    graph.plot_data_map.resize(0);
    graph.plot_data_map.reserve(100000);

    double pixels_per_second = static_cast<float>(m_ui.plot->width()) / view.time;
    size_t offset = view.start_idx * m_graphs.size();
    if (pixels_per_second < m_sample_rate)
    {
        double last_key = 0;
        for (size_t i = view.start_idx; i < view.end_idx; i++)
        {
            auto const& s = m_samples[i];
            auto v = m_values[offset + graph_idx];
            offset += m_graphs.size();
            graph.stats.max_value = math::max(graph.stats.max_value, v);
            graph.stats.min_value = math::min(graph.stats.min_value, v);

            auto key = s.time;
            auto dt = key - last_key;
            if (dt * pixels_per_second >= 1.0)
            {
                last_key = key;
                graph.plot_data_map.push_back(QCPData(key, v));
            }
        }
    }
    else
    {
        for (size_t i = view.start_idx; i < view.end_idx; i++)
        {
            auto const& s = m_samples[i];
            auto v = m_values[offset + graph_idx];
            offset += m_graphs.size();
            graph.stats.min_value = math::min(graph.stats.min_value, v);
            graph.stats.max_value = math::max(graph.stats.max_value, v);
            graph.plot_data_map.push_back(QCPData(s.time, v));
        }
    }
    graph.stats.average_value = (graph.stats.max_value + graph.stats.min_value) * 0.5f;
}

void Numeric_Viewer::process_fft_task(size_t graph_idx, View const& view)
{
    Graph& graph = *m_graphs[graph_idx];

    auto sample_count = math::min(view.end_idx - view.start_idx, FFT::MAX_INPUT_SIZE);

    float div = 1.f / float(sample_count);
    size_t output_size = sample_count / 2 + 1;

    double freq_div = (m_sample_rate / 2) / double(output_size);

    auto* temp_input = graph.fft.temp_input.get();
    size_t offset = view.start_idx * m_graphs.size();
    for (size_t i = 0; i < sample_count; i++)
    {
        temp_input[i] = m_values[offset + graph_idx] - graph.stats.average_value;
        offset += m_graphs.size();
    }
    fftwf_execute(graph.fft.plan);
    auto* temp_output = graph.fft.temp_output.get();
    graph.fft_data_map.resize(0);
    graph.fft_data_map.reserve(100000);

    double pixels_per_second = static_cast<float>(m_ui.plot->width()) / view.time;
    if (pixels_per_second < m_sample_rate)
    {
        double last_key = 0;
        float max_value = std::numeric_limits<float>::lowest();
        for (size_t i = 1; i < output_size; i++)
        {
            float x = temp_output[i][0];
            float y = temp_output[i][1];
            float v = math::sqrt(x*x + y*y) * div;
            max_value = math::max(max_value, v);

            double key = i * freq_div;
            auto dt = key - last_key;
            if (dt * pixels_per_second >= 1.0)
            {
                last_key = key;
                graph.fft_data_map.push_back(QCPData(key, max_value));
                max_value = std::numeric_limits<float>::lowest();
            }
        }
    }
    else
    {
        for (size_t i = 1; i < output_size; i++)
        {
            float x = temp_output[i][0];
            float y = temp_output[i][1];
            float v = math::sqrt(x*x + y*y) * div;
            graph.fft_data_map.push_back(QCPData(i * freq_div, v));
        }
    }
}

void Numeric_Viewer::start_tasks()
{
    copy_samples();
    update_view();

    if (m_sample_rate == 0 || m_samples.empty())
    {
        return;
    }
    auto sample_count = math::min(m_view.end_idx - m_view.start_idx, FFT::MAX_INPUT_SIZE);
    if (sample_count < 2)
    {
        return;
    }

    auto view = m_view; //make a copy as it change change while the task is progressing
    for (size_t i = 0; i < m_graphs.size(); i++)
    {
        auto& graph = *m_graphs[i];
        if (graph.is_visible)
        {
            if (sample_count != graph.fft.plan_sample_count)
            {
                graph.fft.plan = fftwf_plan_dft_r2c_1d(sample_count, graph.fft.temp_input.get(), graph.fft.temp_output.get(), FFTW_ESTIMATE);
                graph.fft.plan_sample_count = sample_count;
            }

            graph.future = silk::async([this, view, i]()
            {
                process_plot_task(i, view);
                process_fft_task(i, view);
            });
        }
        else
        {
            graph.plot->clearData();
            graph.fft_plot->clearData();
        }
    }
}

void Numeric_Viewer::finish_tasks()
{
    float min_value = std::numeric_limits<float>::max();
    float max_value = std::numeric_limits<float>::lowest();

    for (size_t i = 0; i < m_graphs.size(); i++)
    {
        auto& graph = *m_graphs[i];
        graph.plot->data()->swap(graph.plot_data_map);
        graph.fft_plot->data()->swap(graph.fft_data_map);

        min_value = math::min(min_value, graph.stats.min_value);
        max_value = math::max(max_value, graph.stats.max_value);
    }

    float average_value = (max_value + min_value) * 0.5f;
    m_ui.data_range->setValue(max_value - min_value);
    m_ui.average->setValue(average_value);
    m_ui.plot->xAxis->setRange(m_samples[m_view.start_idx].time, m_samples[m_view.start_idx].time + m_view.time);

    auto display_range = m_ui.display_range->value();
    if (display_range == 0)
    {
        m_ui.plot->yAxis->setRange(min_value, max_value);
        m_ui.fft->yAxis->setRange(0, math::sqrt(max_value - min_value));
    }
    else
    {
        m_ui.plot->yAxis->setRange(average_value - display_range / 2, average_value + display_range / 2);
        m_ui.fft->yAxis->setRange(0, math::sqrt(display_range));
    }

    m_ui.plot->replot();
    m_ui.fft->replot();
}
