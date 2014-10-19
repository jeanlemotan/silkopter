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
    m_ui.a_plot->graph(0)->setPen(QPen(Qt::red));
    m_ui.a_plot->addGraph();
    m_ui.a_plot->graph(1)->setPen(QPen(Qt::green));
    m_ui.a_plot->addGraph();
    m_ui.a_plot->graph(2)->setPen(QPen(Qt::blue));

//    m_ui.a_plot->setInteractions(QCP::Interactions(QCP::iRangeZoom | QCP::iRangeDrag));

    m_ui.g_plot->addGraph();
    m_ui.g_plot->graph(0)->setPen(QPen(Qt::red));
    m_ui.g_plot->addGraph();
    m_ui.g_plot->graph(1)->setPen(QPen(Qt::green));
    m_ui.g_plot->addGraph();
    m_ui.g_plot->graph(2)->setPen(QPen(Qt::blue));

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

    m_last_time = q::Clock::now();

    m_calibration.message_box.setParent(this);
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
    m_ui.g_calibrate->setEnabled(m_comms->is_connected());

    static const std::chrono::seconds graph_length(2);

    if (isVisible() && m_comms->is_connected())
	{
        m_plot_x_value += q::Seconds(dt).count();

        {
            auto const& sample = m_comms->get_gyroscope_sample();
            add_plot_sample(*m_ui.g_plot, m_plot_x_value, sample.value);
            if (m_plot_x_value > q::Seconds(graph_length).count())
            {
                remove_plot_data_before(*m_ui.g_plot, graph_length);
            }
            m_ui.g_plot->rescaleAxes(true);
            m_ui.g_plot->replot();
        }

        {
            auto const& sample = m_comms->get_accelerometer_sample();
            add_plot_sample(*m_ui.a_plot, m_plot_x_value, sample.value);
            if (m_plot_x_value > q::Seconds(graph_length).count())
            {
                remove_plot_data_before(*m_ui.a_plot, graph_length);
            }
            m_ui.a_plot->rescaleAxes(true);
            m_ui.a_plot->replot();
        }

        {
            auto const& sample = m_comms->get_compass_sample();
            add_plot_sample(*m_ui.c_plot, m_plot_x_value, sample.value);
            if (m_plot_x_value > q::Seconds(graph_length).count())
            {
                remove_plot_data_before(*m_ui.c_plot, graph_length);
            }
            m_ui.c_plot->rescaleAxes(true);
            m_ui.c_plot->replot();
        }

        {
            auto const& sample = m_comms->get_barometer_sample();
            add_plot_sample(*m_ui.barometer_plot, m_plot_x_value, sample.value);
            if (m_plot_x_value > q::Seconds(graph_length).count())
            {
                remove_plot_data_before(*m_ui.barometer_plot, graph_length);
            }
            m_ui.barometer_plot->rescaleAxes(true);
            m_ui.barometer_plot->replot();
        }

        {
            auto const& sample = m_comms->get_thermometer_sample();
            add_plot_sample(*m_ui.thermometer_plot, m_plot_x_value, sample.value);
            if (m_plot_x_value > q::Seconds(graph_length).count())
            {
                remove_plot_data_before(*m_ui.thermometer_plot, graph_length);
            }
            m_ui.thermometer_plot->rescaleAxes(true);
            m_ui.thermometer_plot->replot();
        }

        {
            auto const& sample = m_comms->get_sonar_sample();
            add_plot_sample(*m_ui.sonar_plot, m_plot_x_value, sample.value);
            if (m_plot_x_value > q::Seconds(graph_length).count())
            {
                remove_plot_data_before(*m_ui.sonar_plot, graph_length);
            }
            m_ui.sonar_plot->rescaleAxes(true);
            m_ui.sonar_plot->replot();
        }
	}
	else
	{
        m_plot_x_value = 0;

        clear_plot(*m_ui.g_plot);
        clear_plot(*m_ui.a_plot);
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
    m_calibration.message_box.setStandardButtons(QMessageBox::Button::Ok);
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
            auto const& sample = m_comms->get_accelerometer_sample();
            if (m_calibration.last_sample_idx != sample.sample_idx && time_passed > std::chrono::seconds(2))
            {
                m_calibration.last_sample_idx = sample.sample_idx;
                samples.push_back(sample.value);
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
    m_calibration.message_box.setStandardButtons(QMessageBox::Button::Ok);
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
            auto const& sample = m_comms->get_gyroscope_sample();
            if (m_calibration.last_sample_idx != sample.sample_idx && time_passed > std::chrono::seconds(2))
            {
                m_calibration.last_sample_idx = sample.sample_idx;
                samples.push_back(sample.value);
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
    m_calibration.message_box.setStandardButtons(QMessageBox::Button::Ok);
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
            auto const& sample = m_comms->get_compass_sample();
            if (m_calibration.last_sample_idx != sample.sample_idx && time_passed > std::chrono::seconds(2))
            {
                m_calibration.last_sample_idx = sample.sample_idx;
                samples.push_back(sample.value);
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
void Sensors::add_plot_sample(QCustomPlot& plot, float key, float sample)
{
    plot.graph(0)->addData(key, sample);
}
