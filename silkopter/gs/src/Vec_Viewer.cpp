#include "stdafx.h"
#include "Vec_Viewer.h"
#include "HAL.h"
#include "Comms.h"
#include "ui_Vec_Viewer.h"

Vec_Viewer::Vec_Viewer(std::string const& unit, uint32_t sample_rate, QWidget *parent)
    : QWidget(parent)
    , m_unit(unit)
{
    m_fft.sample_rate = sample_rate;

    m_ui.setupUi(this);

    m_ui.plot->addGraph();
    m_ui.plot->graph(0)->setPen(QColor(0xe74c3c));
    m_ui.plot->graph(0)->setAdaptiveSampling(false);
    m_ui.plot->addGraph();
    m_ui.plot->graph(1)->setPen(QColor(0x2ecc71));
    m_ui.plot->graph(1)->setAdaptiveSampling(false);
    m_ui.plot->addGraph();
    m_ui.plot->graph(2)->setPen(QColor(0x3498db));
    m_ui.plot->graph(2)->setAdaptiveSampling(false);
    m_ui.plot->addGraph();
    m_ui.plot->graph(3)->setPen(QColor(0x8e44ad));
    m_ui.plot->graph(3)->setAdaptiveSampling(false);
    m_ui.plot->xAxis->setLabel("Time (s)");
    m_ui.plot->yAxis->setLabel(unit.c_str());

    m_ui.fft->addGraph();
    m_ui.fft->graph(0)->setPen(QColor(0xe74c3c));
    m_ui.fft->graph(0)->setBrush(QColor(0xe74c3c));
    m_ui.fft->graph(0)->setAdaptiveSampling(false);
    m_ui.fft->addGraph();
    m_ui.fft->graph(1)->setPen(QColor(0x2ecc71));
    m_ui.fft->graph(1)->setBrush(QColor(0x2ecc71));
    m_ui.fft->graph(1)->setAdaptiveSampling(false);
    m_ui.fft->addGraph();
    m_ui.fft->graph(2)->setPen(QColor(0x3498db));
    m_ui.fft->graph(2)->setBrush(QColor(0x3498db));
    m_ui.fft->graph(2)->setAdaptiveSampling(false);
    m_ui.fft->addGraph();
    m_ui.fft->graph(3)->setPen(QColor(0x8e44ad));
    m_ui.fft->graph(3)->setBrush(QColor(0x8e44ad));
    m_ui.fft->graph(3)->setAdaptiveSampling(false);
    m_ui.fft->xAxis->setLabel("Frequency (Hz)");
    m_ui.fft->yAxis->setLabel(unit.c_str());

    connect(m_ui.logarithmic, &QCheckBox::toggled, [this](bool yes)
    {
        m_ui.fft->xAxis->setScaleType(yes ? QCPAxis::stLogarithmic : QCPAxis::stLinear);
    });

    m_ui.progress->setValue(m_ui.progress->maximum());
    connect(m_ui.progress, &QScrollBar::valueChanged, [this](int pos)
    {
        if (pos == m_ui.progress->maximum())
        {
            m_range.is_at_end = true;
        }
        else
        {
            m_range.is_at_end = false;
            if (m_samples.empty())
            {
                m_range.end_idx = 0;
            }
            else
            {
                float ratio = static_cast<float>(pos - m_ui.progress->minimum()) / static_cast<float>(m_ui.progress->maximum() - m_ui.progress->minimum());
                auto end = m_samples.size() - 1;
                m_range.end_idx = math::clamp(static_cast<size_t>(ratio * end), size_t(0), end);
            }
            m_range.start_idx = m_range.end_idx;
        }
    });

    connect(m_ui.component_x, &QCheckBox::toggled, [this](bool yes) { m_components.x = m_allowed_components.x & yes; });
    connect(m_ui.component_y, &QCheckBox::toggled, [this](bool yes) { m_components.y = m_allowed_components.y & yes; });
    connect(m_ui.component_z, &QCheckBox::toggled, [this](bool yes) { m_components.z = m_allowed_components.z & yes; });
    connect(m_ui.component_w, &QCheckBox::toggled, [this](bool yes) { m_components.w = m_allowed_components.w & yes; });

    m_fft.temp_input.reset(static_cast<float*>(fftwf_malloc(FFT_Data::MAX_INPUT_SIZE * sizeof(float))), fftwf_free);
    m_fft.temp_output.reset(static_cast<fftwf_complex*>(fftwf_malloc(FFT_Data::MAX_INPUT_SIZE * sizeof(fftwf_complex))), fftwf_free);
}

Vec_Viewer::~Vec_Viewer()
{
}

void Vec_Viewer::add_sample(q::Clock::duration dt, math::vec4f const& value)
{
    Sample sample;
    sample.time = m_time;
    sample.value = value;
    m_samples.emplace_back(sample);
    m_time += dt;
}

void Vec_Viewer::set_components(math::vec4<bool> const& components)
{
    m_allowed_components = components;
    m_components = components;
    m_ui.component_x->setVisible(m_allowed_components.x);
    m_ui.component_y->setVisible(m_allowed_components.y);
    m_ui.component_z->setVisible(m_allowed_components.z);
    m_ui.component_w->setVisible(m_allowed_components.w);

    m_ui.component_x->setChecked(m_allowed_components.x);
    m_ui.component_y->setChecked(m_allowed_components.y);
    m_ui.component_z->setChecked(m_allowed_components.z);
    m_ui.component_w->setChecked(m_allowed_components.w);
}

void Vec_Viewer::process()
{
    auto time = std::chrono::duration_cast<q::Clock::duration>(q::Seconds(m_ui.window->value()));

    //update range
    if (!m_samples.empty())
    {
        if (m_range.is_at_end)
        {
            m_range.end_idx = m_samples.size() - 1;
        }
        while (m_range.end_idx > m_range.start_idx &&
               m_samples[m_range.end_idx].time - m_samples[m_range.start_idx].time > time)
        {
            m_range.start_idx++;
        }
        while (m_range.start_idx > 0 &&
               m_samples[m_range.end_idx].time - m_samples[m_range.start_idx].time < time)
        {
            m_range.start_idx--;
        }
        m_range.time = m_samples[m_range.end_idx].time - m_samples[m_range.start_idx].time;
    }

    auto now = q::Clock::now();
    if (now - m_last_time_point > std::chrono::milliseconds(20))
    {
        m_last_time_point = now;

        process_plot();
        process_fft(m_fft);
    }
}

void Vec_Viewer::process_fft(FFT_Data& fft)
{
    if (fft.sample_rate == 0 || m_samples.empty())
    {
        return;
    }

    auto sample_count = m_range.end_idx - m_range.start_idx + 1;
    if (sample_count < 2)
    {
        return;
    }

    //clear plot
    if (!m_components.x)
    {
        m_ui.fft->plottable(0)->clearData();
    }
    if (!m_components.y)
    {
        m_ui.fft->plottable(1)->clearData();
    }
    if (!m_components.z)
    {
        m_ui.fft->plottable(2)->clearData();
    }
    if (!m_components.w)
    {
        m_ui.fft->plottable(3)->clearData();
    }

    if (sample_count != fft.plan_sample_count)
    {
        fft.plan = fftwf_plan_dft_r2c_1d(sample_count, fft.temp_input.get(), fft.temp_output.get(), FFTW_ESTIMATE);
        fft.plan_sample_count = sample_count;

        //clear plot
        for (int i = 0; i < m_ui.fft->plottableCount(); i++)
        {
            m_ui.fft->plottable(i)->clearData();
        }
    }

    float range = m_range.max_value_scalar - m_range.min_value_scalar;
    float div = 1.f / float(sample_count);
    size_t output_size = sample_count / 2 + 1;

    auto freq_div = (fft.sample_rate / 2) / float(output_size);

    auto* temp_output = fft.temp_output.get();
    if (m_components.x)
    {
        auto* temp_input = fft.temp_input.get();
        for (size_t i = m_range.start_idx; i <= m_range.end_idx; i++)
        {
            *temp_input++ = m_samples[i].value.x - m_range.average_value.x;
        }
        fftwf_execute(fft.plan);
        auto* temp_output = fft.temp_output.get();
        auto* plottable = m_ui.fft->graph(0);
        for (size_t i = 1; i < output_size; i++)
        {
            plottable->addData(i * freq_div, math::sqrt(temp_output[i][0]*temp_output[i][0] + temp_output[i][1]*temp_output[i][1]) * div);
        }
    }
    if (m_components.y)
    {
        auto* temp_input = fft.temp_input.get();
        for (size_t i = m_range.start_idx; i <= m_range.end_idx; i++)
        {
            *temp_input++ = m_samples[i].value.y - m_range.average_value.y;
        }
        fftwf_execute(fft.plan);
        auto* temp_output = fft.temp_output.get();
        auto* plottable = m_ui.fft->graph(1);
        for (size_t i = 1; i < output_size; i++)
        {
            plottable->addData(i * freq_div, math::sqrt(temp_output[i][0]*temp_output[i][0] + temp_output[i][1]*temp_output[i][1]) * div);
        }
    }
    if (m_components.z)
    {
        auto* temp_input = fft.temp_input.get();
        for (size_t i = m_range.start_idx; i <= m_range.end_idx; i++)
        {
            *temp_input++ = m_samples[i].value.z - m_range.average_value.z;
        }
        fftwf_execute(fft.plan);
        auto* temp_output = fft.temp_output.get();
        auto* plottable = m_ui.fft->graph(2);
        for (size_t i = 1; i < output_size; i++)
        {
            plottable->addData(i * freq_div, math::sqrt(temp_output[i][0]*temp_output[i][0] + temp_output[i][1]*temp_output[i][1]) * div);
        }
    }
    if (m_components.w)
    {
        auto* temp_input = fft.temp_input.get();
        for (size_t i = m_range.start_idx; i <= m_range.end_idx; i++)
        {
            *temp_input++ = m_samples[i].value.w - m_range.average_value.w;
        }
        fftwf_execute(fft.plan);
        auto* plottable = m_ui.fft->graph(3);
        for (size_t i = 1; i < output_size; i++)
        {
            plottable->addData(i * freq_div, math::sqrt(temp_output[i][0]*temp_output[i][0] + temp_output[i][1]*temp_output[i][1]) * div);
        }
    }

    m_ui.fft->xAxis->setRange(0.5, fft.sample_rate / 2);
    m_ui.fft->yAxis->setRange(0, math::sqrt(range));
    m_ui.fft->replot();
}

void Vec_Viewer::process_plot()
{
    m_range.average_value = math::vec4f::zero;
    m_range.average_value_scalar = 0;
    m_range.min_value_scalar = std::numeric_limits<float>::max();
    m_range.max_value_scalar = std::numeric_limits<float>::lowest();
    m_range.min_value = math::vec4f(m_range.min_value_scalar);
    m_range.max_value = math::vec4f(m_range.max_value_scalar);

    auto* graph_x = m_ui.plot->graph(0);
    graph_x->clearData();
    auto* graph_y = m_ui.plot->graph(1);
    graph_y->clearData();
    auto* graph_z = m_ui.plot->graph(2);
    graph_z->clearData();
    auto* graph_w = m_ui.plot->graph(3);
    graph_w->clearData();

    float rangef = q::Seconds(m_range.time).count();
    float widthf = static_cast<float>(m_ui.plot->width());
    float pixels_per_second = widthf / rangef;
    float last_time = 0;
    math::vec4f min_value(std::numeric_limits<float>::max());
    math::vec4f max_value(std::numeric_limits<float>::lowest());
    for (size_t i = m_range.start_idx; i <= m_range.end_idx; i++)
    {
        auto const& s = m_samples[i];
        min_value = math::min(min_value, s.value);
        max_value = math::max(max_value, s.value);

        auto key = q::Seconds(s.time).count();
        auto dt = key - last_time;
        if (dt * pixels_per_second > 0.5f)
        {
            last_time = key;
            if (m_components.x)
            {
                m_range.min_value_scalar = math::min(m_range.min_value_scalar, min_value.x);
                m_range.max_value_scalar = math::max(m_range.max_value_scalar, max_value.x);
                graph_x->addData(key, min_value.x);
                graph_x->addData(key + 0.0001f, max_value.x);
            }
            if (m_components.y)
            {
                m_range.min_value_scalar = math::min(m_range.min_value_scalar, min_value.y);
                m_range.max_value_scalar = math::max(m_range.max_value_scalar, max_value.y);
                graph_y->addData(key, min_value.y);
                graph_y->addData(key + 0.0001f, max_value.y);
            }
            if (m_components.z)
            {
                m_range.min_value_scalar = math::min(m_range.min_value_scalar, min_value.z);
                m_range.max_value_scalar = math::max(m_range.max_value_scalar, max_value.z);
                graph_z->addData(key, min_value.z);
                graph_z->addData(key + 0.0001f, max_value.z);
            }
            if (m_components.w)
            {
                m_range.min_value_scalar = math::min(m_range.min_value_scalar, min_value.w);
                m_range.max_value_scalar = math::max(m_range.max_value_scalar, max_value.w);
                graph_w->addData(key, min_value.w);
                graph_w->addData(key + 0.0001f, max_value.w);
            }
            min_value = math::vec4f(std::numeric_limits<float>::max());
            max_value = math::vec4f(std::numeric_limits<float>::lowest());
        }
    }
    m_range.average_value = (m_range.max_value + m_range.min_value) * 0.5f;
    m_range.average_value_scalar = (m_range.max_value_scalar + m_range.min_value_scalar) * 0.5f;

    m_ui.data_range->setValue(m_range.max_value_scalar - m_range.min_value_scalar);
    m_ui.average->setValue(m_range.average_value_scalar);

    m_ui.plot->xAxis->rescale(true);

    auto display_range = m_ui.display_range->value();
    if (display_range == 0)
    {
        m_ui.plot->yAxis->setRange(m_range.min_value_scalar, m_range.max_value_scalar);
    }
    else
    {
        m_ui.plot->yAxis->setRange(m_range.average_value_scalar - display_range / 2, m_range.average_value_scalar + display_range / 2);
    }

    m_ui.plot->replot();
}
