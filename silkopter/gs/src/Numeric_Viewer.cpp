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

//    m_ui.plot->setNotAntialiasedElements(QCP::AntialiasedElements(QCP::aeAll));
    m_ui.plot->setPlottingHints(QCP::PlottingHints(QCP::phFastPolylines));
//    m_ui.plot->xAxis->setLabel("Time (s)");
    m_ui.plot->xAxis->setLabelFont(font);
    m_ui.plot->xAxis->setTickLabelFont(font);
    m_ui.plot->yAxis->setLabel(unit.c_str());
    m_ui.plot->yAxis->setLabelFont(font);
    m_ui.plot->yAxis->setTickLabelFont(font);
    m_ui.plot->yAxis->setSelectedLabelFont(font);
    m_ui.plot->yAxis->setSelectedTickLabelFont(font);
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
    m_ui.fft->yAxis->setSelectedLabelFont(font);
    m_ui.fft->yAxis->setSelectedTickLabelFont(font);
    m_ui.fft->yAxis->grid()->setSubGridVisible(true);
    //m_ui.fft->yAxis->setRange(0, 5.f);

//    connect(customPlot->xAxis, SIGNAL(rangeChanged(QCPRange)), customPlot->xAxis2, SLOT(setRange(QCPRange)));
//    connect(customPlot->xAxis, SIGNAL(rangeChanged(QCPRange)), customPlot->xAxis2, SLOT(setRange(QCPRange)));

//    m_ui.fft->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);

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

    // myWidget is any QWidget-derived class
    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, &QWidget::customContextMenuRequested, [this](QPoint const& pos)
    {
        show_context_menu(mapToGlobal(pos));
    });

    m_ui.plot->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(m_ui.plot, &QWidget::customContextMenuRequested, [this](QPoint const& pos)
    {
        show_context_menu(m_ui.plot->mapToGlobal(pos));
    });

    m_ui.fft->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(m_ui.fft, &QWidget::customContextMenuRequested, [this](QPoint const& pos)
    {
        show_context_menu(m_ui.fft->mapToGlobal(pos));
    });

    //m_ui.window->setSingleStep(1.0 / double(m_sample_rate));
}

Numeric_Viewer::~Numeric_Viewer()
{
}

void Numeric_Viewer::show_context_menu(QPoint const& pos)
{
    QMenu menu;
    for (auto& g: m_graphs)
    {
        QImage image(10, 10, QImage::Format_ARGB32_Premultiplied);
        image.fill(g->color);
        QImage pressed_image(8, 8, QImage::Format_ARGB32_Premultiplied);
        pressed_image.fill(g->color);

        QIcon icon;
        icon.addPixmap(QPixmap::fromImage(pressed_image), QIcon::Normal, QIcon::On);
        icon.addPixmap(QPixmap::fromImage(image), QIcon::Normal, QIcon::Off);

        auto* action = menu.addAction(icon, g->name.c_str());
        action->setCheckable(true);
        action->setChecked(g->is_visible);
        auto& graph = *g;
        connect(action, &QAction::toggled, [&graph](bool yes) { graph.is_visible = yes; });
    }

    menu.addSeparator();

    auto* action = menu.addAction(QIcon(":/icons/fft.png"), "Logarithmic FFT");
    action->setCheckable(true);
    action->setChecked(m_ui.fft->xAxis->scaleType() == QCPAxis::stLogarithmic);
    connect(action, &QAction::toggled, [this](bool yes)
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

    menu.exec(pos);
}

void Numeric_Viewer::add_graph(std::string const& name, std::string const& unit, QColor color)
{
    QASSERT(!m_is_started);
    if (m_is_started)
    {
        return;
    }

    auto* plot = m_ui.plot->addGraph();
    plot->setPen(QPen(color, 2));
    plot->setAdaptiveSampling(false);
    plot->setName(name.c_str());

    auto* fft_plot = m_ui.fft->addGraph();
    fft_plot->setPen(QPen(color, 2));
    fft_plot->setBrush(color);
    fft_plot->setAdaptiveSampling(false);
    fft_plot->setName(name.c_str());

    m_graphs.emplace_back(new Graph());
    Graph& graph = *m_graphs.back();
    graph.name = name;
    graph.unit = unit;
    graph.color = color;
    graph.plot = plot;
    graph.fft_plot = fft_plot;
}

void Numeric_Viewer::add_samples(q::Clock::time_point tp, double const* src)
{
    double tpd = std::chrono::duration<double>(tp.time_since_epoch()).count();
    if (tpd < m_tp)
    {
        QLOGE("Sample from the past!!");
        return;
    }

    Sample sample;
    sample.tp = tpd;
    size_t off = m_values.size();
    m_values.resize(m_values.size() + m_graphs.size());
    std::copy(src, src + m_graphs.size(), m_values.begin() + off);
    m_temp_samples.emplace_back(std::move(sample));
    m_tp = tpd;
}

void Numeric_Viewer::process()
{
    //launch any pending task
    if (!m_is_started)
    {
        start_tasks();
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
        m_view.duration = m_samples[m_view.end_idx - 1].tp - m_samples[m_view.start_idx].tp;
    }
    else
    {
        m_view.duration = 0.0;
    }
}

void Numeric_Viewer::process_plot_task(size_t graph_idx, View const& view)
{
    Graph& graph = *m_graphs[graph_idx];
    graph.stats.average_value = 0;
    graph.stats.min_value = std::numeric_limits<double>::max();
    graph.stats.max_value = std::numeric_limits<double>::lowest();

    graph.plot_data_map.resize(0);
    graph.plot_data_map.reserve(100000);

    double pixels_per_second = static_cast<double>(m_ui.plot->width()) / view.duration;
    size_t skip = static_cast<int>(m_sample_rate / pixels_per_second) + 1;
    size_t offset = view.start_idx * m_graphs.size();
    for (size_t i = view.start_idx; i < view.end_idx; i++)
    {
        auto const& s = m_samples[i];
        auto v = m_values[offset + graph_idx];
        offset += m_graphs.size();
        graph.stats.max_value = math::max(graph.stats.max_value, v);
        graph.stats.min_value = math::min(graph.stats.min_value, v);
        if (i % skip == 0)
        {
            graph.plot_data_map.push_back(QCPData(s.tp, v));
        }
    }
    graph.stats.average_value = (graph.stats.max_value + graph.stats.min_value) * 0.5f;
}

void Numeric_Viewer::process_fft_task(size_t graph_idx, View const& view)
{
    Graph& graph = *m_graphs[graph_idx];

    auto sample_count = view.end_idx - view.start_idx;

    graph.fft_data_map.resize(0);
    graph.fft_data_map.reserve(100000);

    if (sample_count < m_sample_rate)
    {
        return;
    }

    size_t output_size = sample_count / 2 + 1;
    double div = 4.f / double(sample_count);
    double freq_div = m_sample_rate / double(sample_count);

    size_t offset = view.start_idx * m_graphs.size();
    auto* temp_input = graph.fft.temp_input.get();
    for (size_t i = 0; i < sample_count; i++)
    {
        temp_input[i] = m_values[offset + graph_idx];
        offset += m_graphs.size();
    }
    fftw_execute(graph.fft.plan);
    auto* temp_output = graph.fft.temp_output.get();

    double pixels_per_second = static_cast<double>(m_ui.plot->width()) / view.duration;
    {
        double last_key = 0;
        double max_value = std::numeric_limits<double>::lowest();
        for (size_t i = 1; i < output_size; i++)
        {
            double x = temp_output[i][0] * div;
            double y = temp_output[i][1] * div;
            double v = math::sqrt(x*x + y*y);
            max_value = math::max(max_value, v);

            double key = i * freq_div;
            auto dt = key - last_key;
            if (dt * pixels_per_second >= 0.2)
            {
                last_key = key;
                graph.fft_data_map.push_back(QCPData(key, max_value));
                max_value = std::numeric_limits<double>::lowest();
            }
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
    auto sample_count = m_view.end_idx - m_view.start_idx;
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
            m_is_started = true;

            if (sample_count != graph.fft.plan_sample_count)
            {
                graph.fft.temp_input.reset(static_cast<double*>(fftw_malloc(sample_count * sizeof(double))), fftw_free);
                graph.fft.temp_output.reset(static_cast<fftw_complex*>(fftw_malloc(sample_count * sizeof(fftw_complex))), fftw_free);
                graph.fft.plan = fftw_plan_dft_r2c_1d(sample_count, graph.fft.temp_input.get(), graph.fft.temp_output.get(), FFTW_ESTIMATE);
                graph.fft.plan_sample_count = sample_count;
            }

            graph.future = silk::async([this, view, i]()
            {
                process_plot_task(i, view);
                process_fft_task(i, view);
                std::this_thread::sleep_for(std::chrono::milliseconds(1));
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
    double min_value = std::numeric_limits<double>::max();
    double max_value = std::numeric_limits<double>::lowest();
    double max_average_value = std::numeric_limits<double>::lowest();
    double max_value_range = std::numeric_limits<double>::lowest();
    bool has_range = false;

    for (size_t i = 0; i < m_graphs.size(); i++)
    {
        auto& graph = *m_graphs[i];
        graph.plot->data()->swap(graph.plot_data_map);
        graph.fft_plot->data()->swap(graph.fft_data_map);

        if (graph.is_visible)
        {
            has_range = true;
            min_value = math::min(min_value, graph.stats.min_value);
            max_value = math::max(max_value, graph.stats.max_value);
            max_average_value = math::max(max_average_value, graph.stats.average_value);
            max_value_range = math::max(max_value_range, graph.stats.max_value - graph.stats.min_value);
        }
    }

    if (!has_range)
    {
        min_value = 0;
        max_value = 0;
        max_average_value = 0;
    }

    m_ui.data_range->setValue(max_value_range);
    m_ui.average->setValue(max_average_value);
    m_ui.plot->xAxis->setRange(m_samples[m_view.start_idx].tp, m_samples[m_view.start_idx].tp + m_view.duration);

    auto display_range = m_ui.display_range->value();
    if (display_range == 0)
    {
        m_ui.plot->yAxis->setRange(min_value, max_value);
        m_ui.fft->yAxis->setRange(0, max_value - min_value);
    }
    else
    {
        auto average_value = (max_value + min_value) * 0.5;
        m_ui.plot->yAxis->setRange(average_value - display_range / 2, average_value + display_range / 2);
        m_ui.fft->yAxis->setRange(0, (display_range));
    }

    m_ui.plot->replot(/*QCustomPlot::rpImmediate*/);
    m_ui.fft->replot();
}
