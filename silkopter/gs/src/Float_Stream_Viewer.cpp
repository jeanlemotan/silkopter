#include "stdafx.h"
#include "Float_Stream_Viewer.h"
#include "HAL.h"
#include "Comms.h"
#include "ui_Float_Stream_Viewer.h"

Float_Stream_Viewer::Float_Stream_Viewer(std::string const& unit, uint32_t sample_rate, QWidget *parent)
    : QWidget(parent)
    , m_unit(unit)
{
    m_fft.sample_rate = sample_rate;

    m_ui.setupUi(this);

    m_ui.plot->addGraph();
    m_ui.plot->graph(0)->setPen(QColor(0x2c3e50));
    m_ui.plot->xAxis->setLabel("Time (s)");
    m_ui.plot->yAxis->setLabel(unit.c_str());
    m_ui.plot->graph(0)->setAdaptiveSampling(false);

    m_fft_plottable = new QCPGraph(m_ui.fft->xAxis, m_ui.fft->yAxis);
    m_fft_plottable->setAdaptiveSampling(false);
    m_ui.fft->addPlottable(m_fft_plottable);
    m_fft_plottable->setPen(QColor(0x2980b9));
    m_fft_plottable->setBrush(QColor(0x2980b9));
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

    m_fft.temp_input.reset(static_cast<float*>(fftwf_malloc(FFT_Data::MAX_INPUT_SIZE * sizeof(float))), fftwf_free);
    m_fft.temp_output.reset(static_cast<fftwf_complex*>(fftwf_malloc(FFT_Data::MAX_INPUT_SIZE * sizeof(fftwf_complex))), fftwf_free);
}

Float_Stream_Viewer::~Float_Stream_Viewer()
{
}

void Float_Stream_Viewer::add_sample(q::Clock::duration dt, float value)
{
    Sample sample;
    sample.time = m_time;
    sample.value = value;
    m_samples.emplace_back(sample);
    m_time += dt;
}

void Float_Stream_Viewer::process()
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

void Float_Stream_Viewer::process_fft(FFT_Data& fft)
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

    if (sample_count != fft.plan_sample_count)
    {
        fft.plan = fftwf_plan_dft_r2c_1d(sample_count, fft.temp_input.get(), fft.temp_output.get(), FFTW_ESTIMATE);
        fft.plan_sample_count = sample_count;

        //clear plot
        for (int i = 0; i < m_ui.fft->plottableCount(); i++)
        {
            m_ui.fft->plottable(i)->clearData();
            m_ui.fft->plottable(i)->rescaleAxes();
        }
    }

    //remove DC
    {
        auto* temp_input = fft.temp_input.get();
        for (size_t i = m_range.start_idx; i <= m_range.end_idx; i++)
        {
            auto const& s = m_samples[i];
            *temp_input++ = s.value - m_range.average_value;
        }
    }

    float range = m_range.max_value - m_range.min_value;
    float div = 1.f / float(sample_count);

    size_t output_size = sample_count / 2 + 1;

    fftwf_execute(fft.plan);

    auto* temp_output = fft.temp_output.get();
    auto freq_div = (fft.sample_rate / 2) / float(output_size);
    for (size_t i = 1; i < output_size; i++)
    {
        float x = temp_output[i][0];
        float y = temp_output[i][1];
        float s = math::sqrt(x*x + y*y) * div;
        m_fft_plottable->addData(i * freq_div, s);
    }
    m_ui.fft->xAxis->setRange(0.5, fft.sample_rate / 2);
    m_ui.fft->yAxis->setRange(0, math::sqrt(range));
    m_ui.fft->replot();
}

void Float_Stream_Viewer::process_plot()
{
    m_range.average_value = 0.f;
    m_range.min_value = std::numeric_limits<float>::max();
    m_range.max_value = std::numeric_limits<float>::lowest();

    auto* graph = m_ui.plot->graph(0);
    graph->clearData();
    float rangef = q::Seconds(m_range.time).count();
    float widthf = static_cast<float>(m_ui.plot->width());
    float pixels_per_second = widthf / rangef;
    float last_time = 0;
    float min_value = std::numeric_limits<float>::max();
    float max_value = std::numeric_limits<float>::lowest();
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
            graph->addData(key, min_value);
            graph->addData(key + 0.0001f, max_value);
            min_value = std::numeric_limits<float>::max();
            max_value = std::numeric_limits<float>::lowest();
        }

        //collect input
        m_range.average_value += s.value;
        m_range.min_value = math::min(m_range.min_value, s.value);
        m_range.max_value = math::max(m_range.max_value, s.value);
    }
    m_range.average_value /= float(m_range.end_idx - m_range.start_idx + 1);

    m_ui.data_range->setValue(m_range.max_value - m_range.min_value);
    m_ui.average->setValue(m_range.average_value);

    m_ui.plot->xAxis->rescale(true);

    auto display_range = m_ui.display_range->value();
    if (display_range == 0)
    {
        m_ui.plot->yAxis->setRange(m_range.min_value, m_range.max_value);
    }
    else
    {
        m_ui.plot->yAxis->setRange(m_range.average_value - display_range / 2, m_range.average_value + display_range / 2);
    }

    m_ui.plot->replot();
}
