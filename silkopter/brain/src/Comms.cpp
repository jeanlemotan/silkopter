#include "BrainStdAfx.h"
#include "Comms.h"

using namespace silk;
using namespace boost::asio;

Comms::Comms(boost::asio::io_service& io_service, HAL& hal, UAV& uav)
    : m_io_service(io_service)
    , m_hal(hal)
    , m_uav(uav)
{
    m_ping_time = q::Clock::now();
}

auto Comms::start_listening(uint16_t port) -> Result
{
    for (int tries = 10; tries >= 0; tries--)
    {
        try
        {
            disconnect();
            m_is_listening = false;

            m_channel.reset();

            m_port = port;
            m_socket = std::make_unique<ip::tcp::socket>(m_io_service);

            m_acceptor = std::make_unique<ip::tcp::acceptor>(m_io_service, ip::tcp::endpoint(ip::tcp::v4(), port));
            m_acceptor->set_option(ip::tcp::acceptor::reuse_address(true));
            m_acceptor->set_option(ip::tcp::no_delay(true));

            m_acceptor->async_accept(*m_socket,
                                     boost::bind(&Comms::handle_accept, this, boost::asio::placeholders::error));
        }
        catch(std::exception e)
        {
            SILK_WARNING("Cannot start listening on port {}: {}", port, e.what());
            if (tries == 0)
            {
                return Result::FAILED;
            }
        }
    }

    m_is_listening = true;
    SILK_INFO("Started listening on port {}", port);
    return Result::OK;
}
void Comms::handle_accept(boost::system::error_code const& error)
{
    if (error)
    {
        SILK_WARNING("Error occured while accepting connection: {}", error.message());
        return;
    }

    m_socket->set_option(socket_base::send_buffer_size(8192));

    m_channel.reset(new Channel(*m_socket));
    m_channel->start();

    m_is_listening = false;

    SILK_INFO("Connected to {}:{}", m_socket->remote_endpoint().address().to_string(), m_port);
}

void Comms::disconnect()
{
    m_acceptor.reset();
    m_socket.reset();
}

auto Comms::is_listening() const -> bool
{
    return m_is_listening;
}
auto Comms::is_connected() const -> bool
{
    return m_socket && m_socket->is_open() && !is_listening() && m_channel;
}

auto Comms::get_remote_address() const -> boost::asio::ip::address
{
    return m_socket->remote_endpoint().address();
}

auto Comms::get_error_count() const -> size_t
{
    return m_error_count;
}

void Comms::message_camera_input()
{
    using namespace camera_input;

    auto res = m_channel->begin_unpack();
    QASSERT(res == Channel::Unpack_Result::OK);
    if (res != Channel::Unpack_Result::OK)
    {
        m_channel->end_unpack();
        return;
    }

    Input q;
    res = m_channel->unpack_param(q);
    QASSERT(res == Channel::Unpack_Result::OK);
    if (res != Channel::Unpack_Result::OK)
    {
        m_channel->end_unpack();
        return;
    }

    if (m_hal.camera)
    {    
        switch (q)
        {
        case Input::START_RECORDING:
            m_hal.camera->start_recording();
            break;
        case Input::STOP_RECORDING:
            m_hal.camera->stop_recording();
            break;
        case Input::ISO:
            {
                Iso iso;
                if (m_channel->unpack_param(iso) == Channel::Unpack_Result::OK)
                {
                    m_hal.camera->set_iso(iso);
                }
            }
            break;
        case Input::SHUTTER_SPEED:
            {
                Shutter_Speed ss;
                if (m_channel->unpack_param(ss) == Channel::Unpack_Result::OK)
                {
                    m_hal.camera->set_shutter_speed(ss);
                }
            }
            break;
        case Input::STREAM_QUALITY:
            {
                Stream_Quality sq;
                if (m_channel->unpack_param(sq) == Channel::Unpack_Result::OK)
                {
                    m_hal.camera->set_quality(sq);
                }
            }
            break;
        default:
            SILK_INFO("BAD Camera input received {}", static_cast<int>(q));
            break;
        };
    }
    
    m_channel->end_unpack();

    SILK_INFO("Camera input received");
}
void Comms::message_uav_input()
{
    using namespace uav_input;

    auto res = m_channel->begin_unpack();
    QASSERT(res == Channel::Unpack_Result::OK);
    if (res != Channel::Unpack_Result::OK)
    {
        m_channel->end_unpack();
        return;
    }

    Input q;
    res = m_channel->unpack_param(q);
    QASSERT(res == Channel::Unpack_Result::OK);
    if (res != Channel::Unpack_Result::OK)
    {
        m_channel->end_unpack();
        return;
    }
    switch(q)
    {
    case Input::THROTTLE_MODE:
        {
            Throttle_Mode v;
            if (m_channel->unpack_param(v) == Channel::Unpack_Result::OK)
            {
                m_uav.set_throttle_mode(v);
            }
        }
        break;
    case Input::PITCH_ROLL_MODE:
        {
            Pitch_Roll_Mode v;
            if (m_channel->unpack_param(v) == Channel::Unpack_Result::OK)
            {
                m_uav.set_pitch_roll_mode(v);
            }
        }
        break;
    case Input::YAW_MODE:
        {
            Yaw_Mode v;
            if (m_channel->unpack_param(v) == Channel::Unpack_Result::OK)
            {
                m_uav.set_yaw_mode(v);
            }
        }
        break;
    case Input::STICKS:
        {
            Sticks v;
            if (m_channel->unpack_param(v) == Channel::Unpack_Result::OK)
            {
                m_uav.set_sticks(v);
            }
        }
        break;
    case Input::ASSISTS:
        {
            Assists v;
            if (m_channel->unpack_param(v) == Channel::Unpack_Result::OK)
            {
                m_uav.set_assists(v);
            }
        }
        break;
    case Input::ARM:
        {
            m_uav.arm();
        }
        break;
    case Input::DISARM:
        {
            m_uav.disarm();
        }
        break;
    default:
        SILK_INFO("BAD UAV input received {}", static_cast<int>(q));
        break;
    }

    m_channel->end_unpack();

    SILK_INFO("UAV input received");
}

void Comms::message_yaw_rate_pid_params()
{
    auto params = m_uav.get_yaw_rate_pid_params();
    if (m_channel->get_message_size() == 0)
    {
        SILK_INFO("yaw rate pid params get");
        m_channel->send(Message::YAW_RATE_PID_PARAMS, params);
    }
    else
    {
        auto result = m_channel->unpack(params);
        if (result == Channel::Unpack_Result::OK)
        {
            SILK_INFO("yaw rate pid params changed");
            m_uav.set_yaw_rate_pid_params(params);
        }
        else
        {
            SILK_WARNING("Failed to receive yaw rate pid params");
        }
    }
}
void Comms::message_pitch_rate_pid_params()
{
    auto params = m_uav.get_pitch_rate_pid_params();
    if (m_channel->get_message_size() == 0)
    {
        SILK_INFO("pitch rate pid params get");
        m_channel->send(Message::PITCH_RATE_PID_PARAMS, params);
    }
    else
    {
        auto result = m_channel->unpack(params);
        if (result == Channel::Unpack_Result::OK)
        {
            SILK_INFO("pitch rate pid params changed");
            m_uav.set_pitch_rate_pid_params(params);
        }
        else
        {
            SILK_WARNING("Failed to receive pitch rate pid params");
        }
    }
}
void Comms::message_roll_rate_pid_params()
{
    auto params = m_uav.get_roll_rate_pid_params();
    if (m_channel->get_message_size() == 0)
    {
        SILK_INFO("roll rate pid params get");
        m_channel->send(Message::ROLL_RATE_PID_PARAMS, params);
    }
    else
    {
        auto result = m_channel->unpack(params);
        if (result == Channel::Unpack_Result::OK)
        {
            SILK_INFO("roll rate pid params changed");
            m_uav.set_roll_rate_pid_params(params);
        }
        else
        {
            SILK_WARNING("Failed to receive roll rate pid params");
        }
    }
}
void Comms::message_altitude_rate_pid_params()
{
    auto params = m_uav.get_altitude_rate_pid_params();
    if (m_channel->get_message_size() == 0)
    {
        SILK_INFO("altitude rate pid params get");
        m_channel->send(Message::ALTITUDE_RATE_PID_PARAMS, params);
    }
    else
    {
        auto result = m_channel->unpack(params);
        if (result == Channel::Unpack_Result::OK)
        {
            SILK_INFO("altitude rate pid params changed");
            m_uav.set_altitude_rate_pid_params(params);
        }
        else
        {
            SILK_WARNING("Failed to receive altitude rate pid params");
        }
    }
}
void Comms::message_yaw_pid_params()
{
    auto params = m_uav.get_yaw_pid_params();
    if (m_channel->get_message_size() == 0)
    {
        SILK_INFO("yaw pid params get");
        m_channel->send(Message::YAW_PID_PARAMS, params);
    }
    else
    {
        auto result = m_channel->unpack(params);
        if (result == Channel::Unpack_Result::OK)
        {
            SILK_INFO("yaw pid params changed");
            m_uav.set_yaw_pid_params(params);
        }
        else
        {
            SILK_WARNING("Failed to receive yaw pid params");
        }
    }
}
void Comms::message_pitch_pid_params()
{
    auto params = m_uav.get_pitch_pid_params();
    if (m_channel->get_message_size() == 0)
    {
        SILK_INFO("pitch pid params get");
        m_channel->send(Message::PITCH_PID_PARAMS, params);
    }
    else
    {
        auto result = m_channel->unpack(params);
        if (result == Channel::Unpack_Result::OK)
        {
            SILK_INFO("pitch pid params changed");
            m_uav.set_pitch_pid_params(params);
        }
        else
        {
            SILK_WARNING("Failed to receive pitch pid params");
        }
    }
}
void Comms::message_roll_pid_params()
{
    auto params = m_uav.get_roll_pid_params();
    if (m_channel->get_message_size() == 0)
    {
        SILK_INFO("roll pid params get");
        m_channel->send(Message::ROLL_PID_PARAMS, params);
    }
    else
    {
        auto result = m_channel->unpack(params);
        if (result == Channel::Unpack_Result::OK)
        {
            SILK_INFO("roll pid params changed");
            m_uav.set_roll_pid_params(params);
        }
        else
        {
            SILK_WARNING("Failed to receive roll pid params");
        }
    }
}
void Comms::message_altitude_pid_params()
{
    auto params = m_uav.get_altitude_pid_params();
    if (m_channel->get_message_size() == 0)
    {
        SILK_INFO("altitude pid params get");
        m_channel->send(Message::ALTITUDE_PID_PARAMS, params);
    }
    else
    {
        auto result = m_channel->unpack(params);
        if (result == Channel::Unpack_Result::OK)
        {
            SILK_INFO("altitude pid params changed");
            m_uav.set_altitude_pid_params(params);
        }
        else
        {
            SILK_WARNING("Failed to receive altitude pid params");
        }
    }
}

void Comms::message_assist_params()
{
    auto params = m_uav.get_assist_params();
    if (m_channel->get_message_size() == 0)
    {
        SILK_INFO("assist params get");
        m_channel->send(Message::ASSIST_PARAMS, params);
    }
    else
    {
        auto result = m_channel->unpack(params);
        if (result == Channel::Unpack_Result::OK)
        {
            SILK_INFO("assist params changed");
            m_uav.set_assist_params(params);
        }
        else
        {
            SILK_WARNING("Failed to receive assist params");
        }
    }
}

void Comms::message_calibration_accelerometer()
{
    QASSERT(m_hal.sensors);
    math::vec3f bias, scale;
    if (m_channel->get_message_size() == 0)
    {
        SILK_INFO("Accelerometer calibration set req");
        m_hal.sensors->get_accelerometer_calibration_data(bias, scale);
        m_channel->send(Message::CALIBRATION_ACCELEROMETER, bias, scale);
    }
    else
    {
        auto result = m_channel->unpack(bias, scale);
        if (result == Channel::Unpack_Result::OK)
        {
            SILK_INFO("Accelerometer calibration changed");
            m_hal.sensors->set_accelerometer_calibration_data(bias, scale);
            m_channel->send(Message::CALIBRATION_ACCELEROMETER, bias, scale);
        }
        else
        {
            SILK_WARNING("Failed to receive accelerometer calibration");
        }
    }
}
void Comms::message_calibration_gyroscope()
{
    QASSERT(m_hal.sensors);
    math::vec3f bias;
    if (m_channel->get_message_size() == 0)
    {
        SILK_INFO("Gyroscope calibration set req");
        m_hal.sensors->get_gyroscope_calibration_data(bias);
        m_channel->send(Message::CALIBRATION_GYROSCOPE, bias);
    }
    else
    {
        auto result = m_channel->unpack(bias);
        if (result == Channel::Unpack_Result::OK)
        {
            SILK_INFO("Gyroscope calibration changed");
            m_hal.sensors->set_gyroscope_calibration_data(bias);
            m_channel->send(Message::CALIBRATION_GYROSCOPE, bias);
        }
        else
        {
            SILK_WARNING("Failed to receive gyroscope calibration");
        }
    }
}
void Comms::message_calibration_compass()
{
    math::vec3f bias;
    if (m_channel->get_message_size() == 0)
    {
        SILK_INFO("Compass calibration req, sending to GS");
        m_hal.sensors->get_compass_calibration_data(bias);
        m_channel->send(Message::CALIBRATION_COMPASS, bias);
    }
    else
    {
        auto result = m_channel->unpack(bias);
        if (result == Channel::Unpack_Result::OK)
        {
            SILK_INFO("Compass calibration changed");
            m_hal.sensors->set_compass_calibration_data(bias);
            m_channel->send(Message::CALIBRATION_COMPASS, bias);
        }
        else
        {
            SILK_WARNING("Failed to receive compass calibration");
        }
    }
}

void Comms::send_sensor_data()
{
    QASSERT(m_hal.sensors);
    auto now = q::Clock::now();
    auto delay = std::chrono::milliseconds(50);

    {
        auto const& samples = m_hal.sensors->get_gyroscope_samples();
        std::copy(samples.begin(), samples.end(), std::back_inserter(m_sensors_samples.gyroscope));
    }
    {
        auto const& samples = m_hal.sensors->get_accelerometer_samples();
        std::copy(samples.begin(), samples.end(), std::back_inserter(m_sensors_samples.accelerometer));
    }
    {
        auto const& samples = m_hal.sensors->get_compass_samples();
        std::copy(samples.begin(), samples.end(), std::back_inserter(m_sensors_samples.compass));
    }
    {
        auto const& samples = m_hal.sensors->get_barometer_samples();
        std::copy(samples.begin(), samples.end(), std::back_inserter(m_sensors_samples.barometer));
    }
    {
        auto const& samples = m_hal.sensors->get_thermometer_samples();
        std::copy(samples.begin(), samples.end(), std::back_inserter(m_sensors_samples.thermometer));
    }
    {
        auto const& samples = m_hal.sensors->get_sonar_samples();
        std::copy(samples.begin(), samples.end(), std::back_inserter(m_sensors_samples.sonar));
    }
    {
        auto const& samples = m_hal.sensors->get_voltage_samples();
        std::copy(samples.begin(), samples.end(), std::back_inserter(m_sensors_samples.voltage));
    }
    {
        auto const& samples = m_hal.sensors->get_current_samples();
        std::copy(samples.begin(), samples.end(), std::back_inserter(m_sensors_samples.current));
    }
    {
        auto const& samples = m_hal.sensors->get_gps_samples();
        std::copy(samples.begin(), samples.end(), std::back_inserter(m_sensors_samples.gps));
    }

    //SILK_INFO("acc: {}", m_sensors_samples.accelerometer.size());
    if (now - m_sensors_samples.last_sent_timestamp < delay)
    {
        return;
    }

    m_channel->begin_stream();

    {
        m_channel->add_to_stream(static_cast<uint16_t>(m_sensors_samples.gyroscope.size()));
        for (auto& a: m_sensors_samples.gyroscope)
        {
            m_channel->add_to_stream(a.value);
            m_channel->add_to_stream(static_cast<uint16_t>(std::chrono::duration_cast<std::chrono::microseconds>(a.dt).count()));
        }
        m_sensors_samples.gyroscope.clear();
    }
    {
        m_channel->add_to_stream(static_cast<uint16_t>(m_sensors_samples.accelerometer.size()));
        for (auto& a: m_sensors_samples.accelerometer)
        {
            m_channel->add_to_stream(a.value);
            m_channel->add_to_stream(static_cast<uint16_t>(std::chrono::duration_cast<std::chrono::microseconds>(a.dt).count()));
        }
        m_sensors_samples.accelerometer.clear();
    }
    {
        m_channel->add_to_stream(static_cast<uint16_t>(m_sensors_samples.compass.size()));
        for (auto& a: m_sensors_samples.compass)
        {
            m_channel->add_to_stream(a.value);
            m_channel->add_to_stream(static_cast<uint16_t>(std::chrono::duration_cast<std::chrono::microseconds>(a.dt).count()));
        }
        m_sensors_samples.compass.clear();
    }
    {
        m_channel->add_to_stream(static_cast<uint16_t>(m_sensors_samples.barometer.size()));
        for (auto& a: m_sensors_samples.barometer)
        {
            m_channel->add_to_stream(a.value);
            m_channel->add_to_stream(static_cast<uint16_t>(std::chrono::duration_cast<std::chrono::microseconds>(a.dt).count()));
        }
        m_sensors_samples.barometer.clear();
    }
    {
        m_channel->add_to_stream(static_cast<uint16_t>(m_sensors_samples.thermometer.size()));
        for (auto& a: m_sensors_samples.thermometer)
        {
            m_channel->add_to_stream(a.value);
            m_channel->add_to_stream(static_cast<uint16_t>(std::chrono::duration_cast<std::chrono::microseconds>(a.dt).count()));
        }
        m_sensors_samples.thermometer.clear();
    }
    {
        m_channel->add_to_stream(static_cast<uint16_t>(m_sensors_samples.sonar.size()));
        for (auto& a: m_sensors_samples.sonar)
        {
            m_channel->add_to_stream(a.value);
            m_channel->add_to_stream(static_cast<uint16_t>(std::chrono::duration_cast<std::chrono::microseconds>(a.dt).count()));
        }
        m_sensors_samples.sonar.clear();
    }
    {
        m_channel->add_to_stream(static_cast<uint16_t>(m_sensors_samples.voltage.size()));
        for (auto& a: m_sensors_samples.voltage)
        {
            m_channel->add_to_stream(a.value);
            m_channel->add_to_stream(static_cast<uint16_t>(std::chrono::duration_cast<std::chrono::microseconds>(a.dt).count()));
        }
        m_sensors_samples.voltage.clear();
    }
    {
        m_channel->add_to_stream(static_cast<uint16_t>(m_sensors_samples.current.size()));
        for (auto& a: m_sensors_samples.current)
        {
            m_channel->add_to_stream(a.value);
            m_channel->add_to_stream(static_cast<uint16_t>(std::chrono::duration_cast<std::chrono::microseconds>(a.dt).count()));
        }
        m_sensors_samples.current.clear();
    }
    {
        m_channel->add_to_stream(static_cast<uint16_t>(m_sensors_samples.gps.size()));
        for (auto& a: m_sensors_samples.gps)
        {
            m_channel->add_to_stream(a.value);
            m_channel->add_to_stream(static_cast<uint16_t>(std::chrono::duration_cast<std::chrono::microseconds>(a.dt).count()));
        }
        m_sensors_samples.gps.clear();
    }

    m_channel->end_stream(Message::SENSORS);

    m_sensors_samples.last_sent_timestamp = now;
}

void Comms::send_uav_data()
{
    auto now = q::Clock::now();
    auto delay = std::chrono::milliseconds(100);

    if (now - m_uav_sent_timestamp < delay)
    {
        return;
    }
    m_uav_sent_timestamp = now;

    m_channel->send(Message::UAV_ROTATION, m_uav.get_ahrs().get_b2e_quat());
    m_channel->send(Message::UAV_LINEAR_ACCELERATION, m_uav.get_linear_acceleration());
    m_channel->send(Message::UAV_VELOCITY, m_uav.get_velocity());
    m_channel->send(Message::UAV_POSITION, m_uav.get_position());
}

void Comms::process()
{
    if (!is_connected())
    {
        if (m_port && !is_listening())
        {
            start_listening(m_port);
        }
        return;
    }
    QASSERT(m_channel);

    if (auto msg = m_channel->get_next_message())
    {
        switch (msg.get())
        {
            case Message::CAMERA_INPUT: message_camera_input(); break;
            case Message::UAV_INPUT: message_uav_input(); break;

            case Message::CALIBRATION_ACCELEROMETER: message_calibration_accelerometer(); break;
            case Message::CALIBRATION_GYROSCOPE: message_calibration_gyroscope(); break;
            case Message::CALIBRATION_COMPASS: message_calibration_compass(); break;

            case Message::YAW_RATE_PID_PARAMS: message_yaw_rate_pid_params(); break;
            case Message::PITCH_RATE_PID_PARAMS: message_pitch_rate_pid_params(); break;
            case Message::ROLL_RATE_PID_PARAMS: message_roll_rate_pid_params(); break;
            case Message::ALTITUDE_RATE_PID_PARAMS: message_altitude_rate_pid_params(); break;
            case Message::YAW_PID_PARAMS: message_yaw_pid_params(); break;
            case Message::PITCH_PID_PARAMS: message_pitch_pid_params(); break;
            case Message::ROLL_PID_PARAMS: message_roll_pid_params(); break;
            case Message::ALTITUDE_PID_PARAMS: message_altitude_pid_params(); break;

            case Message::ASSIST_PARAMS: message_assist_params(); break;

            default:
                SILK_WARNING("Received unhandled message: {}", static_cast<int>(msg.get()));
                m_error_count++;
            break;
        }
    }

    send_sensor_data();
    send_uav_data();

    {
        auto now = q::Clock::now();
        auto d = now - m_ping_time;
        if (d >= std::chrono::milliseconds(20))
        {
            m_ping_time = now;
            m_alive_time_ms += std::chrono::duration_cast<std::chrono::milliseconds>(d).count();
            m_channel->send(Message::UAV_PING, m_alive_time_ms);
        }
    }
}


