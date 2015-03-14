#pragma once

#include "HAL.h"
#include "Comms.h"
#include "ui_Float_Stream_Viewer.h"

template<class Stream>
class Float_Stream_Viewer : public QWidget
{
public:
    Float_Stream_Viewer(std::string const& unit, std::shared_ptr<Stream> stream, QWidget *parent)
        : QWidget(parent)
        , m_stream(stream)
        , m_unit(unit)
    {
        m_ui.setupUi(this);

        m_ui.plot->addGraph();
        m_ui.plot->graph(0)->setPen(QColor(0x2c3e50));
        m_ui.plot->xAxis->setLabel("Time (s)");
        m_ui.plot->yAxis->setLabel(unit.c_str());

        m_spectogram_plottable = new QCPGraph(m_ui.spectogram->xAxis, m_ui.spectogram->yAxis);
        m_ui.spectogram->addPlottable(m_spectogram_plottable);
        m_spectogram_plottable->setPen(QColor(0x2980b9));
        m_spectogram_plottable->setBrush(QColor(0x2980b9));
        m_ui.spectogram->xAxis->setLabel("Frequency (Hz)");
        m_ui.spectogram->yAxis->setLabel(unit.c_str());

        connect(m_ui.logarithmic, &QCheckBox::toggled, [this](bool yes)
        {
            m_ui.spectogram->xAxis->setScaleType(yes ? QCPAxis::stLogarithmic : QCPAxis::stLinear);
        });

        m_fft.temp_input.reset(static_cast<double*>(fftw_malloc(FFT_Data::MAX_INPUT_SIZE * sizeof(double))), fftw_free);
        m_fft.temp_output.reset(static_cast<fftw_complex*>(fftw_malloc(FFT_Data::MAX_INPUT_SIZE * sizeof(fftw_complex))), fftw_free);

        m_connection = stream->samples_available_signal.connect([this](Stream& stream)
        {
            process_samples(stream);
        });
    }

    ~Float_Stream_Viewer()
    {
    }

private:
    Ui::Float_Stream_Viewer m_ui;
    QCPGraph* m_spectogram_plottable = nullptr;
    silk::node::stream::Stream_wptr m_stream;
    std::string m_unit;
    q::util::Scoped_Connection m_connection;
    std::chrono::duration<double> m_time = std::chrono::duration<double>(0);
    q::Clock::time_point m_last_time_point = q::Clock::now();

    struct FFT_Data
    {
        size_t sample_rate = 0;
        std::vector<double> input;

        static const size_t MAX_INPUT_SIZE = 65536;
        std::shared_ptr<double> temp_input;
        std::shared_ptr<fftw_complex> temp_output;
        fftw_plan plan;
        size_t plan_sample_count = 0;
    } m_fft;

    void process_fft(FFT_Data& fft)
    {
        //clear plot
        for (int i = 0; i < m_ui.spectogram->plottableCount(); i++)
        {
            m_ui.spectogram->plottable(i)->clearData();
            m_ui.spectogram->plottable(i)->rescaleAxes();
        }
        if (fft.sample_rate == 0)
        {
            return;
        }

        //collect input
        double max_time = m_time.count();
        double min_time = math::max(max_time - m_ui.window->value(), 0.0);
        size_t needed_samples = static_cast<size_t>((max_time - min_time) * fft.sample_rate);
        fft.input.clear();
        double median = 0.0; //to remove DC
        double min = std::numeric_limits<double>::max();
        double max = std::numeric_limits<double>::lowest();
        {
            QCPDataMap* data = m_ui.plot->graph(0)->data();
            auto it = data->lowerBound(max_time);
            if (it == data->end())
            {
                it = data->end();
                --it;
            }
            while (it != data->begin() && it != data->end() && fft.input.size() < needed_samples)
            {
                auto value = it.value().value;
                median += value;
                min = math::min(min, value);
                max = math::max(max, value);
                fft.input.push_back(value);
                --it;
            }
            if (!fft.input.empty())
            {
                median /= double(fft.input.size());
            }
        }
        if (fft.input.size() < 2)
        {
            return;
        }
        if (fft.input.size() != fft.plan_sample_count)
        {
            fft.plan = fftw_plan_dft_r2c_1d(fft.input.size(), fft.temp_input.get(), fft.temp_output.get(), FFTW_ESTIMATE);
            fft.plan_sample_count = fft.input.size();
        }

        double range = max - min;
        double div = 1.0 / double(fft.input.size());
        median *= div;

        size_t output_size = fft.input.size() / 2 + 1;

        //compute fft per component
        auto* temp_input = fft.temp_input.get();
        std::transform(fft.input.begin(), fft.input.end(), temp_input, [median](double v) { return v - median; });

        fftw_execute(fft.plan);

        auto* temp_output = fft.temp_output.get();
        auto freq_div = (fft.sample_rate / 2) / double(output_size);
        for (size_t i = 1; i < output_size; i++)
        {
            auto x = temp_output[i][0];
            auto y = temp_output[i][1];
            auto s = math::sqrt(x*x + y*y) * div;
            m_spectogram_plottable->addData(i * freq_div, s);
        }
        m_ui.spectogram->xAxis->setRange(0.5, fft.sample_rate / 2);
        m_ui.spectogram->yAxis->setRange(0, math::sqrt(range));
        m_ui.spectogram->replot();
    }

    void process_samples(Stream& stream)
    {
        m_fft.sample_rate = stream.rate;

        for (auto const& s: stream.samples)
        {
            m_ui.plot->graph(0)->addData(m_time.count(), s.value);
            m_time += s.dt;
        }

        auto now = q::Clock::now();
        if (now - m_last_time_point > std::chrono::milliseconds(20))
        {
            m_last_time_point = now;

            m_ui.plot->xAxis->setRange(math::max(m_time.count() - m_ui.window->value(), 0.0), m_time.count());
            m_ui.plot->yAxis->rescale(true);
            m_ui.plot->replot();

            process_fft(m_fft);
        }
    }
};
