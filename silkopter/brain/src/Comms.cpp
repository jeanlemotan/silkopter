#include "BrainStdAfx.h"
#include "Comms.h"

using namespace silk;
using namespace boost::asio;

Comms::Comms(boost::asio::io_service& io_service, HAL& hal, UAV& uav)
    : m_io_service(io_service)
    , m_hal(hal)
    , m_uav(uav)
{
    m_ping.last_time_point = q::Clock::now();

    m_raw_sensor_samples.gyroscope.set_capacity(Raw_Sensor_Samples::MAX_SIZE);
    m_raw_sensor_samples.accelerometer.set_capacity(Raw_Sensor_Samples::MAX_SIZE);
    m_raw_sensor_samples.compass.set_capacity(Raw_Sensor_Samples::MAX_SIZE);
    m_raw_sensor_samples.barometer.set_capacity(Raw_Sensor_Samples::MAX_SIZE);
    m_raw_sensor_samples.thermometer.set_capacity(Raw_Sensor_Samples::MAX_SIZE);
    m_raw_sensor_samples.sonar.set_capacity(Raw_Sensor_Samples::MAX_SIZE);
    m_raw_sensor_samples.voltage.set_capacity(Raw_Sensor_Samples::MAX_SIZE);
    m_raw_sensor_samples.current.set_capacity(Raw_Sensor_Samples::MAX_SIZE);
    m_raw_sensor_samples.gps.set_capacity(Raw_Sensor_Samples::MAX_SIZE);
}

auto Comms::start_listening(uint16_t port) -> bool
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

            break;
        }
        catch(std::exception e)
        {
            SILK_WARNING("Cannot start listening on port {}: {}", port, e.what());
            if (tries == 0)
            {
                return false;
            }
        }
    }

    m_is_listening = true;
    SILK_INFO("Started listening on port {}", port);
    return true;
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

auto Comms::get_remote_clock() const -> Remote_Clock const&
{
    return m_remote_clock;
}

void Comms::process_message_ping()
{
    uint64_t remote_now = 0;
    uint32_t seq = 0;
    if (!m_channel->unpack(remote_now, seq))
    {
        SILK_WARNING("Failed to unpack ping");
        return;
    }

    m_channel->send(detail::Comm_Message::PONG, static_cast<uint64_t>(q::Clock::now().time_since_epoch().count()), seq);
    m_channel->flush();

    if (m_remote_clock.now().time_since_epoch().count() < remote_now)
    {
        SILK_WARNING("Setting remote time in the past!!!");
    }
    m_remote_clock.set_now(Remote_Clock::duration(remote_now));
    //SILK_INFO("Remote Clock set to {}", m_remote_clock.now());
}
void Comms::process_message_pong()
{
    uint64_t remote_now = 0;
    uint32_t seq = 0;
    if (!m_channel->unpack(remote_now, seq))
    {
        SILK_WARNING("Failed to unpack pong");
        return;
    }
    if (m_remote_clock.now().time_since_epoch().count() < remote_now)
    {
        SILK_WARNING("Setting remote time in the past!!!");
    }
    m_remote_clock.set_now(Remote_Clock::duration(remote_now));
    //SILK_INFO("Remote Clock set to {}", m_remote_clock.now());

    auto it = m_ping.seq_sent.find(seq);
    if (it != m_ping.seq_sent.end())
    {
        auto rtt = q::Clock::now() - it->second;
        m_ping.rtts.push_back(rtt);
        while (m_ping.rtts.size() > 1024)
        {
            m_ping.rtts.pop_front();
        }
        m_ping.seq_sent.erase(it);
    }
    else
    {
        SILK_WARNING("invalid ping seq received: {}", seq);
    }
}


void Comms::process_message_camera_input()
{
    using namespace camera_input;

    if (!m_channel->begin_unpack())
    {
        SILK_ERR("Cannot unpack camera input message");
        m_channel->end_unpack();
        return;
    }

    Input q;
    if (!m_channel->unpack_param(q))
    {
        SILK_ERR("Cannot unpack camera input message");
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
                if (m_channel->unpack_param(iso))
                {
                    m_hal.camera->set_iso(iso);
                }
            }
            break;
        case Input::SHUTTER_SPEED:
            {
                Shutter_Speed ss;
                if (m_channel->unpack_param(ss))
                {
                    m_hal.camera->set_shutter_speed(ss);
                }
            }
            break;
        case Input::STREAM_QUALITY:
            {
                Stream_Quality sq;
                if (m_channel->unpack_param(sq))
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
void Comms::process_message_uav_input()
{
    using namespace uav_input;

    if (!m_channel->begin_unpack())
    {
        SILK_ERR("Cannot unpack uav input message");
        m_channel->end_unpack();
        return;
    }

    Input q;
    if (!m_channel->unpack_param(q))
    {
        SILK_ERR("Cannot unpack uav input message");
        m_channel->end_unpack();
        return;
    }
    switch(q)
    {
    case Input::THROTTLE_MODE:
        {
            Throttle_Mode v;
            if (m_channel->unpack_param(v))
            {
                m_uav.set_throttle_mode(v);
            }
        }
        break;
    case Input::PITCH_ROLL_MODE:
        {
            Pitch_Roll_Mode v;
            if (m_channel->unpack_param(v))
            {
                m_uav.set_pitch_roll_mode(v);
            }
        }
        break;
    case Input::YAW_MODE:
        {
            Yaw_Mode v;
            if (m_channel->unpack_param(v))
            {
                m_uav.set_yaw_mode(v);
            }
        }
        break;
    case Input::STICKS:
        {
            Sticks v;
            if (m_channel->unpack_param(v))
            {
                m_uav.set_sticks(v);
            }
        }
        break;
    case Input::ASSISTS:
        {
            Assists v;
            if (m_channel->unpack_param(v))
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

void Comms::process_message_yaw_rate_pid_params()
{
    auto params = m_uav.get_yaw_rate_pid_params();
    if (m_channel->get_message_size() == 0)
    {
        SILK_INFO("yaw rate pid params get");
        m_channel->send(detail::Comm_Message::YAW_RATE_PID_PARAMS, params);
    }
    else
    {
        if (m_channel->unpack(params))
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
void Comms::process_message_pitch_rate_pid_params()
{
    auto params = m_uav.get_pitch_rate_pid_params();
    if (m_channel->get_message_size() == 0)
    {
        SILK_INFO("pitch rate pid params get");
        m_channel->send(detail::Comm_Message::PITCH_RATE_PID_PARAMS, params);
    }
    else
    {
        if (m_channel->unpack(params))
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
void Comms::process_message_roll_rate_pid_params()
{
    auto params = m_uav.get_roll_rate_pid_params();
    if (m_channel->get_message_size() == 0)
    {
        SILK_INFO("roll rate pid params get");
        m_channel->send(detail::Comm_Message::ROLL_RATE_PID_PARAMS, params);
    }
    else
    {
        if (m_channel->unpack(params))
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
void Comms::process_message_altitude_rate_pid_params()
{
    auto params = m_uav.get_altitude_rate_pid_params();
    if (m_channel->get_message_size() == 0)
    {
        SILK_INFO("altitude rate pid params get");
        m_channel->send(detail::Comm_Message::ALTITUDE_RATE_PID_PARAMS, params);
    }
    else
    {
        if (m_channel->unpack(params))
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
void Comms::process_message_yaw_pid_params()
{
    auto params = m_uav.get_yaw_pid_params();
    if (m_channel->get_message_size() == 0)
    {
        SILK_INFO("yaw pid params get");
        m_channel->send(detail::Comm_Message::YAW_PID_PARAMS, params);
    }
    else
    {
        if (m_channel->unpack(params))
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
void Comms::process_message_pitch_pid_params()
{
    auto params = m_uav.get_pitch_pid_params();
    if (m_channel->get_message_size() == 0)
    {
        SILK_INFO("pitch pid params get");
        m_channel->send(detail::Comm_Message::PITCH_PID_PARAMS, params);
    }
    else
    {
        if (m_channel->unpack(params))
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
void Comms::process_message_roll_pid_params()
{
    auto params = m_uav.get_roll_pid_params();
    if (m_channel->get_message_size() == 0)
    {
        SILK_INFO("roll pid params get");
        m_channel->send(detail::Comm_Message::ROLL_PID_PARAMS, params);
    }
    else
    {
        if (m_channel->unpack(params))
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
void Comms::process_message_altitude_pid_params()
{
    auto params = m_uav.get_altitude_pid_params();
    if (m_channel->get_message_size() == 0)
    {
        SILK_INFO("altitude pid params get");
        m_channel->send(detail::Comm_Message::ALTITUDE_PID_PARAMS, params);
    }
    else
    {
        if (m_channel->unpack(params))
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

void Comms::process_message_assist_params()
{
    auto params = m_uav.get_assist_params();
    if (m_channel->get_message_size() == 0)
    {
        SILK_INFO("assist params get");
        m_channel->send(detail::Comm_Message::ASSIST_PARAMS, params);
    }
    else
    {
        if (m_channel->unpack(params))
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

void Comms::process_message_raw_sensors()
{
    QASSERT(m_hal.sensors);
    detail::Comm_Message_Sensor sensors;
    if (!m_channel->unpack(sensors))
    {
        SILK_INFO("Failed to unpack raw sensors request");
        return;
    }

    send_raw_sensor_data(sensors);
}


void Comms::process_message_calibration_accelerometer()
{
    QASSERT(m_hal.sensors);
    math::vec3f bias, scale;
    if (m_channel->get_message_size() == 0)
    {
        SILK_INFO("Accelerometer calibration set req");
        m_hal.sensors->get_accelerometer_calibration_data(bias, scale);
        m_channel->send(detail::Comm_Message::CALIBRATION_ACCELEROMETER, bias, scale);
    }
    else
    {
        if (m_channel->unpack(bias, scale))
        {
            SILK_INFO("Accelerometer calibration changed");
            m_hal.sensors->set_accelerometer_calibration_data(bias, scale);
            m_channel->send(detail::Comm_Message::CALIBRATION_ACCELEROMETER, bias, scale);
        }
        else
        {
            SILK_WARNING("Failed to receive accelerometer calibration");
        }
    }
}
void Comms::process_message_calibration_gyroscope()
{
    QASSERT(m_hal.sensors);
    math::vec3f bias;
    if (m_channel->get_message_size() == 0)
    {
        SILK_INFO("Gyroscope calibration set req");
        m_hal.sensors->get_gyroscope_calibration_data(bias);
        m_channel->send(detail::Comm_Message::CALIBRATION_GYROSCOPE, bias);
    }
    else
    {
        if (m_channel->unpack(bias))
        {
            SILK_INFO("Gyroscope calibration changed");
            m_hal.sensors->set_gyroscope_calibration_data(bias);
            m_channel->send(detail::Comm_Message::CALIBRATION_GYROSCOPE, bias);
        }
        else
        {
            SILK_WARNING("Failed to receive gyroscope calibration");
        }
    }
}
void Comms::process_message_calibration_compass()
{
    math::vec3f bias;
    if (m_channel->get_message_size() == 0)
    {
        SILK_INFO("Compass calibration req, sending to GS");
        m_hal.sensors->get_compass_calibration_data(bias);
        m_channel->send(detail::Comm_Message::CALIBRATION_COMPASS, bias);
    }
    else
    {
        if (m_channel->unpack(bias))
        {
            SILK_INFO("Compass calibration changed");
            m_hal.sensors->set_compass_calibration_data(bias);
            m_channel->send(detail::Comm_Message::CALIBRATION_COMPASS, bias);
        }
        else
        {
            SILK_WARNING("Failed to receive compass calibration");
        }
    }
}

void Comms::send_raw_sensor_data(detail::Comm_Message_Sensors sensors)
{
    QASSERT(m_hal.sensors);
    if (!sensors.any())
    {
        return;
    }

    m_channel->begin_pack();

    m_channel->pack_param(sensors);

    if (sensors.test(detail::Comm_Message_Sensor::GYROSCOPE))
    {
        m_channel->pack_param(static_cast<uint32_t>(m_raw_sensor_samples.gyroscope.size()));
        for (auto& a: m_raw_sensor_samples.gyroscope)
        {
            m_channel->pack_param(a);
        }
        m_raw_sensor_samples.gyroscope.clear();
    }
    if (sensors.test(detail::Comm_Message_Sensor::ACCELEROMETER))
    {
        m_channel->pack_param(static_cast<uint32_t>(m_raw_sensor_samples.accelerometer.size()));
        for (auto& a: m_raw_sensor_samples.accelerometer)
        {
            m_channel->pack_param(a.value);
        }
        m_raw_sensor_samples.accelerometer.clear();
    }
    if (sensors.test(detail::Comm_Message_Sensor::COMPASS))
    {
        m_channel->pack_param(static_cast<uint32_t>(m_raw_sensor_samples.compass.size()));
        for (auto& a: m_raw_sensor_samples.compass)
        {
            m_channel->pack_param(a.value);
        }
        m_raw_sensor_samples.compass.clear();
    }
    if (sensors.test(detail::Comm_Message_Sensor::BAROMETER))
    {
        m_channel->pack_param(static_cast<uint32_t>(m_raw_sensor_samples.barometer.size()));
        for (auto& a: m_raw_sensor_samples.barometer)
        {
            m_channel->pack_param(a.value);
        }
        m_raw_sensor_samples.barometer.clear();
    }
    if (sensors.test(detail::Comm_Message_Sensor::THERMOMETER))
    {
        m_channel->pack_param(static_cast<uint32_t>(m_raw_sensor_samples.thermometer.size()));
        for (auto& a: m_raw_sensor_samples.thermometer)
        {
            m_channel->pack_param(a.value);
        }
        m_raw_sensor_samples.thermometer.clear();
    }
    if (sensors.test(detail::Comm_Message_Sensor::SONAR))
    {
        m_channel->pack_param(static_cast<uint32_t>(m_raw_sensor_samples.sonar.size()));
        for (auto& a: m_raw_sensor_samples.sonar)
        {
            m_channel->pack_param(a.value);
        }
        m_raw_sensor_samples.sonar.clear();
    }
    if (sensors.test(detail::Comm_Message_Sensor::VOLTAGE))
    {
        m_channel->pack_param(static_cast<uint32_t>(m_raw_sensor_samples.voltage.size()));
        for (auto& a: m_raw_sensor_samples.voltage)
        {
            m_channel->pack_param(a.value);
        }
        m_raw_sensor_samples.voltage.clear();
    }
    if (sensors.test(detail::Comm_Message_Sensor::CURRENT))
    {
        m_channel->pack_param(static_cast<uint32_t>(m_raw_sensor_samples.current.size()));
        for (auto& a: m_raw_sensor_samples.current)
        {
            m_channel->pack_param(a.value);
        }
        m_raw_sensor_samples.current.clear();
    }
    if (sensors.test(detail::Comm_Message_Sensor::GPS))
    {
        m_channel->pack_param(static_cast<uint32_t>(m_raw_sensor_samples.gps.size()));
        for (auto& a: m_raw_sensor_samples.gps)
        {
            m_channel->pack_param(a.value);
        }
        m_raw_sensor_samples.gps.clear();
    }

    m_channel->end_pack(detail::Comm_Message::SENSORS);
}

void Comms::send_sensor_data()
{
    QASSERT(m_hal.sensors);
    auto now = q::Clock::now();
    auto delay = std::chrono::milliseconds(50);

    //SILK_INFO("acc: {}", m_sensors_samples.accelerometer.size());
    if (now - m_sensor_samples.last_sent_timestamp < delay)
    {
        return;
    }

    detail::Comm_Message_Sensors sensors;

    {
        auto const& samples = m_hal.sensors->get_gyroscope_samples();
        if (!samples.empty() && samples.back().sample_idx != m_sensor_samples.gyroscope.sample_idx)
        {
            sensors.set(detail::Comm_Message_Sensor::GYROSCOPE);
            m_sensor_samples.gyroscope = samples.back();
        }
    }
    {
        auto const& samples = m_hal.sensors->get_accelerometer_samples();
        if (!samples.empty() && samples.back().sample_idx != m_sensor_samples.accelerometer.sample_idx)
        {
            sensors.set(detail::Comm_Message_Sensor::ACCELEROMETER);
            m_sensor_samples.accelerometer = samples.back();
        }
    }
    {
        auto const& samples = m_hal.sensors->get_compass_samples();
        if (!samples.empty() && samples.back().sample_idx != m_sensor_samples.compass.sample_idx)
        {
            sensors.set(detail::Comm_Message_Sensor::COMPASS);
            m_sensor_samples.compass = samples.back();
        }
    }
    {
        auto const& samples = m_hal.sensors->get_barometer_samples();
        if (!samples.empty() && samples.back().sample_idx != m_sensor_samples.barometer.sample_idx)
        {
            sensors.set(detail::Comm_Message_Sensor::BAROMETER);
            m_sensor_samples.barometer = samples.back();
        }
    }
    {
        auto const& samples = m_hal.sensors->get_thermometer_samples();
        if (!samples.empty() && samples.back().sample_idx != m_sensor_samples.thermometer.sample_idx)
        {
            sensors.set(detail::Comm_Message_Sensor::THERMOMETER);
            m_sensor_samples.thermometer = samples.back();
        }
    }
    {
        auto const& samples = m_hal.sensors->get_sonar_samples();
        if (!samples.empty() && samples.back().sample_idx != m_sensor_samples.sonar.sample_idx)
        {
            sensors.set(detail::Comm_Message_Sensor::SONAR);
            m_sensor_samples.sonar = samples.back();
        }
    }
    {
        auto const& samples = m_hal.sensors->get_voltage_samples();
        if (!samples.empty() && samples.back().sample_idx != m_sensor_samples.voltage.sample_idx)
        {
            sensors.set(detail::Comm_Message_Sensor::VOLTAGE);
            m_sensor_samples.voltage = samples.back();
        }
    }
    {
        auto const& samples = m_hal.sensors->get_current_samples();
        if (!samples.empty() && samples.back().sample_idx != m_sensor_samples.current.sample_idx)
        {
            sensors.set(detail::Comm_Message_Sensor::CURRENT);
            m_sensor_samples.current = samples.back();
        }
    }
    {
        auto const& samples = m_hal.sensors->get_gps_samples();
        if (!samples.empty() && samples.back().sample_idx != m_sensor_samples.gps.sample_idx)
        {
            sensors.set(detail::Comm_Message_Sensor::GPS);
            m_sensor_samples.gps = samples.back();
        }
    }

    if (!sensors.any())
    {
        return;
    }

    m_channel->begin_pack();
    m_channel->pack_param(sensors);

    if (sensors.test(detail::Comm_Message_Sensor::GYROSCOPE))
    {
        m_channel->pack_param(m_sensor_samples.gyroscope);
    }
    if (sensors.test(detail::Comm_Message_Sensor::ACCELEROMETER))
    {
        m_channel->pack_param(m_sensor_samples.accelerometer);
    }
    if (sensors.test(detail::Comm_Message_Sensor::COMPASS))
    {
        m_channel->pack_param(m_sensor_samples.compass);
    }
    if (sensors.test(detail::Comm_Message_Sensor::BAROMETER))
    {
        m_channel->pack_param(m_sensor_samples.barometer);
    }
    if (sensors.test(detail::Comm_Message_Sensor::THERMOMETER))
    {
        m_channel->pack_param(m_sensor_samples.thermometer);
    }
    if (sensors.test(detail::Comm_Message_Sensor::SONAR))
    {
        m_channel->pack_param(m_sensor_samples.sonar);
    }
    if (sensors.test(detail::Comm_Message_Sensor::VOLTAGE))
    {
        m_channel->pack_param(m_sensor_samples.voltage);
    }
    if (sensors.test(detail::Comm_Message_Sensor::CURRENT))
    {
        m_channel->pack_param(m_sensor_samples.current);
    }
    if (sensors.test(detail::Comm_Message_Sensor::GPS))
    {
        m_channel->pack_param(m_sensor_samples.gps);
    }

    m_channel->end_pack(detail::Comm_Message::SENSORS);

    m_sensor_samples.last_sent_timestamp = now;
}

void Comms::store_raw_sensor_data()
{
    {
        auto const& samples = m_hal.sensors->get_gyroscope_samples();
        std::copy(samples.begin(), samples.end(), std::back_inserter(m_raw_sensor_samples.gyroscope));
    }
    {
        auto const& samples = m_hal.sensors->get_accelerometer_samples();
        std::copy(samples.begin(), samples.end(), std::back_inserter(m_raw_sensor_samples.accelerometer));
    }
    {
        auto const& samples = m_hal.sensors->get_compass_samples();
        std::copy(samples.begin(), samples.end(), std::back_inserter(m_raw_sensor_samples.compass));
    }
    {
        auto const& samples = m_hal.sensors->get_barometer_samples();
        std::copy(samples.begin(), samples.end(), std::back_inserter(m_raw_sensor_samples.barometer));
    }
    {
        auto const& samples = m_hal.sensors->get_thermometer_samples();
        std::copy(samples.begin(), samples.end(), std::back_inserter(m_raw_sensor_samples.thermometer));
    }
    {
        auto const& samples = m_hal.sensors->get_sonar_samples();
        std::copy(samples.begin(), samples.end(), std::back_inserter(m_raw_sensor_samples.sonar));
    }
    {
        auto const& samples = m_hal.sensors->get_voltage_samples();
        std::copy(samples.begin(), samples.end(), std::back_inserter(m_raw_sensor_samples.voltage));
    }
    {
        auto const& samples = m_hal.sensors->get_current_samples();
        std::copy(samples.begin(), samples.end(), std::back_inserter(m_raw_sensor_samples.current));
    }
    {
        auto const& samples = m_hal.sensors->get_gps_samples();
        std::copy(samples.begin(), samples.end(), std::back_inserter(m_raw_sensor_samples.gps));
    }

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

    m_channel->send(detail::Comm_Message::UAV_ROTATION, m_uav.get_ahrs().get_b2e_quat());
    m_channel->send(detail::Comm_Message::UAV_LINEAR_ACCELERATION, m_uav.get_linear_acceleration());
    m_channel->send(detail::Comm_Message::UAV_VELOCITY, m_uav.get_velocity());
    m_channel->send(detail::Comm_Message::UAV_POSITION, m_uav.get_position());
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
            case detail::Comm_Message::PING:  process_message_ping(); break;
            case detail::Comm_Message::PONG:  process_message_pong(); break;

            case detail::Comm_Message::CAMERA_INPUT: process_message_camera_input(); break;
            case detail::Comm_Message::UAV_INPUT: process_message_uav_input(); break;

            case detail::Comm_Message::RAW_SENSORS: process_message_raw_sensors(); break;

            case detail::Comm_Message::CALIBRATION_ACCELEROMETER: process_message_calibration_accelerometer(); break;
            case detail::Comm_Message::CALIBRATION_GYROSCOPE: process_message_calibration_gyroscope(); break;
            case detail::Comm_Message::CALIBRATION_COMPASS: process_message_calibration_compass(); break;

            case detail::Comm_Message::YAW_RATE_PID_PARAMS: process_message_yaw_rate_pid_params(); break;
            case detail::Comm_Message::PITCH_RATE_PID_PARAMS: process_message_pitch_rate_pid_params(); break;
            case detail::Comm_Message::ROLL_RATE_PID_PARAMS: process_message_roll_rate_pid_params(); break;
            case detail::Comm_Message::ALTITUDE_RATE_PID_PARAMS: process_message_altitude_rate_pid_params(); break;
            case detail::Comm_Message::YAW_PID_PARAMS: process_message_yaw_pid_params(); break;
            case detail::Comm_Message::PITCH_PID_PARAMS: process_message_pitch_pid_params(); break;
            case detail::Comm_Message::ROLL_PID_PARAMS: process_message_roll_pid_params(); break;
            case detail::Comm_Message::ALTITUDE_PID_PARAMS: process_message_altitude_pid_params(); break;

            case detail::Comm_Message::ASSIST_PARAMS: process_message_assist_params(); break;

            default:
                SILK_WARNING("Received unhandled message: {}", static_cast<int>(msg.get()));
                m_error_count++;
            break;
        }
    }

    store_raw_sensor_data();

    send_sensor_data();
    send_uav_data();

    auto now = q::Clock::now();

    if (now - m_ping.last_time_point >= std::chrono::milliseconds(100))
    {
        if (m_ping.seq_sent.size() > 100)
        {
            m_ping.seq_sent.clear();
        }
        m_ping.seq_sent[m_ping.seq] = now;
        m_channel->send(detail::Comm_Message::PONG, static_cast<uint64_t>(now.time_since_epoch().count()), m_ping.seq);
        m_ping.seq++;
    }
}


