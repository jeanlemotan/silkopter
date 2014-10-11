#include "stdafx.h"
#include "Sensors.h"
#include "physics/constants.h"


//class Butterworth //8Hz
//{
//public:
//    static constexpr size_t NZEROS  = 4;
//    static constexpr size_t NPOLES  = 4;
//    static constexpr double GAIN    = 2.674241096e+06;
//    double xv[NZEROS+1], yv[NPOLES+1];
//    double process(double t)
//    {
//        xv[0] = xv[1]; xv[1] = xv[2]; xv[2] = xv[3]; xv[3] = xv[4];
//                xv[4] = t / GAIN;
//                yv[0] = yv[1]; yv[1] = yv[2]; yv[2] = yv[3]; yv[3] = yv[4];
//                yv[4] =   (xv[0] + xv[4]) + 4 * (xv[1] + xv[3]) + 6 * xv[2]
//                             + ( -0.8768965608 * yv[0]) + (  3.6227607596 * yv[1])
//                             + ( -5.6145268496 * yv[2]) + (  3.8686566679 * yv[3]);
//        return yv[4];
//    }
//};

//class Butterworth //10hz
//{
//public:
//    static constexpr size_t NZEROS  = 6;
//    static constexpr size_t NPOLES  = 6;
//    static constexpr double GAIN    = 1.172113723e+09;
//    double xv[NZEROS+1], yv[NPOLES+1];
//    double process(double t)
//    {
//        xv[0] = xv[1]; xv[1] = xv[2]; xv[2] = xv[3]; xv[3] = xv[4]; xv[4] = xv[5]; xv[5] = xv[6];
//                xv[6] = t / GAIN;
//                yv[0] = yv[1]; yv[1] = yv[2]; yv[2] = yv[3]; yv[3] = yv[4]; yv[4] = yv[5]; yv[5] = yv[6];
//                yv[6] =   (xv[0] + xv[6]) + 6 * (xv[1] + xv[5]) + 15 * (xv[2] + xv[4])
//                             + 20 * xv[3]
//                             + ( -0.7844171769 * yv[0]) + (  4.8969248914 * yv[1])
//                             + (-12.7416173290 * yv[2]) + ( 17.6873761800 * yv[3])
//                             + (-13.8155108060 * yv[4]) + (  5.7572441862 * yv[5]);
//        return yv[6];
//    }
//};

class Butterworth //10hz
{
public:
    static constexpr size_t NZEROS  = 2;
    static constexpr size_t NPOLES  = 2;
    static constexpr float GAIN    = 1.058546241e+03;
    float xv[NZEROS+1], yv[NPOLES+1];
    float process(float t)
    {
        xv[0] = xv[1]; xv[1] = xv[2];
        xv[2] = t / GAIN;
        yv[0] = yv[1]; yv[1] = yv[2];
        yv[2] =   (xv[0] + xv[2]) + 2 * xv[1]
                   + ( -0.9149758348f * yv[0]) + (  1.9111970674f * yv[1]);
        return yv[2];
    }
};

//class Butterworth //4, 10hz
//{
//public:
//    static constexpr size_t NZEROS  = 4;
//    static constexpr size_t NPOLES  = 4;
//    static constexpr float GAIN    = 1.112983215e+06;
//    float xv[NZEROS+1], yv[NPOLES+1];
//    float process(float t)
//      {
//        xv[0] = xv[1]; xv[1] = xv[2]; xv[2] = xv[3]; xv[3] = xv[4];
//                xv[4] = t / GAIN;
//                yv[0] = yv[1]; yv[1] = yv[2]; yv[2] = yv[3]; yv[3] = yv[4];
//                yv[4] =   (xv[0] + xv[4]) + 4 * (xv[1] + xv[3]) + 6 * xv[2]
//                             + ( -0.8485559993f * yv[0]) + (  3.5335352195f * yv[1])
//                             + ( -5.5208191366f * yv[2]) + (  3.8358255406f * yv[3]);
//        return yv[4];
//      }
//};


Butterworth gfx, gfy, gfz;
Butterworth afx, afy, afz;





Sensors::Sensors(QWidget *parent /* = 0 */)
	: QWidget(parent)
    , m_comms(nullptr)
{
	m_ui.setupUi(this);

    m_ui.a_plot->addGraph();
    m_ui.a_plot->graph(0)->setPen(QPen(QColor(255, 0, 0, 64)));
    m_ui.a_plot->addGraph();
    m_ui.a_plot->graph(1)->setPen(QPen(QColor(0, 255, 0, 64)));
    m_ui.a_plot->addGraph();
    m_ui.a_plot->graph(2)->setPen(QPen(QColor(0, 0, 255, 64)));

    m_ui.a_plot->addGraph();
    m_ui.a_plot->graph(3)->setPen(QPen(Qt::red));
    m_ui.a_plot->addGraph();
    m_ui.a_plot->graph(4)->setPen(QPen(Qt::green));
    m_ui.a_plot->addGraph();
    m_ui.a_plot->graph(5)->setPen(QPen(Qt::blue));

//    m_ui.a_plot->setInteractions(QCP::Interactions(QCP::iRangeZoom | QCP::iRangeDrag));

    m_ui.a_spectrum->addGraph();
    m_ui.a_spectrum->graph(0)->setPen(QPen(QColor(255, 0, 0, 64)));
    m_ui.a_spectrum->addGraph();
    m_ui.a_spectrum->graph(1)->setPen(QPen(QColor(0, 255, 0, 64)));
    m_ui.a_spectrum->addGraph();
    m_ui.a_spectrum->graph(2)->setPen(QPen(QColor(0, 0, 255, 64)));

    m_ui.a_spectrum->addGraph();
    m_ui.a_spectrum->graph(3)->setPen(QPen(Qt::red));
    m_ui.a_spectrum->addGraph();
    m_ui.a_spectrum->graph(4)->setPen(QPen(Qt::green));
    m_ui.a_spectrum->addGraph();
    m_ui.a_spectrum->graph(5)->setPen(QPen(Qt::blue));
    m_ui.a_spectrum->yAxis->setRange(0, 0.1f);

    m_ui.g_plot->addGraph();
    m_ui.g_plot->graph(0)->setPen(QPen(QColor(255, 0, 0, 64)));
    m_ui.g_plot->addGraph();
    m_ui.g_plot->graph(1)->setPen(QPen(QColor(0, 255, 0, 64)));
    m_ui.g_plot->addGraph();
    m_ui.g_plot->graph(2)->setPen(QPen(QColor(0, 0, 255, 64)));

    m_ui.g_plot->addGraph();
    m_ui.g_plot->graph(3)->setPen(QPen(Qt::red));
    m_ui.g_plot->addGraph();
    m_ui.g_plot->graph(4)->setPen(QPen(Qt::green));
    m_ui.g_plot->addGraph();
    m_ui.g_plot->graph(5)->setPen(QPen(Qt::blue));

    m_ui.g_spectrum->addGraph();
    m_ui.g_spectrum->graph(0)->setPen(QPen(QColor(255, 0, 0, 64)));
    m_ui.g_spectrum->addGraph();
    m_ui.g_spectrum->graph(1)->setPen(QPen(QColor(0, 255, 0, 64)));
    m_ui.g_spectrum->addGraph();
    m_ui.g_spectrum->graph(2)->setPen(QPen(QColor(0, 0, 255, 64)));

    m_ui.g_spectrum->addGraph();
    m_ui.g_spectrum->graph(3)->setPen(QPen(Qt::red));
    m_ui.g_spectrum->addGraph();
    m_ui.g_spectrum->graph(4)->setPen(QPen(Qt::green));
    m_ui.g_spectrum->addGraph();
    m_ui.g_spectrum->graph(5)->setPen(QPen(Qt::blue));
    m_ui.g_spectrum->yAxis->setRange(0, 0.1f);

    m_ui.c_plot->addGraph();
    m_ui.c_plot->graph(0)->setPen(QPen(Qt::red));
    m_ui.c_plot->addGraph();
    m_ui.c_plot->graph(1)->setPen(QPen(Qt::green));
    m_ui.c_plot->addGraph();
    m_ui.c_plot->graph(2)->setPen(QPen(Qt::blue));

	m_ui.barometer_plot->addGraph();
	m_ui.barometer_plot->graph(0)->setPen(QPen(Qt::red));
    m_ui.barometer_plot->addGraph();
    m_ui.barometer_plot->graph(1)->setPen(QPen(Qt::blue));

    m_ui.sonar_plot->addGraph();
    m_ui.sonar_plot->graph(0)->setPen(QPen(Qt::red));
    m_ui.sonar_plot->addGraph();
    m_ui.sonar_plot->graph(1)->setPen(QPen(Qt::blue));

    m_ui.thermometer_plot->addGraph();
    m_ui.thermometer_plot->graph(0)->setPen(QPen(Qt::red));
    m_ui.thermometer_plot->addGraph();
    m_ui.thermometer_plot->graph(1)->setPen(QPen(Qt::blue));

    connect(m_ui.a_calibrate, &QPushButton::released, this, &Sensors::start_accelerometer_calibration);
    connect(m_ui.g_calibrate, &QPushButton::released, this, &Sensors::start_gyroscope_calibration);
    connect(m_ui.c_calibrate, &QPushButton::released, this, &Sensors::start_compass_calibration);

    connect(m_ui.save_history, &QToolButton::released, this, &Sensors::dump_history_to_file);
    connect(m_ui.load_history, &QToolButton::released, this, &Sensors::load_history);
    connect(m_ui.clear_history, &QToolButton::released, this, &Sensors::clear_history);
    connect(m_ui.rewind_history, &QToolButton::released, this, &Sensors::rewind);
    connect(m_ui.play_history, &QToolButton::toggled, [this](bool checked) { m_playback.is_playing = checked; } );

    m_last_time = q::Clock::now();

    m_calibration.message_box.setParent(this);

    // Allocate input and output buffers
    m_gyroscope_fft.temp_input.reset(static_cast<double*>(fftw_malloc(FFT_Data::MAX_INPUT_SIZE * sizeof(double))), fftw_free);
    m_gyroscope_fft.temp_output.reset(static_cast<fftw_complex*>(fftw_malloc(FFT_Data::MAX_INPUT_SIZE * sizeof(fftw_complex))), fftw_free);

    m_accelerometer_fft.temp_input.reset(static_cast<double*>(fftw_malloc(FFT_Data::MAX_INPUT_SIZE * sizeof(double))), fftw_free);
    m_accelerometer_fft.temp_output.reset(static_cast<fftw_complex*>(fftw_malloc(FFT_Data::MAX_INPUT_SIZE * sizeof(fftw_complex))), fftw_free);

    q::util::fs::create_folder(q::Path("sensor_data"));

}

Sensors::~Sensors()
{
}

void Sensors::init(silk::Comms* comms)
{
    m_comms = comms;
}

void Sensors::process()
{
    auto now = q::Clock::now();
    auto dt = now - m_last_time;
	m_last_time = now;

    QASSERT(m_comms);

    m_ui.a_calibrate->setEnabled(m_comms->is_connected());
    m_ui.a_test_filter->setEnabled(m_comms->is_connected());
    m_ui.g_calibrate->setEnabled(m_comms->is_connected());
    m_ui.g_test_filter->setEnabled(m_comms->is_connected());

    static const std::chrono::seconds graph_length(2);

    auto old_time = m_playback.time_point;
    if (m_playback.is_playing)
    {
        m_playback.time_point += dt;
    }
    //QLOG_INFO("xxx", "{}", m_history_playback.time);

    if (isVisible() && (m_comms->is_connected() || m_playback.is_loaded))
	{
        {
            std::vector<silk::Gyroscope_Sample> data;

            if (m_playback.is_loaded)
            {
                for (size_t i = m_playback.last_gyroscope_idx; i < m_playback.gyroscope_samples.size(); i++)
                {
                    auto const& s = m_playback.gyroscope_samples[i];
                    if (s.time_point >= old_time && s.time_point < m_playback.time_point)
                    {
                        data.push_back(s);
                        m_playback.last_gyroscope_idx = i;
                    }
                    else if (s.time_point >= m_playback.time_point)
                    {
                        break;
                    }
                }
            }
            else
            {
                data = m_comms->get_gyroscope_samples();
            }

            if (!data.empty())
            {
                float time_increment = q::Seconds(data.front().dt).count();
                m_gyroscope_fft.sample_rate = static_cast<size_t>(1.f / time_increment);

                for (auto const& av: data)
                {
                    auto const& value = av.value;
                    m_gyroscope_sample_time += time_increment;

                    add_plot_sample(*m_ui.g_plot, m_gyroscope_sample_time, value);
                    m_history.gyroscope_samples.emplace_back(m_gyroscope_sample_time, value);

                    math::vec3f f(gfx.process(value.x), gfy.process(value.y), gfz.process(value.z));

                    add_plot_filtered_sample(*m_ui.g_plot, m_gyroscope_sample_time, f);
                    m_history.gyroscope_filtered_samples.emplace_back(m_gyroscope_sample_time, f);
                }
            }
            if (m_gyroscope_sample_time > q::Seconds(graph_length).count())
            {
                remove_plot_data_before(*m_ui.g_plot, graph_length);
            }
            m_ui.g_plot->rescaleAxes(true);
            m_ui.g_plot->replot();

            if (!m_playback.is_loaded || m_playback.is_playing)
            {
                clear_plot(*m_ui.g_spectrum);
                prepare_fft(*m_ui.g_spectrum, m_history.gyroscope_samples, m_gyroscope_fft.input);
                prepare_fft(*m_ui.g_spectrum, m_history.gyroscope_filtered_samples, m_gyroscope_fft.filtered_input);
                process_fft(m_gyroscope_fft);
                display_fft(*m_ui.g_spectrum, m_gyroscope_fft);
                display_filtered_fft(*m_ui.g_spectrum, m_gyroscope_fft);
                m_ui.g_spectrum->rescaleAxes(true);
                m_ui.g_spectrum->replot();
            }
        }

        {
            silk::Comms::Accelerometer_Data data;

            if (m_playback.is_loaded)
            {
                for (size_t i = m_playback.last_accelerometer_idx; i < m_playback.accelerometer_samples.size(); i++)
                {
                    auto const& s = m_playback.accelerometer_samples[i];
                    if (s.first >= old_time && s.first < m_playback.time)
                    {
                        data.value.accelerations.push_back(s.second);
                        m_playback.last_accelerometer_idx = i;
                    }
                    else if (s.first >= m_playback.time)
                    {
                        break;
                    }
                }
                if (!data.value.accelerations.empty())
                {
                    data.value.sample_time = std::chrono::milliseconds(1);
                    data.sample_idx++;
                    data.timestamp = now;
                }
            }
            else
            {
                data = m_comms->get_sensor_accelerometer_data();
            }

            m_accelerometer_fft.sample_rate = static_cast<size_t>(1.f / q::Seconds(data.value.sample_time).count());

            float time_increment = q::Seconds(data.value.sample_time).count();
            for (auto& av: data.value.accelerations)
            {
                m_accelerometer_sample_time += time_increment;
                add_plot_sample(*m_ui.a_plot, m_accelerometer_sample_time, av);
                m_history.accelerometer_samples.emplace_back(m_accelerometer_sample_time, av);

                math::vec3f f(afx.process(av.x), afy.process(av.y), afz.process(av.z));

                add_plot_filtered_sample(*m_ui.a_plot, m_accelerometer_sample_time, f);
                m_history.accelerometer_filtered_samples.emplace_back(m_accelerometer_sample_time, f);
            }

            if (m_accelerometer_sample_time > q::Seconds(graph_length).count())
            {
                remove_plot_data_before(*m_ui.a_plot, graph_length);
            }
            m_ui.a_plot->rescaleAxes(true);
            m_ui.a_plot->replot();

            if (!m_playback.is_loaded || m_playback.is_playing)
            {
                clear_plot(*m_ui.a_spectrum);
                prepare_fft(*m_ui.a_spectrum, m_history.accelerometer_samples, m_accelerometer_fft.input);
                prepare_fft(*m_ui.a_spectrum, m_history.accelerometer_filtered_samples, m_accelerometer_fft.filtered_input);
                process_fft(m_accelerometer_fft);
                display_fft(*m_ui.a_spectrum, m_accelerometer_fft);
                display_filtered_fft(*m_ui.a_spectrum, m_accelerometer_fft);
                m_ui.a_spectrum->rescaleAxes(true);
                m_ui.a_spectrum->replot();
            }
        }


        auto alive = m_comms->get_uav_alive_duration();
        if (m_comms->is_connected() && alive != m_last_uav_alive_duration)
		{
            m_last_uav_alive_duration = alive;

            double seconds = q::Seconds(alive).count();
            //static double seconds = 0;// double(time_ms) / 1000.0;
			//seconds += 0.01f;

// 			static float s_cpu = 0;
// 			s_cpu = math::lerp(s_cpu, m_protocol->data_board_cpu_usage.value, 0.01f);
// 			m_ui.g_plot->graph(0)->addData(seconds, s_cpu);
            {
                auto data = m_comms->get_sensor_compass_data();
                auto direction = math::normalized<float, math::safe>(data.value);
                m_ui.c_plot->graph(0)->addData(seconds, direction.x);
                m_ui.c_plot->graph(1)->addData(seconds, direction.y);
                m_ui.c_plot->graph(2)->addData(seconds, direction.z);
                m_history.compass_samples.emplace_back(seconds, direction);
                //SILK_INFO("compass: {}", data.value);
            }

            {
                auto data = m_comms->get_sensor_sonar_data();
                m_ui.sonar_plot->graph(0)->addData(seconds, data.value);
                m_average_sonar = math::lerp(m_average_sonar, data.value, 0.1f);
                m_ui.sonar_plot->graph(1)->addData(seconds, m_average_sonar);
            }
            {
                auto data = m_comms->get_sensor_barometer_data();
                m_ui.barometer_plot->graph(0)->addData(seconds, data.value);
                m_average_barometer = math::lerp(m_average_barometer, data.value, 0.01f);
                m_ui.barometer_plot->graph(1)->addData(seconds, m_average_barometer);
            }
            {
                auto data = m_comms->get_sensor_thermometer_data();
                m_ui.thermometer_plot->graph(0)->addData(seconds, data.value);
                m_average_thermometer = math::lerp(m_average_thermometer, data.value, 0.01f);
                m_ui.thermometer_plot->graph(1)->addData(seconds, m_average_thermometer);
            }

			//printf("\n%f, %f, %f", m_protocol->data_board_accelerometer.value.x, m_protocol->data_board_accelerometer.value.y, m_protocol->data_board_accelerometer.value.z);

			//////////////////////////////////////////////////////////////////////////

            if (seconds > q::Seconds(graph_length).count())
            {
                remove_plot_data_before(*m_ui.c_plot, graph_length);
                remove_plot_data_before(*m_ui.barometer_plot, graph_length);
                remove_plot_data_before(*m_ui.sonar_plot, graph_length);
                remove_plot_data_before(*m_ui.thermometer_plot, graph_length);
            }


            m_ui.c_plot->rescaleAxes(true);
            m_ui.c_plot->replot();

			m_ui.barometer_plot->rescaleAxes(true);
			m_ui.barometer_plot->replot();

            m_ui.sonar_plot->rescaleAxes(true);
			m_ui.sonar_plot->replot();

            m_ui.thermometer_plot->rescaleAxes(true);
            m_ui.thermometer_plot->replot();
        }
	}
	else
	{
        m_last_uav_alive_duration = q::Clock::duration();
        m_gyroscope_sample_time = 0;
        m_accelerometer_sample_time = 0;

        clear_plot(*m_ui.g_plot);
        clear_plot(*m_ui.g_spectrum);
        clear_plot(*m_ui.a_plot);
        clear_plot(*m_ui.a_spectrum);
        clear_plot(*m_ui.c_plot);
        clear_plot(*m_ui.barometer_plot);
        clear_plot(*m_ui.sonar_plot);
        clear_plot(*m_ui.thermometer_plot);
    }

    update_calibration();
}

void Sensors::update_calibration()
{
    if (m_calibration.step == Calibration::Step::IDLE)
    {
        return;
    }

    switch(m_calibration.sensor)
    {
    case Calibration::Sensor::ACCELEROMETER: update_accelerometer_calibration(); break;
    case Calibration::Sensor::GYROSCOPE: update_gyroscope_calibration(); break;
    case Calibration::Sensor::COMPASS: update_compass_calibration(); break;
    }
}

void Sensors::start_accelerometer_calibration()
{
    m_calibration.sensor = Calibration::Sensor::ACCELEROMETER;
    m_calibration.step = Calibration::Step::START;
    m_calibration.step_timepoint = q::Clock::now();
    m_calibration.collect_data_step = 0;

    std::string msg;
    q::util::format(msg, "Accelerometer calibration will start now.\nMake sure you can rotate the UAV when instructed.");

    m_calibration.message_box.setWindowTitle("Accelerometer Calibration");
    m_calibration.message_box.setText(msg.c_str());
    m_calibration.message_box.setWindowModality(Qt::ApplicationModal);
    m_calibration.message_box.show();
}

void Sensors::update_accelerometer_calibration()
{
    std::chrono::seconds duration(5);

    auto now = q::Clock::now();

    char const* direction_str[] =
    {
        "level",
        "on its LEFT side",
        "on its RIGHT side",
        "nose DOWN",
        "nose UP",
        "on its BACK",
    };

    //q::util::format(msg, "Place the board {} for {} seconds.\nPress Ok to continue.", direction_str[step], duration.count());


    //first reset the bias
    if (m_calibration.step == Calibration::Step::START)
    {
        if (!m_calibration.message_box.isVisible())
        {
            m_calibration.connection = m_comms->accelerometer_calibration_data_received.connect([this](math::vec3f const& bias, math::vec3f const& scale)
            {
                m_calibration.received_bias = bias;
                m_calibration.received_scale = scale;
                m_calibration.is_received_data_valid = true;
            });

            m_calibration.step = Calibration::Step::RESET;
            m_calibration.step_timepoint = q::Clock::now();
            m_calibration.is_received_data_valid = false;

            m_comms->set_accelerometer_calibration_data(math::vec3f::zero, math::vec3f::one);
        }
    }
    else if (m_calibration.step == Calibration::Step::RESET)
    {
        if (now - m_calibration.step_timepoint > std::chrono::seconds(1))
        {
            QMessageBox::critical(this, "Error", "Cannot start the calibration procedure.\nFailed to reset the sensors.");
            m_calibration.step = Calibration::Step::IDLE;
            return;
        }
        if (m_calibration.is_received_data_valid &&
                math::is_zero(m_calibration.received_bias) &&
                math::is_one(m_calibration.received_scale))
        {
            m_calibration.step = Calibration::Step::SHOW_INSTRUCTIONS;
            m_calibration.step_timepoint = q::Clock::now();

            std::string msg;
            q::util::format(msg, "Place the board {} for {} seconds.\nPress Ok to continue.", direction_str[m_calibration.collect_data_step], duration.count());
            m_calibration.message_box.setStandardButtons(QMessageBox::Button::Ok);
            m_calibration.message_box.setText(msg.c_str());
            m_calibration.message_box.setWindowModality(Qt::NonModal);
            m_calibration.message_box.show();
        }
    }
    else if (m_calibration.step == Calibration::Step::SHOW_INSTRUCTIONS)
    {
        if (!m_calibration.message_box.isVisible())
        {
            m_calibration.step = Calibration::Step::COLLECT_DATA;
            m_calibration.step_timepoint = q::Clock::now();
            m_calibration.samples.clear();
            m_calibration.message_box.setStandardButtons(0);
            m_calibration.message_box.show();
        }
    }
    else if (m_calibration.step == Calibration::Step::COLLECT_DATA)
    {
        auto& samples = m_calibration.samples;

        auto time_passed = now - m_calibration.step_timepoint;
        if (time_passed < duration)
        {
            auto data = m_comms->get_sensor_accelerometer_data();
            if (data.timestamp > m_calibration.last_data_timepoint && time_passed > std::chrono::seconds(2))
            {
                m_calibration.last_data_timepoint = data.timestamp;
                std::copy(data.value.accelerations.begin(), data.value.accelerations.end(), std::back_inserter(samples));
            }

            auto left = std::chrono::duration_cast<std::chrono::seconds>(duration - time_passed).count();
            left = std::max<int>(left, 0);

            std::string msg;
            q::util::format(msg, "Collected {} sample{}...\n{} second{} left.",
                            samples.size(),
                            samples.size() != 1 ? "s" : "",
                            left,
                            left != 1 ? "s" : "");

            m_calibration.message_box.setText(msg.c_str());
        }
        else
        {
            m_calibration.message_box.hide();

            //make sure we have enough data
            if (samples.size() < 60)
            {
                QMessageBox::critical(this, "Error", "Calibration failed - not enough samples collected!");
                m_calibration.step = Calibration::Step::IDLE;
            }
            else
            {
                m_calibration.averages[m_calibration.collect_data_step].set(std::accumulate(samples.begin(), samples.end(), math::vec3f()));
                m_calibration.averages[m_calibration.collect_data_step] /= static_cast<double>(samples.size());

                m_calibration.collect_data_step++;
                if (m_calibration.collect_data_step == 6)
                {
                    math::vec3<double> bias, scale;
                    calibrate_accelerometer(m_calibration.averages, bias, scale);

                    m_calibration.new_bias = math::vec3f(bias);
                    m_calibration.new_scale = math::vec3f(scale);
                    m_calibration.is_received_data_valid = false;
                    m_comms->set_accelerometer_calibration_data(math::vec3f(bias), math::vec3f(scale));

                    m_calibration.step = Calibration::Step::SET;
                    m_calibration.step_timepoint = q::Clock::now();
                }
                else
                {
                    m_calibration.step = Calibration::Step::SHOW_INSTRUCTIONS;
                    m_calibration.step_timepoint = q::Clock::now();

                    std::string msg;
                    q::util::format(msg, "Place the board {} for {} seconds.\nPress Ok to continue.", direction_str[m_calibration.collect_data_step], duration.count());
                    m_calibration.message_box.setStandardButtons(QMessageBox::Button::Ok);
                    m_calibration.message_box.setText(msg.c_str());
                    m_calibration.message_box.show();
                }
            }
        }
    }
    else if (m_calibration.step == Calibration::Step::SET)
    {
        if (now - m_calibration.step_timepoint > std::chrono::seconds(1))
        {
            QMessageBox::critical(this, "Error", "Failed to set new calibration data.\nRepeat the procedure please.");
            m_calibration.step = Calibration::Step::IDLE;
            m_calibration.connection.disconnect();
            return;
        }

        if (m_calibration.is_received_data_valid &&
                math::equals(m_calibration.received_bias, m_calibration.new_bias) &&
                math::equals(m_calibration.received_scale, m_calibration.new_scale))
        {
            m_calibration.step = Calibration::Step::IDLE;

            std::string msg;
            q::util::format(msg, "New accelerometer bias / scale: {.9} / {.9}.", m_calibration.received_bias, m_calibration.received_scale);
            m_calibration.message_box.setText(msg.c_str());
            m_calibration.message_box.setStandardButtons(QMessageBox::Button::Ok);
            m_calibration.message_box.exec();
            m_calibration.connection.disconnect();
            return;
        }
    }
}

void Sensors::start_gyroscope_calibration()
{
    m_calibration.sensor = Calibration::Sensor::GYROSCOPE;
    m_calibration.step = Calibration::Step::START;
    m_calibration.step_timepoint = q::Clock::now();

    std::string msg;
    q::util::format(msg, "Place the board level and completely still");

    m_calibration.message_box.setWindowTitle("Gyroscope Calibration");
    m_calibration.message_box.setText(msg.c_str());
    m_calibration.message_box.setWindowModality(Qt::ApplicationModal);
    m_calibration.message_box.show();
}

void Sensors::update_gyroscope_calibration()
{
    std::chrono::seconds duration(15);

    auto now = q::Clock::now();

	//first reset the bias
    if (m_calibration.step == Calibration::Step::START)
    {
        if (!m_calibration.message_box.isVisible())
        {
            m_calibration.connection = m_comms->gyroscope_calibration_data_received.connect([this](math::vec3f const& bias)
            {
                m_calibration.received_bias = bias;
                m_calibration.is_received_data_valid = true;
            });

            m_calibration.is_received_data_valid = false;
            m_calibration.step = Calibration::Step::RESET;
            m_calibration.step_timepoint = q::Clock::now();

            m_comms->set_gyroscope_calibration_data(math::vec3f::zero);
        }
    }
    else if (m_calibration.step == Calibration::Step::RESET)
	{
        if (now - m_calibration.step_timepoint > std::chrono::seconds(1))
        {
            QMessageBox::critical(this, "Error", "Cannot start the calibration procedure.\nFailed to reset the sensors.");
            m_calibration.step = Calibration::Step::IDLE;
            return;
        }

        if (m_calibration.is_received_data_valid &&
                math::is_zero(m_calibration.received_bias))
        {
            m_calibration.step = Calibration::Step::COLLECT_DATA;
            m_calibration.step_timepoint = q::Clock::now();
            m_calibration.samples.clear();

            m_calibration.message_box.setStandardButtons(0);
            m_calibration.message_box.setText(QString());
            m_calibration.message_box.setWindowModality(Qt::NonModal);
            m_calibration.message_box.show();
            return;
        }
	}
    else if (m_calibration.step == Calibration::Step::COLLECT_DATA)
    {
        auto& samples = m_calibration.samples;

        auto time_passed = now - m_calibration.step_timepoint;
        if (time_passed < duration)
        {
            auto data = m_comms->get_sensor_gyroscope_data();
            if (data.timestamp > m_calibration.last_data_timepoint && time_passed > std::chrono::seconds(2))
            {
                m_calibration.last_data_timepoint = data.timestamp;
                std::copy(data.value.angular_velocities.begin(), data.value.angular_velocities.end(), std::back_inserter(samples));
            }

            auto left = std::chrono::duration_cast<std::chrono::seconds>(duration - time_passed).count();
            left = std::max<int>(left, 0);

            auto bias = std::accumulate(samples.begin(), samples.end(), math::vec3f::zero);
            if (!samples.empty())
            {
                bias /= static_cast<float>(samples.size());
            }

            std::string msg;
            q::util::format(msg, "Collected {} sample{}...\n{} second{} left.\nApproximated bias: {.9}",
                            samples.size(),
                            samples.size() != 1 ? "s" : "",
                            left,
                            left != 1 ? "s" : "",
                            bias);

            m_calibration.message_box.setText(msg.c_str());
        }
        else
        {
            m_calibration.message_box.hide();
            //make sure we have enough data
            if (samples.size() < 100)
            {
                QMessageBox::critical(this, "Error", "Calibration failed - not enough samples collected!");
                m_calibration.step = Calibration::Step::IDLE;
            }
            else
            {
                auto bias = std::accumulate(samples.begin(), samples.end(), math::vec3f::zero);
                bias /= static_cast<float>(samples.size());

                m_calibration.is_received_data_valid = false;
                m_calibration.new_bias = bias;
                m_calibration.step = Calibration::Step::SET;
                m_calibration.step_timepoint = q::Clock::now();

                m_comms->set_gyroscope_calibration_data(bias);
            }
        }
    }
    else if (m_calibration.step == Calibration::Step::SET)
    {
        if (now - m_calibration.step_timepoint > std::chrono::seconds(1))
        {
            QMessageBox::critical(this, "Error", "Failed to set new calibration data.\nRepeat the procedure please.");
            m_calibration.step = Calibration::Step::IDLE;
            m_calibration.connection.disconnect();
            return;
        }

        if (m_calibration.is_received_data_valid &&
                math::equals(m_calibration.received_bias, m_calibration.new_bias))
        {
            m_calibration.step = Calibration::Step::IDLE;

            std::string msg;
            q::util::format(msg, "New gyroscope bias: {.9}.", m_calibration.received_bias);
            m_calibration.message_box.setText(msg.c_str());
            m_calibration.message_box.setStandardButtons(QMessageBox::Button::Ok);
            m_calibration.message_box.exec();
            m_calibration.connection.disconnect();
            return;
        }
    }
}

void Sensors::start_compass_calibration()
{
    m_calibration.sensor = Calibration::Sensor::COMPASS;
    m_calibration.step = Calibration::Step::START;
    m_calibration.step_timepoint = q::Clock::now();

    std::string msg;
    q::util::format(msg, "Rotate the board along all axes.");

    m_calibration.message_box.setWindowTitle("Compass Calibration");
    m_calibration.message_box.setText(msg.c_str());
    m_calibration.message_box.setWindowModality(Qt::ApplicationModal);
    m_calibration.message_box.show();
}

void Sensors::update_compass_calibration()
{
    std::chrono::seconds duration(60);

    auto now = q::Clock::now();

    //first reset the bias
    if (m_calibration.step == Calibration::Step::START)
    {
        if (!m_calibration.message_box.isVisible())
        {
            m_calibration.connection = m_comms->compass_calibration_data_received.connect([this](math::vec3f const& bias)
            {
                m_calibration.received_bias = bias;
                m_calibration.is_received_data_valid = true;
            });

            m_calibration.is_received_data_valid = false;
            m_calibration.step = Calibration::Step::RESET;
            m_calibration.step_timepoint = q::Clock::now();

            m_comms->set_compass_calibration_data(math::vec3f::zero);
        }
    }
    else if (m_calibration.step == Calibration::Step::RESET)
    {
        if (now - m_calibration.step_timepoint > std::chrono::seconds(1))
        {
            QMessageBox::critical(this, "Error", "Cannot start the calibration procedure.\nFailed to reset the sensors.");
            m_calibration.step = Calibration::Step::IDLE;
            return;
        }

        if (m_calibration.is_received_data_valid &&
                math::is_zero(m_calibration.received_bias))
        {
            m_calibration.step = Calibration::Step::COLLECT_DATA;
            m_calibration.step_timepoint = q::Clock::now();
            m_calibration.samples.clear();

            m_calibration.message_box.setStandardButtons(0);
            m_calibration.message_box.setText(QString());
            m_calibration.message_box.setWindowModality(Qt::NonModal);
            m_calibration.message_box.show();
            return;
        }
    }
    else if (m_calibration.step == Calibration::Step::COLLECT_DATA)
    {
        auto& samples = m_calibration.samples;

        auto time_passed = now - m_calibration.step_timepoint;
        if (time_passed < duration)
        {
            auto data = m_comms->get_sensor_compass_data();
            if (data.timestamp > m_calibration.last_data_timepoint && time_passed > std::chrono::seconds(2))
            {
                m_calibration.last_data_timepoint = data.timestamp;
                samples.push_back(data.value);
            }

            auto left = std::chrono::duration_cast<std::chrono::seconds>(duration - time_passed).count();
            left = std::max<int>(left, 0);

            math::aabb3f box(samples.empty() ? math::vec3f::zero : samples[0]);
            for (auto const& s: samples)
            {
                box.add_internal_point(s);
            }

            auto bias = box.get_center();

            std::string msg;
            q::util::format(msg, "Collected {} sample{}...\n{} second{} left.\nApproximated bias: {.9}",
                            samples.size(),
                            samples.size() != 1 ? "s" : "",
                            left,
                            left != 1 ? "s" : "",
                            bias);

            m_calibration.message_box.setText(msg.c_str());
        }
        else
        {
            m_calibration.message_box.hide();
            //make sure we have enough data
            if (samples.size() < 100)
            {
                QMessageBox::critical(this, "Error", "Calibration failed - not enough samples collected!");
                m_calibration.step = Calibration::Step::IDLE;
            }
            else
            {
                math::aabb3f box(samples.empty() ? math::vec3f::zero : samples[0]);
                for (auto const& s: samples)
                {
                    box.add_internal_point(s);
                }

                auto bias = box.get_center();

                m_calibration.new_bias = bias;
                m_calibration.step = Calibration::Step::SET;
                m_calibration.step_timepoint = q::Clock::now();
                m_calibration.is_received_data_valid = false;

                m_comms->set_compass_calibration_data(bias);
            }
        }
    }
    else if (m_calibration.step == Calibration::Step::SET)
    {
        if (now - m_calibration.step_timepoint > std::chrono::seconds(1))
        {
            QMessageBox::critical(this, "Error", "Failed to set new calibration data.\nRepeat the procedure please.");
            m_calibration.step = Calibration::Step::IDLE;
            m_calibration.connection.disconnect();
            return;
        }

        if (m_calibration.is_received_data_valid &&
                math::equals(m_calibration.received_bias, m_calibration.new_bias))
        {
            m_calibration.step = Calibration::Step::IDLE;

            std::string msg;
            q::util::format(msg, "New compass bias: {.9}.", m_calibration.received_bias);
            m_calibration.message_box.setText(msg.c_str());
            m_calibration.message_box.setStandardButtons(QMessageBox::Button::Ok);
            m_calibration.message_box.exec();
            m_calibration.connection.disconnect();
            return;
        }
    }
}

bool Sensors::calibrate_accelerometer(std::array<math::vec3<double>, 6> const& samples, math::vec3<double>& bias, math::vec3<double>& scale)
{
	//code copied from arducopter
	int iteration_count = 0;
	static const double s_eps = 0.000000001;
	static const int s_iterations = 2000;
	double change = 100.0;
	double data[3];
	double beta[6];
	double delta[6];
	double ds[6];
	double JS[6][6];
	bool success = true;

	// reset
	beta[0] = beta[1] = beta[2] = 0;
	beta[3] = beta[4] = beta[5] = 1.0f / physics::constants::g;

	while (iteration_count < s_iterations && change > s_eps)
	{
		iteration_count++;

		calibrate_reset_matrices(ds, JS);

		for (int i = 0; i < 6; i++)
		{
			data[0] = samples[i].x;
			data[1] = samples[i].y;
			data[2] = samples[i].z;
			calibrate_update_matrices(ds, JS, beta, data);
		}

		calibrate_find_delta(ds, JS, delta);

		change = delta[0] * delta[0] +
			delta[0] * delta[0] +
			delta[1] * delta[1] +
			delta[2] * delta[2] +
			delta[3] * delta[3] / (beta[3] * beta[3]) +
			delta[4] * delta[4] / (beta[4] * beta[4]) +
			delta[5] * delta[5] / (beta[5] * beta[5]);

		for (int i = 0; i < 6; i++)
		{
			beta[i] -= delta[i];
		}
	}

	// copy results out
	scale.x = beta[3] * physics::constants::g;
	scale.y = beta[4] * physics::constants::g;
	scale.z = beta[5] * physics::constants::g;
	bias.x = beta[0] * scale.x;
	bias.y = beta[1] * scale.y;
	bias.z = beta[2] * scale.z;

	// sanity check scale
// 	if (scale.is_nan() || fabsf(scale.x - 1.0f) > 0.1f || fabsf(scale.y - 1.0f) > 0.1f || fabsf(scale.z - 1.0f) > 0.1f) 
// 	{
// 		success = false;
// 	}
// 	// sanity check offsets (3.5 is roughly 3/10th of a G, 5.0 is roughly half a G)
// 	if (bias.is_nan() || fabsf(bias.x) > 3.5f || fabsf(bias.y) > 3.5f || fabsf(bias.z) > 3.5f) 
// 	{
// 		success = false;
// 	}

	// return success or failure
	return success;
}

void Sensors::calibrate_update_matrices(double dS[6], double JS[6][6], double beta[6], double data[3])
{
	//code copied from arducopter
	double residual = 1.0;
	double jacobian[6];

	for (int j = 0; j < 3; j++)
	{
		double b = beta[3 + j];
		double dx = data[j] - beta[j];
		residual -= b*b*dx*dx;
		jacobian[j] = 2.0f*b*b*dx;
		jacobian[3 + j] = -2.0f*b*dx*dx;
	}

	for (int j = 0; j < 6; j++)
	{
		dS[j] += jacobian[j] * residual;
		for (int k = 0; k < 6; k++)
		{
			JS[j][k] += jacobian[j] * jacobian[k];
		}
	}
}


// _calibrate_reset_matrices - clears matrices
void Sensors::calibrate_reset_matrices(double dS[6], double JS[6][6])
{
	//code copied from arducopter
	for (int j = 0; j < 6; j++)
	{
		dS[j] = 0.0f;
		for (int k = 0; k < 6; k++)
		{
			JS[j][k] = 0.0f;
		}
	}
}

void Sensors::calibrate_find_delta(double dS[6], double JS[6][6], double delta[6])
{
	//code copied from arducopter
	//Solve 6-d matrix equation JS*x = dS
	//first put in upper triangular form

	//make upper triangular
	for (int i = 0; i < 6; i++)
	{
		//eliminate all nonzero entries below JS[i][i]
		for (int j = i + 1; j < 6; j++)
		{
			double mu = JS[i][j] / JS[i][i];
			if (mu != 0.0) 
			{
				dS[j] -= mu*dS[i];
				for (int k = j; k < 6; k++)
				{
					JS[k][j] -= mu*JS[k][i];
				}
			}
		}
	}

	//back-substitute
	for (int i = 5; i >= 0; i--)
	{
		dS[i] /= JS[i][i];
		JS[i][i] = 1.0;

		for (int j = 0; j < i; j++)
		{
			double mu = JS[i][j];
			dS[j] -= mu*dS[i];
			JS[i][j] = 0.0;
		}
	}

	for (int i = 0; i < 6; i++)
	{
		delta[i] = dS[i];
	}
}

void Sensors::dump_history_to_file()
{
    do
    {
        SILK_INFO("cwd: {}", q::util::fs::get_current_folder());
        std::string filepath;
        q::util::format(filepath, "sensor_data/sensors-{}.history", ++m_history.last_file_idx);
        if (!q::util::fs::exists(q::Path(filepath)))
        {
            q::data::File_Sink file((q::Path(filepath)));
            if (!file.is_open())
            {
                continue;
            }

            file << m_history.gyroscope_samples;
            file << m_history.accelerometer_samples;
            file << m_history.compass_samples;
            break;
        }
    } while (m_history.last_file_idx < 10000);

    clear_history();
}

void Sensors::load_history_from_file(q::Path const& filepath)
{
    q::data::File_Source file((filepath));
    if (!file.is_open())
    {
        return;
    }

    file >> m_playback.gyroscope_samples;
    file >> m_playback.accelerometer_samples;
    file >> m_playback.compass_samples;

    rewind();
    m_playback.is_loaded = true;
    m_playback.is_playing = true;
}

void Sensors::load_history()
{
    auto fn = QFileDialog::getOpenFileName(this, "Open History", "sensor_data", "History Files(*.history)");
    if (fn.count() == 0)
    {
        return;
    }

    load_history_from_file(q::Path(fn.toLatin1().data()));

}
void Sensors::clear_history()
{
    m_history.gyroscope_samples.clear();
    m_history.gyroscope_filtered_samples.clear();
    m_history.accelerometer_samples.clear();
    m_history.accelerometer_filtered_samples.clear();
    m_history.compass_samples.clear();
    m_history.compass_filtered_samples.clear();

    m_playback.gyroscope_samples.clear();
    m_playback.accelerometer_samples.clear();
    m_playback.compass_samples.clear();

    if (m_playback.is_loaded)
    {
        m_playback.is_loaded = false;

        clear_plot(*m_ui.a_plot);
        clear_plot(*m_ui.g_plot);
        clear_plot(*m_ui.g_spectrum);
        clear_plot(*m_ui.a_spectrum);
    }
}

void Sensors::rewind()
{
    m_history.gyroscope_samples.clear();
    m_history.gyroscope_filtered_samples.clear();
    m_history.accelerometer_samples.clear();
    m_history.accelerometer_filtered_samples.clear();
    m_history.compass_samples.clear();
    m_history.compass_filtered_samples.clear();

    m_playback.time = m_playback.gyroscope_samples.empty() ? 0 : m_playback.gyroscope_samples.front().first;
    m_playback.last_accelerometer_idx = 0;
    m_playback.last_gyroscope_idx = 0;
    m_playback.last_compass_idx = 0;

    clear_plot(*m_ui.g_plot);
    clear_plot(*m_ui.a_plot);
    clear_plot(*m_ui.g_spectrum);
    clear_plot(*m_ui.a_spectrum);
}


void Sensors::remove_plot_data_before(QCustomPlot& plot, q::Clock::duration length)
{
    for (int i = 0; i < plot.graphCount(); i++)
    {
        auto graph = plot.graph(i);
        if (graph && !graph->data()->empty())
        {
            plot.graph(i)->removeDataBefore(plot.graph(i)->data()->last().key - q::Seconds(length).count());
        }
    }
    plot.rescaleAxes(true);
}
void Sensors::clear_plot(QCustomPlot& plot)
{
    for (int i = 0; i < plot.graphCount(); i++)
    {
        auto graph = plot.graph(i);
        if (graph)
        {
            plot.graph(i)->clearData();
        }
    }
//    plot.rescaleAxes(true);
    for (int i = 0; i < plot.plottableCount(); i++)
    {
        plot.plottable(i)->rescaleAxes();
    }
}
void Sensors::add_plot_sample(QCustomPlot& plot, float key, math::vec3f const& sample)
{
    plot.graph(0)->addData(key, sample.x);
    plot.graph(1)->addData(key, sample.y);
    plot.graph(2)->addData(key, sample.z);
}
void Sensors::add_plot_filtered_sample(QCustomPlot& plot, float key, math::vec3f const& sample)
{
    plot.graph(3)->addData(key, sample.x);
    plot.graph(4)->addData(key, sample.y);
    plot.graph(5)->addData(key, sample.z);
}

void Sensors::prepare_fft(QCustomPlot& plot, std::vector<std::pair<float, math::vec3f>> const& samples, std::vector<math::vec3f>& output)
{
    float min = plot.graph(0)->keyAxis()->range().minRange;
    float max = plot.graph(0)->keyAxis()->range().maxRange;
    output.clear();
    for (auto const& v: samples)
    {
        if (v.first >= min && v.first < max)
        {
            output.push_back(v.second);
        }
        else if (v.first >= max)
        {
            break;
        }
    }
}
void Sensors::display_fft(QCustomPlot& plot, FFT_Data const& fft)
{
    if (!fft.output.empty())
    {
        for (size_t i = 1; i < fft.output.size(); i++)
        {
            auto const& s = fft.output[i];
            auto freq = (fft.sample_rate / 2) * i / fft.output.size();
            add_plot_sample(plot, freq, s);
        }
//        for (int i = 0; i < plot.plottableCount(); i++)
//        {
//            plot.plottable(i)->rescaleAxes();
//        }
//        plot.replot();
    }
}
void Sensors::display_filtered_fft(QCustomPlot& plot, FFT_Data const& fft)
{
    if (!fft.filtered_output.empty())
    {
        for (size_t i = 1; i < fft.filtered_output.size(); i++)
        {
            auto const& s = fft.filtered_output[i];
            auto freq = (fft.sample_rate / 2) * i / fft.filtered_output.size();
            add_plot_filtered_sample(plot, freq, s);
        }
//        for (int i = 0; i < plot.plottableCount(); i++)
//        {
//            plot.plottable(i)->rescaleAxes();
//        }
//        plot.replot();
    }
}

static void process_fft(fftw_plan& plan, size_t required_sample_count, std::vector<math::vec3f> const& input, std::vector<math::vec3f>& output, double* temp_input, fftw_complex* temp_output)
{
    auto start = input.end() - required_sample_count;
    auto end = input.end();

    //first remove DC
    math::vec3f median;
    for (auto it = start; it != end; ++it)
    {
        median += *it;
    }
    float div = 1.f / required_sample_count;
    median *= div;

    size_t output_size = required_sample_count/2 + 1;
    output.resize(output_size);

    //compute fft per component
    std::transform(start, end, temp_input, [median](math::vec3f const& v) { return v.x - median.x; });
    fftw_execute(plan);
    for (size_t i = 0; i < output_size; i++)
    {
        output[i].x = math::sqrt(temp_output[i][0]*temp_output[i][0] + temp_output[i][1]*temp_output[i][1]) * div;
    }

    std::transform(start, end, temp_input, [median](math::vec3f const& v) { return v.y - median.y; });
    fftw_execute(plan);
    for (size_t i = 0; i < output_size; i++)
    {
        output[i].y = math::sqrt(temp_output[i][0]*temp_output[i][0] + temp_output[i][1]*temp_output[i][1]) * div;
    }

    std::transform(start, end, temp_input, [median](math::vec3f const& v) { return v.z - median.z; });
    fftw_execute(plan);
    for (size_t i = 0; i < output_size; i++)
    {
        output[i].z = math::sqrt(temp_output[i][0]*temp_output[i][0] + temp_output[i][1]*temp_output[i][1]) * div;
    }
}

void Sensors::process_fft(FFT_Data& fft)
{
    const std::chrono::milliseconds required_duration(1000);
    const size_t required_sample_count = math::min(
                static_cast<size_t>(fft.sample_rate * q::Seconds(required_duration).count()),
                fft.MAX_INPUT_SIZE);
    if (fft.input.size() < required_sample_count || required_sample_count == 0)
    {
        fft.output.resize(0);
        return;
    }

    if (required_sample_count != fft.plan_sample_count)
    {
        fft.plan = fftw_plan_dft_r2c_1d(required_sample_count, fft.temp_input.get(), fft.temp_output.get(), FFTW_ESTIMATE);
        fft.plan_sample_count = required_sample_count;
    }

    ::process_fft(fft.plan, required_sample_count, fft.input, fft.output, fft.temp_input.get(), fft.temp_output.get());
    ::process_fft(fft.plan, required_sample_count, fft.filtered_input, fft.filtered_output, fft.temp_input.get(), fft.temp_output.get());
}
