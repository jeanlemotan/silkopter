#include "stdafx.h"
#include "Comms.h"

using namespace silk;
using namespace boost::asio;

constexpr uint8_t COMMS_CHANNEL = 12;
constexpr uint8_t TELEMETRY_CHANNEL = 13;


Comms::Comms(boost::asio::io_service& io_service)
    : m_io_service(io_service)
    , m_socket(io_service)
    , m_rudp(m_socket)
    , m_comms_channel(m_rudp, COMMS_CHANNEL)
    , m_telemetry_channel(m_rudp, TELEMETRY_CHANNEL)
{
    util::RUDP::Send_Params sparams;
    sparams.is_compressed = true;

    sparams.is_reliable = false;
    sparams.importance = 64;
    m_rudp.set_send_params(TELEMETRY_CHANNEL, sparams);

    sparams.is_reliable = true;
    sparams.importance = 127;
    m_rudp.set_send_params(COMMS_CHANNEL, sparams);

    util::RUDP::Receive_Params rparams;
    rparams.max_receive_time = std::chrono::seconds(999999);
    m_rudp.set_receive_params(COMMS_CHANNEL, rparams);

    rparams.max_receive_time = std::chrono::milliseconds(50);
    m_rudp.set_receive_params(TELEMETRY_CHANNEL, rparams);

}

auto Comms::start(boost::asio::ip::address const& address, uint16_t send_port, uint16_t receive_port) -> Result
{
    try
    {
        m_socket.open(ip::udp::v4());
        m_socket.set_option(ip::udp::socket::reuse_address(true));
        m_socket.set_option(socket_base::send_buffer_size(65536));
        m_socket.bind(ip::udp::endpoint(ip::udp::v4(), receive_port));

        m_rudp.set_send_endpoint(ip::udp::endpoint(address, send_port));

        m_rudp.start();

        SILK_INFO("Started sending on port {} and receiving on port {}", send_port, receive_port);
    }
    catch(...)
    {
        m_socket.close();
        SILK_WARNING("Connect failed");
        return Result::FAILED;
    }

    return Result::OK;
}

void Comms::disconnect()
{
    m_socket.close();
}

auto Comms::is_connected() const -> bool
{
    return m_socket.is_open();
}

auto Comms::get_remote_address() const -> boost::asio::ip::address
{
    return m_remote_endpoint.address();
}

auto Comms::get_error_count() const -> size_t
{
    return m_error_count;
}

auto Comms::get_remote_clock() const -> Manual_Clock const&
{
    return m_remote_clock;
}


auto Comms::get_accelerometer_sample() const  -> Accelerometer_Sample const&
{
    return m_sensor_samples.accelerometer;
}
auto Comms::get_gyroscope_sample() const      -> Gyroscope_Sample const&
{
    return m_sensor_samples.gyroscope;
}
auto Comms::get_compass_sample() const        -> Compass_Sample const&
{
    return m_sensor_samples.compass;
}
auto Comms::get_barometer_sample() const      -> Barometer_Sample const&
{
    return m_sensor_samples.barometer;
}
auto Comms::get_sonar_sample() const          -> Sonar_Sample const&
{
    return m_sensor_samples.sonar;
}
auto Comms::get_thermometer_sample() const    -> Thermometer_Sample const&
{
    return m_sensor_samples.thermometer;
}
auto Comms::get_voltage_sample() const        -> Voltage_Sample const&
{
    return m_sensor_samples.voltage;
}
auto Comms::get_current_sample() const        -> Current_Sample const&
{
    return m_sensor_samples.current;
}
auto Comms::get_gps_sample() const            -> GPS_Sample const&
{
    return m_sensor_samples.gps;
}

auto Comms::get_uav_rotation_l2w() const -> math::quatf const&
{
    return m_uav.rotation_l2w;
}
auto Comms::get_uav_linear_acceleration_w() const -> math::vec3f const&
{
    return m_uav.linear_acceleration_w;
}
auto Comms::get_uav_velocity_w() const -> math::vec3f const&
{
    return m_uav.velocity_w;
}
auto Comms::get_uav_position_w() const -> math::vec3f const&
{
    return m_uav.position_w;
}

void Comms::set_yaw_rate_pid_params(Yaw_Rate_PID::Params const& params)
{
    m_comms_channel.pack(detail::Comm_Message::YAW_RATE_PID_PARAMS, params);
}
void Comms::request_yaw_rate_pid_params()
{
    m_comms_channel.pack(detail::Comm_Message::YAW_RATE_PID_PARAMS);
}
void Comms::set_pitch_rate_pid_params(Pitch_Rate_PID::Params const& params)
{
    m_comms_channel.pack(detail::Comm_Message::PITCH_RATE_PID_PARAMS, params);
}
void Comms::request_pitch_rate_pid_params()
{
    m_comms_channel.pack(detail::Comm_Message::PITCH_RATE_PID_PARAMS);
}
void Comms::set_roll_rate_pid_params(Roll_Rate_PID::Params const& params)
{
    m_comms_channel.pack(detail::Comm_Message::ROLL_RATE_PID_PARAMS, params);
}
void Comms::request_roll_rate_pid_params()
{
    m_comms_channel.pack(detail::Comm_Message::ROLL_RATE_PID_PARAMS);
}
void Comms::set_altitude_rate_pid_params(Altitude_Rate_PID::Params const& params)
{
    m_comms_channel.pack(detail::Comm_Message::ALTITUDE_RATE_PID_PARAMS, params);
}
void Comms::request_altitude_rate_pid_params()
{
    m_comms_channel.pack(detail::Comm_Message::ALTITUDE_RATE_PID_PARAMS);
}
void Comms::set_yaw_pid_params(Yaw_PID::Params const& params)
{
    m_comms_channel.pack(detail::Comm_Message::YAW_PID_PARAMS, params);
}
void Comms::request_yaw_pid_params()
{
    m_comms_channel.pack(detail::Comm_Message::YAW_PID_PARAMS);
}
void Comms::set_pitch_pid_params(Pitch_PID::Params const& params)
{
    m_comms_channel.pack(detail::Comm_Message::PITCH_PID_PARAMS, params);
}
void Comms::request_pitch_pid_params()
{
    m_comms_channel.pack(detail::Comm_Message::PITCH_PID_PARAMS);
}
void Comms::set_roll_pid_params(Roll_PID::Params const& params)
{
    m_comms_channel.pack(detail::Comm_Message::ROLL_PID_PARAMS, params);
}
void Comms::request_roll_pid_params()
{
    m_comms_channel.pack(detail::Comm_Message::ROLL_PID_PARAMS);
}
void Comms::set_altitude_pid_params(Altitude_PID::Params const& params)
{
    m_comms_channel.pack(detail::Comm_Message::ALTITUDE_PID_PARAMS, params);
}
void Comms::request_altitude_pid_params()
{
    m_comms_channel.pack(detail::Comm_Message::ALTITUDE_PID_PARAMS);
}

void Comms::process_message_sensors()
{
    auto& channel = m_telemetry_channel;
    detail::Telemetry_Message_Sensors sensors;
    if (!channel.begin_unpack() || !channel.unpack_param(sensors))
    {
        SILK_WARNING("Failed to receive sensors data");
        return;
    }

    bool res = true;
    if (res && sensors.test(detail::Telemetry_Message_Sensor::ACCELEROMETER))
    {
        res = channel.unpack_param(m_sensor_samples.accelerometer);
    }
    if (res && sensors.test(detail::Telemetry_Message_Sensor::GYROSCOPE))
    {
        res = channel.unpack_param(m_sensor_samples.gyroscope);
    }
    if (res && sensors.test(detail::Telemetry_Message_Sensor::COMPASS))
    {
        res = channel.unpack_param(m_sensor_samples.compass);
    }
    if (res && sensors.test(detail::Telemetry_Message_Sensor::BAROMETER))
    {
        res = channel.unpack_param(m_sensor_samples.barometer);
    }
    if (res && sensors.test(detail::Telemetry_Message_Sensor::THERMOMETER))
    {
        res = channel.unpack_param(m_sensor_samples.thermometer);
    }
    if (res && sensors.test(detail::Telemetry_Message_Sensor::SONAR))
    {
        res = channel.unpack_param(m_sensor_samples.sonar);
    }
    if (res && sensors.test(detail::Telemetry_Message_Sensor::VOLTAGE))
    {
        res = channel.unpack_param(m_sensor_samples.voltage);
    }
    if (res && sensors.test(detail::Telemetry_Message_Sensor::CURRENT))
    {
        res = channel.unpack_param(m_sensor_samples.current);
    }
    if (res && sensors.test(detail::Telemetry_Message_Sensor::GPS))
    {
        res = channel.unpack_param(m_sensor_samples.gps);
    }

    channel.end_unpack();

    if (!res)
    {
        SILK_WARNING("Failed to receive sensors data");
        return;
    }
}


void Comms::process_message_calibration_accelerometer()
{
    math::vec3f bias, scale;
    if (!m_comms_channel.unpack(bias, scale))
    {
        SILK_WARNING("Failed to receive accelerometer calibration");
        return;
    }
    accelerometer_calibration_data_received.execute(bias, scale);
}
void Comms::process_message_calibration_gyroscope()
{
    math::vec3f bias;
    if (!m_comms_channel.unpack(bias))
    {
        SILK_WARNING("Failed to receive gyroscope calibration");
        return;
    }
    gyroscope_calibration_data_received.execute(bias);
}
void Comms::process_message_calibration_compass()
{
    math::vec3f bias;
    if (!m_comms_channel.unpack(bias))
    {
        SILK_WARNING("Failed to receive compass calibration");
        return;
    }
    compass_calibration_data_received.execute(bias);
}

void Comms::process_message_yaw_rate_pid_params()
{
    Yaw_Rate_PID::Params params;
    if (!m_comms_channel.unpack(params))
    {
        SILK_WARNING("Failed to receive yaw rate pid params");
        return;
    }
    yaw_rate_pid_params_received.execute(params);
}
void Comms::process_message_pitch_rate_pid_params()
{
    Pitch_Rate_PID::Params params;
    if (!m_comms_channel.unpack(params))
    {
        SILK_WARNING("Failed to receive pitch rate pid params");
        return;
    }
    pitch_rate_pid_params_received.execute(params);
}
void Comms::process_message_roll_rate_pid_params()
{
    Roll_Rate_PID::Params params;
    if (!m_comms_channel.unpack(params))
    {
        SILK_WARNING("Failed to receive roll rate pid params");
        return;
    }
    roll_rate_pid_params_received.execute(params);
}
void Comms::process_message_altitude_rate_pid_params()
{
    Altitude_Rate_PID::Params params;
    if (!m_comms_channel.unpack(params))
    {
        SILK_WARNING("Failed to receive altitude rate pid params");
        return;
    }
    altitude_rate_pid_params_received.execute(params);
}

void Comms::process_message_yaw_pid_params()
{
    Yaw_PID::Params params;
    if (!m_comms_channel.unpack(params))
    {
        SILK_WARNING("Failed to receive yaw pid params");
        return;
    }
    yaw_pid_params_received.execute(params);
}
void Comms::process_message_pitch_pid_params()
{
    Pitch_PID::Params params;
    if (!m_comms_channel.unpack(params))
    {
        SILK_WARNING("Failed to receive pitch pid params");
        return;
    }
    pitch_pid_params_received.execute(params);
}
void Comms::process_message_roll_pid_params()
{
    Roll_PID::Params params;
    if (!m_comms_channel.unpack(params))
    {
        SILK_WARNING("Failed to receive roll pid params");
        return;
    }
    roll_pid_params_received.execute(params);
}
void Comms::process_message_altitude_pid_params()
{
    Altitude_PID::Params params;
    if (!m_comms_channel.unpack(params))
    {
        SILK_WARNING("Failed to receive altitude pid params");
        return;
    }
    altitude_pid_params_received.execute(params);
}

void Comms::process_message_uav_rotation_l2w()
{
    if (!m_telemetry_channel.unpack_param(m_uav.rotation_l2w))
    {
        SILK_WARNING("Failed to receive uav rotation");
        return;
    }
}
void Comms::process_message_uav_linear_acceleration_w()
{
    if (!m_telemetry_channel.unpack_param(m_uav.linear_acceleration_w))
    {
        SILK_WARNING("Failed to receive uav linear_acceleration");
        return;
    }
}
void Comms::process_message_uav_velocity_w()
{
    if (!m_telemetry_channel.unpack_param(m_uav.velocity_w))
    {
        SILK_WARNING("Failed to receive uav velocity");
        return;
    }
}
void Comms::process_message_uav_position_w()
{
    if (!m_telemetry_channel.unpack_param(m_uav.position_w))
    {
        SILK_WARNING("Failed to receive uav position");
        return;
    }
}

void Comms::set_accelerometer_calibration_data(math::vec3f const& bias, math::vec3f const& scale)
{
    m_comms_channel.pack(detail::Comm_Message::CALIBRATION_ACCELEROMETER, bias, scale);
}

void Comms::request_accelerometer_calibration_data() const
{
    m_comms_channel.pack(detail::Comm_Message::CALIBRATION_ACCELEROMETER);
}

void Comms::set_gyroscope_calibration_data(math::vec3f const& bias)
{
    m_comms_channel.pack(detail::Comm_Message::CALIBRATION_GYROSCOPE, bias);
}
void Comms::request_gyroscope_calibration_data() const
{
    m_comms_channel.pack(detail::Comm_Message::CALIBRATION_GYROSCOPE);
}

void Comms::set_compass_calibration_data(math::vec3f const& bias)
{
    m_comms_channel.pack(detail::Comm_Message::CALIBRATION_COMPASS, bias);
}
void Comms::request_compass_calibration_data() const
{
    m_comms_channel.pack(detail::Comm_Message::CALIBRATION_COMPASS);
}

void Comms::process()
{
    if (!is_connected())
    {
        return;
    }

//    static int xxx = 0;
//    xxx++;
//    SILK_INFO("LOOP: {}", xxx);
    while (auto msg = m_telemetry_channel.get_next_message())
    {
        switch (msg.get())
        {
            case detail::Telemetry_Message::SENSORS:  process_message_sensors(); break;

            case detail::Telemetry_Message::UAV_AHRS_ROTATION_L2W:  process_message_uav_rotation_l2w(); break;
            case detail::Telemetry_Message::UAV_LINEAR_ACCELERATION_W:  process_message_uav_linear_acceleration_w(); break;
            case detail::Telemetry_Message::UAV_VELOCITY_W:  process_message_uav_velocity_w(); break;
            case detail::Telemetry_Message::UAV_POSITION_W:  process_message_uav_position_w(); break;
            default:
                SILK_WARNING("Received unhandled message: {}", static_cast<int>(msg.get()));
                m_error_count++;
            break;
        }
    }

    while (auto msg = m_comms_channel.get_next_message())
    {
        switch (msg.get())
        {
            case detail::Comm_Message::REQUEST_RAW_SENSORS:  process_message_sensors(); break;

            case detail::Comm_Message::YAW_RATE_PID_PARAMS: process_message_yaw_rate_pid_params(); break;
            case detail::Comm_Message::PITCH_RATE_PID_PARAMS: process_message_pitch_rate_pid_params(); break;
            case detail::Comm_Message::ROLL_RATE_PID_PARAMS: process_message_roll_rate_pid_params(); break;
            case detail::Comm_Message::ALTITUDE_RATE_PID_PARAMS: process_message_altitude_rate_pid_params(); break;

            case detail::Comm_Message::YAW_PID_PARAMS: process_message_yaw_pid_params(); break;
            case detail::Comm_Message::PITCH_PID_PARAMS: process_message_pitch_pid_params(); break;
            case detail::Comm_Message::ROLL_PID_PARAMS: process_message_roll_pid_params(); break;
            case detail::Comm_Message::ALTITUDE_PID_PARAMS: process_message_altitude_pid_params(); break;

            case detail::Comm_Message::CALIBRATION_ACCELEROMETER:  process_message_calibration_accelerometer(); break;
            case detail::Comm_Message::CALIBRATION_GYROSCOPE:  process_message_calibration_gyroscope(); break;
            case detail::Comm_Message::CALIBRATION_COMPASS:  process_message_calibration_compass(); break;
            default:
                SILK_WARNING("Received unhandled message: {}", static_cast<int>(msg.get()));
                m_error_count++;
            break;
        }
    }
//    SILK_INFO("*********** LOOP: {}", xxx);

    m_rudp.process();
    m_comms_channel.send();
    m_telemetry_channel.try_sending();
}

auto Comms::get_rudp() -> util::RUDP&
{
    return m_rudp;
}
