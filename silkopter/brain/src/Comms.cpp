#include "BrainStdAfx.h"
#include "Comms.h"
#include "utils/Timed_Scope.h"


using namespace silk;
using namespace boost::asio;

constexpr uint8_t COMMS_CHANNEL = 12;
constexpr uint8_t TELEMETRY_CHANNEL = 13;

Comms::Comms(boost::asio::io_service& io_service, HAL& hal, UAV& uav)
    : m_io_service(io_service)
    , m_hal(hal)
    , m_uav(uav)
    , m_socket(io_service)
    , m_rudp(m_socket)
    , m_comms_channel(m_rudp)
    , m_telemetry_channel(m_rudp)
{
    util::RUDP::Send_Params sparams;
    sparams.is_compressed = false;

    sparams.is_reliable = false;
    sparams.importance = 64;
    m_rudp.set_send_params(TELEMETRY_CHANNEL, sparams);

    sparams.is_reliable = true;
    sparams.importance = 127;
    m_rudp.set_send_params(COMMS_CHANNEL, sparams);

    util::RUDP::Receive_Params rparams;
    rparams.max_receive_time = std::chrono::seconds(999999);
    m_rudp.set_receive_params(COMMS_CHANNEL, rparams);

    rparams.max_receive_time = std::chrono::milliseconds(500);
    m_rudp.set_receive_params(TELEMETRY_CHANNEL, rparams);
}

auto Comms::start(uint16_t send_port, uint16_t receive_port) -> bool
{
    try
    {
        m_socket.open(ip::udp::v4());
        m_socket.set_option(ip::udp::socket::reuse_address(true));
        m_socket.set_option(socket_base::send_buffer_size(65536));
        m_socket.bind(ip::udp::endpoint(ip::udp::v4(), receive_port));
        //m_rudp.set_send_endpoint(ip::udp::endpoint(ip::address::from_string("192.168.1.37"), send_port));

        m_rudp.start();
    }
    catch(std::exception e)
    {
        SILK_WARNING("Cannot start comms on ports s:{} r:{}", send_port, receive_port);
        return false;
    }

    m_send_port = send_port;
    m_receive_port = receive_port;


    m_is_connected = true;
    SILK_INFO("Started sending on ports s:{} r:{}", send_port, receive_port);

    return true;
}

auto Comms::is_connected() const -> bool
{
    return m_is_connected;
}

auto Comms::get_remote_address() const -> boost::asio::ip::address
{
    return m_rudp.get_send_endpoint().address();
}

auto Comms::get_error_count() const -> size_t
{
    return m_error_count;
}

auto Comms::get_remote_clock() const -> Manual_Clock const&
{
    return m_remote_clock;
}

void Comms::process_message_camera_input()
{
    using namespace camera_input;

    if (!m_comms_channel.begin_unpack())
    {
        SILK_ERR("Cannot unpack camera input message");
        m_comms_channel.end_unpack();
        return;
    }

    Input q;
    if (!m_comms_channel.unpack_param(q))
    {
        SILK_ERR("Cannot unpack camera input message");
        m_comms_channel.end_unpack();
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
                if (m_comms_channel.unpack_param(iso))
                {
                    m_hal.camera->set_iso(iso);
                }
            }
            break;
        case Input::SHUTTER_SPEED:
            {
                Shutter_Speed ss;
                if (m_comms_channel.unpack_param(ss))
                {
                    m_hal.camera->set_shutter_speed(ss);
                }
            }
            break;
        case Input::STREAM_QUALITY:
            {
                Stream_Quality sq;
                if (m_comms_channel.unpack_param(sq))
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
    
    m_comms_channel.end_unpack();

    SILK_INFO("Camera input received");
}
void Comms::process_message_uav_input()
{
    using namespace uav_input;

    if (!m_comms_channel.begin_unpack())
    {
        SILK_ERR("Cannot unpack uav input message");
        m_comms_channel.end_unpack();
        return;
    }

    Input q;
    if (!m_comms_channel.unpack_param(q))
    {
        SILK_ERR("Cannot unpack uav input message");
        m_comms_channel.end_unpack();
        return;
    }
    switch(q)
    {
    case Input::THROTTLE_MODE:
        {
            Throttle_Mode v;
            if (m_comms_channel.unpack_param(v))
            {
                m_uav.set_throttle_mode(v);
            }
        }
        break;
    case Input::PITCH_ROLL_MODE:
        {
            Pitch_Roll_Mode v;
            if (m_comms_channel.unpack_param(v))
            {
                m_uav.set_pitch_roll_mode(v);
            }
        }
        break;
    case Input::YAW_MODE:
        {
            Yaw_Mode v;
            if (m_comms_channel.unpack_param(v))
            {
                m_uav.set_yaw_mode(v);
            }
        }
        break;
    case Input::STICKS:
        {
            Sticks v;
            if (m_comms_channel.unpack_param(v))
            {
                m_uav.set_sticks(v);
            }
        }
        break;
    case Input::ASSISTS:
        {
            Assists v;
            if (m_comms_channel.unpack_param(v))
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

    m_comms_channel.end_unpack();

    SILK_INFO("UAV input received");
}

void Comms::process_message_yaw_rate_pid_params()
{
    auto params = m_uav.get_yaw_rate_pid_params();
    if (m_comms_channel.get_message_size() == 0)
    {
        SILK_INFO("yaw rate pid params get");
        m_comms_channel.pack(detail::Comm_Message::YAW_RATE_PID_PARAMS, params);
    }
    else
    {
        if (m_comms_channel.unpack(params))
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
    if (m_comms_channel.get_message_size() == 0)
    {
        SILK_INFO("pitch rate pid params get");
        m_comms_channel.pack(detail::Comm_Message::PITCH_RATE_PID_PARAMS, params);
    }
    else
    {
        if (m_comms_channel.unpack(params))
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
    if (m_comms_channel.get_message_size() == 0)
    {
        SILK_INFO("roll rate pid params get");
        m_comms_channel.pack(detail::Comm_Message::ROLL_RATE_PID_PARAMS, params);
    }
    else
    {
        if (m_comms_channel.unpack(params))
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
    if (m_comms_channel.get_message_size() == 0)
    {
        SILK_INFO("altitude rate pid params get");
        m_comms_channel.pack(detail::Comm_Message::ALTITUDE_RATE_PID_PARAMS, params);
    }
    else
    {
        if (m_comms_channel.unpack(params))
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
    if (m_comms_channel.get_message_size() == 0)
    {
        SILK_INFO("yaw pid params get");
        m_comms_channel.pack(detail::Comm_Message::YAW_PID_PARAMS, params);
    }
    else
    {
        if (m_comms_channel.unpack(params))
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
    if (m_comms_channel.get_message_size() == 0)
    {
        SILK_INFO("pitch pid params get");
        m_comms_channel.pack(detail::Comm_Message::PITCH_PID_PARAMS, params);
    }
    else
    {
        if (m_comms_channel.unpack(params))
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
    if (m_comms_channel.get_message_size() == 0)
    {
        SILK_INFO("roll pid params get");
        m_comms_channel.pack(detail::Comm_Message::ROLL_PID_PARAMS, params);
    }
    else
    {
        if (m_comms_channel.unpack(params))
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
    if (m_comms_channel.get_message_size() == 0)
    {
        SILK_INFO("altitude pid params get");
        m_comms_channel.pack(detail::Comm_Message::ALTITUDE_PID_PARAMS, params);
    }
    else
    {
        if (m_comms_channel.unpack(params))
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
    if (m_comms_channel.get_message_size() == 0)
    {
        SILK_INFO("assist params get");
        m_comms_channel.pack(detail::Comm_Message::ASSIST_PARAMS, params);
    }
    else
    {
        if (m_comms_channel.unpack(params))
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
    detail::Telemetry_Message_Sensor sensors;
    if (!m_comms_channel.unpack(sensors))
    {
        SILK_INFO("Failed to unpack raw sensors request");
        return;
    }

    send_raw_sensor_samples(sensors);
}


void Comms::process_message_calibration_accelerometer()
{
    QASSERT(m_hal.sensors);
    math::vec3f bias, scale;
    if (m_comms_channel.get_message_size() == 0)
    {
        SILK_INFO("Accelerometer calibration set req");
        m_hal.sensors->get_accelerometer_calibration_data(bias, scale);
        m_comms_channel.pack(detail::Comm_Message::CALIBRATION_ACCELEROMETER, bias, scale);
    }
    else
    {
        if (m_comms_channel.unpack(bias, scale))
        {
            SILK_INFO("Accelerometer calibration changed");
            m_hal.sensors->set_accelerometer_calibration_data(bias, scale);
            m_comms_channel.pack(detail::Comm_Message::CALIBRATION_ACCELEROMETER, bias, scale);
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
    if (m_comms_channel.get_message_size() == 0)
    {
        SILK_INFO("Gyroscope calibration set req");
        m_hal.sensors->get_gyroscope_calibration_data(bias);
        m_comms_channel.pack(detail::Comm_Message::CALIBRATION_GYROSCOPE, bias);
    }
    else
    {
        if (m_comms_channel.unpack(bias))
        {
            SILK_INFO("Gyroscope calibration changed");
            m_hal.sensors->set_gyroscope_calibration_data(bias);
            m_comms_channel.pack(detail::Comm_Message::CALIBRATION_GYROSCOPE, bias);
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
    if (m_comms_channel.get_message_size() == 0)
    {
        SILK_INFO("Compass calibration req, sending to GS");
        m_hal.sensors->get_compass_calibration_data(bias);
        m_comms_channel.pack(detail::Comm_Message::CALIBRATION_COMPASS, bias);
    }
    else
    {
        if (m_comms_channel.unpack(bias))
        {
            SILK_INFO("Compass calibration changed");
            m_hal.sensors->set_compass_calibration_data(bias);
            m_comms_channel.pack(detail::Comm_Message::CALIBRATION_COMPASS, bias);
        }
        else
        {
            SILK_WARNING("Failed to receive compass calibration");
        }
    }
}

void Comms::send_raw_sensor_samples(detail::Telemetry_Message_Sensors sensors)
{
    QASSERT(m_hal.sensors);
    if (!sensors.any())
    {
        return;
    }

    m_telemetry_channel.begin_pack(detail::Telemetry_Message::SENSORS);

    m_telemetry_channel.pack_param(sensors);

    if (sensors.test(detail::Telemetry_Message_Sensor::ACCELEROMETER))
    {
        m_telemetry_channel.pack_param(static_cast<uint32_t>(m_raw_sensor_samples.accelerometer.size()));
        for (auto& a: m_raw_sensor_samples.accelerometer) { m_telemetry_channel.pack_param(a); }
    }
    if (sensors.test(detail::Telemetry_Message_Sensor::GYROSCOPE))
    {
        m_telemetry_channel.pack_param(static_cast<uint32_t>(m_raw_sensor_samples.gyroscope.size()));
        for (auto& a: m_raw_sensor_samples.gyroscope) { m_telemetry_channel.pack_param(a); }
    }
    if (sensors.test(detail::Telemetry_Message_Sensor::COMPASS))
    {
        m_telemetry_channel.pack_param(static_cast<uint32_t>(m_raw_sensor_samples.compass.size()));
        for (auto& a: m_raw_sensor_samples.compass) { m_telemetry_channel.pack_param(a); }
    }
    if (sensors.test(detail::Telemetry_Message_Sensor::BAROMETER))
    {
        m_telemetry_channel.pack_param(static_cast<uint32_t>(m_raw_sensor_samples.barometer.size()));
        for (auto& a: m_raw_sensor_samples.barometer) { m_telemetry_channel.pack_param(a); }
    }
    if (sensors.test(detail::Telemetry_Message_Sensor::THERMOMETER))
    {
        m_telemetry_channel.pack_param(static_cast<uint32_t>(m_raw_sensor_samples.thermometer.size()));
        for (auto& a: m_raw_sensor_samples.thermometer) { m_telemetry_channel.pack_param(a); }
    }
    if (sensors.test(detail::Telemetry_Message_Sensor::SONAR))
    {
        m_telemetry_channel.pack_param(static_cast<uint32_t>(m_raw_sensor_samples.sonar.size()));
        for (auto& a: m_raw_sensor_samples.sonar) { m_telemetry_channel.pack_param(a); }
    }
    if (sensors.test(detail::Telemetry_Message_Sensor::VOLTAGE))
    {
        m_telemetry_channel.pack_param(static_cast<uint32_t>(m_raw_sensor_samples.voltage.size()));
        for (auto& a: m_raw_sensor_samples.voltage) { m_telemetry_channel.pack_param(a); }
    }
    if (sensors.test(detail::Telemetry_Message_Sensor::CURRENT))
    {
        m_telemetry_channel.pack_param(static_cast<uint32_t>(m_raw_sensor_samples.current.size()));
        for (auto& a: m_raw_sensor_samples.current) { m_telemetry_channel.pack_param(a); }
    }
    if (sensors.test(detail::Telemetry_Message_Sensor::GPS))
    {
        m_telemetry_channel.pack_param(static_cast<uint32_t>(m_raw_sensor_samples.gps.size()));
        for (auto& a: m_raw_sensor_samples.gps) { m_telemetry_channel.pack_param(a); }
    }

    m_telemetry_channel.end_pack();

    m_raw_sensor_samples.last_sent = q::Clock::now();
    clear_raw_sensor_samples();
}

void Comms::clear_raw_sensor_samples()
{
    m_raw_sensor_samples.accelerometer.clear();
    m_raw_sensor_samples.gyroscope.clear();
    m_raw_sensor_samples.compass.clear();
    m_raw_sensor_samples.barometer.clear();
    m_raw_sensor_samples.thermometer.clear();
    m_raw_sensor_samples.sonar.clear();
    m_raw_sensor_samples.voltage.clear();
    m_raw_sensor_samples.current.clear();
    m_raw_sensor_samples.gps.clear();
}

void Comms::store_raw_sensor_samples()
{
    constexpr std::chrono::seconds MAX_DURATION{4};

    auto now = q::Clock::now();
    if (now - m_raw_sensor_samples.last_sent >= MAX_DURATION)
    {
        clear_raw_sensor_samples();
        return;
    }

    {
        auto const& samples = m_hal.sensors->get_accelerometer_samples();
        std::copy(samples.begin(), samples.end(), std::back_inserter(m_raw_sensor_samples.accelerometer));
    }
    {
        auto const& samples = m_hal.sensors->get_gyroscope_samples();
        std::copy(samples.begin(), samples.end(), std::back_inserter(m_raw_sensor_samples.gyroscope));
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


void Comms::send_sensor_samples()
{
    QASSERT(m_hal.sensors);
    auto now = q::Clock::now();
    auto delay = std::chrono::milliseconds(30);

    //SILK_INFO("acc: {}", m_sensors_samples.accelerometer.size());
    if (now - m_sensor_samples.last_sent_timestamp < delay)
    {
        return;
    }

    detail::Telemetry_Message_Sensors sensors;

    {
        static q::util::Rand rnd;
        auto sample = m_hal.sensors->get_last_accelerometer_sample();
        //if (sample.sample_idx != m_sensor_samples.accelerometer.sample_idx)
        {
            sensors.set(detail::Telemetry_Message_Sensor::ACCELEROMETER);
            sample.value.set(rnd.get_float(), rnd.get_float(), rnd.get_float());
            m_sensor_samples.accelerometer = sample;
        }
    }
    {
        auto const& sample = m_hal.sensors->get_last_gyroscope_sample();
        if (sample.sample_idx != m_sensor_samples.gyroscope.sample_idx)
        {
            sensors.set(detail::Telemetry_Message_Sensor::GYROSCOPE);
            m_sensor_samples.gyroscope = sample;
        }
    }
    {
        auto const& sample = m_hal.sensors->get_last_compass_sample();
        if (sample.sample_idx != m_sensor_samples.compass.sample_idx)
        {
            sensors.set(detail::Telemetry_Message_Sensor::COMPASS);
            m_sensor_samples.compass = sample;
        }
    }
    {
        auto const& sample = m_hal.sensors->get_last_barometer_sample();
        if (sample.sample_idx != m_sensor_samples.barometer.sample_idx)
        {
            sensors.set(detail::Telemetry_Message_Sensor::BAROMETER);
            m_sensor_samples.barometer = sample;
        }
    }
    {
        auto const& sample = m_hal.sensors->get_last_thermometer_sample();
        if (sample.sample_idx != m_sensor_samples.thermometer.sample_idx)
        {
            sensors.set(detail::Telemetry_Message_Sensor::THERMOMETER);
            m_sensor_samples.thermometer = sample;
        }
    }
    {
        auto const& sample = m_hal.sensors->get_last_sonar_sample();
        if (sample.sample_idx != m_sensor_samples.sonar.sample_idx)
        {
            sensors.set(detail::Telemetry_Message_Sensor::SONAR);
            m_sensor_samples.sonar = sample;
        }
    }
    {
        auto const& sample = m_hal.sensors->get_last_voltage_sample();
        if (sample.sample_idx != m_sensor_samples.voltage.sample_idx)
        {
            sensors.set(detail::Telemetry_Message_Sensor::VOLTAGE);
            m_sensor_samples.voltage = sample;
        }
    }
    {
        auto const& sample = m_hal.sensors->get_last_current_sample();
        if (sample.sample_idx != m_sensor_samples.current.sample_idx)
        {
            sensors.set(detail::Telemetry_Message_Sensor::CURRENT);
            m_sensor_samples.current = sample;
        }
    }
    {
        auto const& sample = m_hal.sensors->get_last_gps_sample();
        if (sample.sample_idx != m_sensor_samples.gps.sample_idx)
        {
            sensors.set(detail::Telemetry_Message_Sensor::GPS);
            m_sensor_samples.gps = sample;
        }
    }

    if (!sensors.any())
    {
        return;
    }

    m_telemetry_channel.begin_pack(detail::Telemetry_Message::SENSORS);
    m_telemetry_channel.pack_param(sensors);

    if (sensors.test(detail::Telemetry_Message_Sensor::ACCELEROMETER))
    {
        m_telemetry_channel.pack_param(m_sensor_samples.accelerometer);
    }
    if (sensors.test(detail::Telemetry_Message_Sensor::GYROSCOPE))
    {
        m_telemetry_channel.pack_param(m_sensor_samples.gyroscope);
    }
    if (sensors.test(detail::Telemetry_Message_Sensor::COMPASS))
    {
        m_telemetry_channel.pack_param(m_sensor_samples.compass);
    }
    if (sensors.test(detail::Telemetry_Message_Sensor::BAROMETER))
    {
        m_telemetry_channel.pack_param(m_sensor_samples.barometer);
    }
    if (sensors.test(detail::Telemetry_Message_Sensor::THERMOMETER))
    {
        m_telemetry_channel.pack_param(m_sensor_samples.thermometer);
    }
    if (sensors.test(detail::Telemetry_Message_Sensor::SONAR))
    {
        m_telemetry_channel.pack_param(m_sensor_samples.sonar);
    }
    if (sensors.test(detail::Telemetry_Message_Sensor::VOLTAGE))
    {
        m_telemetry_channel.pack_param(m_sensor_samples.voltage);
    }
    if (sensors.test(detail::Telemetry_Message_Sensor::CURRENT))
    {
        m_telemetry_channel.pack_param(m_sensor_samples.current);
    }
    if (sensors.test(detail::Telemetry_Message_Sensor::GPS))
    {
        m_telemetry_channel.pack_param(m_sensor_samples.gps);
    }

    m_telemetry_channel.end_pack();

    m_sensor_samples.last_sent_timestamp = now;
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

    m_telemetry_channel.pack(detail::Telemetry_Message::UAV_ROTATION_L2W, m_uav.get_ahrs().get_quat_l2w());
    m_telemetry_channel.pack(detail::Telemetry_Message::UAV_LINEAR_ACCELERATION_W, m_uav.get_linear_acceleration_w());
    m_telemetry_channel.pack(detail::Telemetry_Message::UAV_VELOCITY_W, m_uav.get_velocity_w());
    m_telemetry_channel.pack(detail::Telemetry_Message::UAV_POSITION_W, m_uav.get_position_w());
}

void Comms::process()
{
    if (!is_connected())
    {
        return;
    }

    while (auto msg = m_comms_channel.get_next_message(COMMS_CHANNEL))
    {
        switch (msg.get())
        {
            case detail::Comm_Message::CAMERA_INPUT: process_message_camera_input(); break;
            case detail::Comm_Message::UAV_INPUT: process_message_uav_input(); break;

            case detail::Comm_Message::REQUEST_RAW_SENSORS: process_message_raw_sensors(); break;

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

    store_raw_sensor_samples();

    send_sensor_samples();
    send_uav_data();

    {
        if (m_rudp.get_send_endpoint().address().is_unspecified() && !m_rudp.get_last_receive_endpoint().address().is_unspecified())
        {
            auto endpoint = m_rudp.get_last_receive_endpoint();
            endpoint.port(m_send_port);
            m_rudp.set_send_endpoint(endpoint);
        }
    }

    m_rudp.process();
    m_comms_channel.send(COMMS_CHANNEL);
    m_telemetry_channel.send(TELEMETRY_CHANNEL);


//    static std::vector<uint8_t> buf;
//    if (buf.empty())
//    {
//        buf.resize(16000000);
//        std::generate(buf.begin(), buf.end(), [](){ return rand() % 50;});
//    }

//    while(true)
//    {
//        m_rudp.send(12, buf.data(), buf.size());

//        m_rudp.process();
//        //m_channel.send(COMMS_CHANNEL);
//        static int xxx = 0;
//        SILK_INFO("{}", xxx);
//        xxx++;
//        boost::this_thread::sleep_for(boost::chrono::milliseconds(30));
//    }
}

auto Comms::get_rudp() -> util::RUDP&
{
    return m_rudp;
}

