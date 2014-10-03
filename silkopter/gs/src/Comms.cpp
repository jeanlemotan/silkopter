#include "stdafx.h"
#include "Comms.h"

using namespace silk;
using namespace boost::asio;

Comms::Comms(boost::asio::io_service& io_service)
    : m_io_service(io_service)
    , m_socket(io_service)
    , m_channel(m_socket)
{
}

auto Comms::connect(boost::asio::ip::address const& address, uint16_t port) -> Result
{
    try
    {
        disconnect();

        m_remote_endpoint = ip::tcp::endpoint(address, port);
        boost::system::error_code error = boost::asio::error::host_not_found;
        //m_socket.connect(m_remote_endpoint, error);

        if (!error)
        {
            SILK_INFO("Connected to {}:{}", address.to_string(), port);
            m_channel.start();
        }
        else
        {
            m_socket.close();
            SILK_WARNING("Connect failed: {}", error.message());
            return Result::FAILED;
        }
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

auto Comms::get_sensor_accelerometer_data() const -> Accelerometer_Data
{
    return m_sensor_data.accelerometer;
}
auto Comms::get_sensor_gyroscope_data() const -> Gyroscope_Data
{
    return m_sensor_data.gyroscope;
}
auto Comms::get_sensor_compass_data() const -> Compass_Data
{
    return m_sensor_data.compass;
}
auto Comms::get_sensor_barometer_data() const -> Barometer_Data
{
    return m_sensor_data.barometer;
}
auto Comms::get_sensor_sonar_data() const -> Sonar_Data
{
    return m_sensor_data.sonar;
}
auto Comms::get_sensor_thermometer_data() const -> Thermometer_Data
{
    return m_sensor_data.thermometer;
}
auto Comms::get_sensor_gps_data() const -> GPS_Data
{
    return m_sensor_data.gps;
}
auto Comms::get_uav_rotation_data() const -> UAV_Rotation_Data
{
    return m_uav.rotation;
}
auto Comms::get_uav_linear_acceleration_data() const -> UAV_Linear_Acceleration_Data
{
    return m_uav.linear_acceleration;
}
auto Comms::get_uav_velocity_data() const -> UAV_Velocity_Data
{
    return m_uav.velocity;
}
auto Comms::get_uav_position_data() const -> UAV_Position_Data
{
    return m_uav.position;
}

void Comms::set_yaw_rate_pid_params(Yaw_Rate_PID::Params const& params)
{
    m_channel.send(Message::YAW_RATE_PID_PARAMS, params);
}
void Comms::request_yaw_rate_pid_params()
{
    m_channel.send(Message::YAW_RATE_PID_PARAMS);
}
void Comms::set_pitch_rate_pid_params(Pitch_Rate_PID::Params const& params)
{
    m_channel.send(Message::PITCH_RATE_PID_PARAMS, params);
}
void Comms::request_pitch_rate_pid_params()
{
    m_channel.send(Message::PITCH_RATE_PID_PARAMS);
}
void Comms::set_roll_rate_pid_params(Roll_Rate_PID::Params const& params)
{
    m_channel.send(Message::ROLL_RATE_PID_PARAMS, params);
}
void Comms::request_roll_rate_pid_params()
{
    m_channel.send(Message::ROLL_RATE_PID_PARAMS);
}
void Comms::set_altitude_rate_pid_params(Altitude_Rate_PID::Params const& params)
{
    m_channel.send(Message::ALTITUDE_RATE_PID_PARAMS, params);
}
void Comms::request_altitude_rate_pid_params()
{
    m_channel.send(Message::ALTITUDE_RATE_PID_PARAMS);
}
void Comms::set_yaw_pid_params(Yaw_PID::Params const& params)
{
    m_channel.send(Message::YAW_PID_PARAMS, params);
}
void Comms::request_yaw_pid_params()
{
    m_channel.send(Message::YAW_PID_PARAMS);
}
void Comms::set_pitch_pid_params(Pitch_PID::Params const& params)
{
    m_channel.send(Message::PITCH_PID_PARAMS, params);
}
void Comms::request_pitch_pid_params()
{
    m_channel.send(Message::PITCH_PID_PARAMS);
}
void Comms::set_roll_pid_params(Roll_PID::Params const& params)
{
    m_channel.send(Message::ROLL_PID_PARAMS, params);
}
void Comms::request_roll_pid_params()
{
    m_channel.send(Message::ROLL_PID_PARAMS);
}
void Comms::set_altitude_pid_params(Altitude_PID::Params const& params)
{
    m_channel.send(Message::ALTITUDE_PID_PARAMS, params);
}
void Comms::request_altitude_pid_params()
{
    m_channel.send(Message::ALTITUDE_PID_PARAMS);
}

void Comms::process_message_sensors()
{
    Sensors sensors;
    auto result = m_channel.begin_unpack();
    if (result != Channel::Unpack_Result::OK ||
        m_channel.unpack_param(sensors) != Channel::Unpack_Result::OK)
    {
        SILK_WARNING("Failed to receive sensors data");
        return;
    }

    if (sensors.test(Sensor::ACCELEROMETER) && result == Channel::Unpack_Result::OK)
    {
        uint16_t count = 0;
        result = m_channel.unpack_param(count);
        auto start = m_sensor_data.accelerometer.value.accelerations.size();
        m_sensor_data.accelerometer.value.accelerations.resize(start + count);
        for (size_t i = 0; i < count; i++)
        {
            result = m_channel.unpack_param(m_sensor_data.accelerometer.value.accelerations[start + i]);
            uint16_t sample_time_us = 0;
            result = m_channel.unpack_param(sample_time_us);
            if (result != Channel::Unpack_Result::OK)
            {
                break;
            }
            m_sensor_data.accelerometer.value.sample_time = std::chrono::microseconds(sample_time_us);
        }
        if (result == Channel::Unpack_Result::OK)
        {
            m_sensor_data.accelerometer.timestamp = q::Clock::now();
            m_sensor_data.accelerometer.sample_idx++;
        }
    }
    if (sensors.test(Sensor::GYROSCOPE) && result == Channel::Unpack_Result::OK)
    {
        uint16_t count = 0;
        result = m_channel.unpack_param(count);
        auto start = m_sensor_data.gyroscope.value.angular_velocities.size();
        m_sensor_data.gyroscope.value.angular_velocities.resize(start + count);
        for (size_t i = 0; i < count; i++)
        {
            result = m_channel.unpack_param(m_sensor_data.gyroscope.value.angular_velocities[start + i]);
            uint16_t sample_time_us = 0;
            result = m_channel.unpack_param(sample_time_us);
            if (result != Channel::Unpack_Result::OK)
            {
                break;
            }
            m_sensor_data.gyroscope.value.sample_time = std::chrono::microseconds(sample_time_us);
        }
        if (result == Channel::Unpack_Result::OK)
        {
            m_sensor_data.gyroscope.timestamp = q::Clock::now();
            m_sensor_data.gyroscope.sample_idx++;
        }
    }
    if (sensors.test(Sensor::COMPASS) && result == Channel::Unpack_Result::OK)
    {
        result = m_channel.unpack_param(m_sensor_data.compass.value);
        if (result == Channel::Unpack_Result::OK)
        {
            m_sensor_data.compass.timestamp = q::Clock::now();
            m_sensor_data.compass.sample_idx++;
        }
    }
    if (sensors.test(Sensor::BAROMETER) && result == Channel::Unpack_Result::OK)
    {
        result = m_channel.unpack_param(m_sensor_data.barometer.value);
        if (result == Channel::Unpack_Result::OK)
        {
            m_sensor_data.barometer.timestamp = q::Clock::now();
            m_sensor_data.barometer.sample_idx++;
        }
    }
    if (sensors.test(Sensor::THERMOMETER) && result == Channel::Unpack_Result::OK)
    {
        result = m_channel.unpack_param(m_sensor_data.thermometer.value);
        if (result == Channel::Unpack_Result::OK)
        {
            m_sensor_data.thermometer.timestamp = q::Clock::now();
            m_sensor_data.thermometer.sample_idx++;
        }
    }
    if (sensors.test(Sensor::SONAR) && result == Channel::Unpack_Result::OK)
    {
        result = m_channel.unpack_param(m_sensor_data.sonar.value);
        if (result == Channel::Unpack_Result::OK)
        {
            m_sensor_data.sonar.timestamp = q::Clock::now();
            m_sensor_data.sonar.sample_idx++;
        }
    }

    m_channel.end_unpack();

    if (result != Channel::Unpack_Result::OK)
    {
        SILK_WARNING("Failed to receive sensors data");
        return;
    }
}


void Comms::process_message_calibration_accelerometer()
{
    math::vec3f bias, scale;
    if (m_channel.unpack(bias, scale) != Channel::Unpack_Result::OK)
    {
        SILK_WARNING("Failed to receive accelerometer calibration");
        return;
    }
    accelerometer_calibration_data_received.execute(bias, scale);
}
void Comms::process_message_calibration_gyroscope()
{
    math::vec3f bias;
    if (m_channel.unpack(bias) != Channel::Unpack_Result::OK)
    {
        SILK_WARNING("Failed to receive gyroscope calibration");
        return;
    }
    gyroscope_calibration_data_received.execute(bias);
}
void Comms::process_message_calibration_compass()
{
    math::vec3f bias;
    if (m_channel.unpack(bias) != Channel::Unpack_Result::OK)
    {
        SILK_WARNING("Failed to receive compass calibration");
        return;
    }
    compass_calibration_data_received.execute(bias);
}

void Comms::process_message_yaw_rate_pid_params()
{
    Yaw_Rate_PID::Params params;
    if (m_channel.unpack(params) != Channel::Unpack_Result::OK)
    {
        SILK_WARNING("Failed to receive yaw rate pid params");
        return;
    }
    yaw_rate_pid_params_received.execute(params);
}
void Comms::process_message_pitch_rate_pid_params()
{
    Pitch_Rate_PID::Params params;
    if (m_channel.unpack(params) != Channel::Unpack_Result::OK)
    {
        SILK_WARNING("Failed to receive pitch rate pid params");
        return;
    }
    pitch_rate_pid_params_received.execute(params);
}
void Comms::process_message_roll_rate_pid_params()
{
    Roll_Rate_PID::Params params;
    if (m_channel.unpack(params) != Channel::Unpack_Result::OK)
    {
        SILK_WARNING("Failed to receive roll rate pid params");
        return;
    }
    roll_rate_pid_params_received.execute(params);
}
void Comms::process_message_altitude_rate_pid_params()
{
    Altitude_Rate_PID::Params params;
    if (m_channel.unpack(params) != Channel::Unpack_Result::OK)
    {
        SILK_WARNING("Failed to receive altitude rate pid params");
        return;
    }
    altitude_rate_pid_params_received.execute(params);
}

void Comms::process_message_yaw_pid_params()
{
    Yaw_PID::Params params;
    if (m_channel.unpack(params) != Channel::Unpack_Result::OK)
    {
        SILK_WARNING("Failed to receive yaw pid params");
        return;
    }
    yaw_pid_params_received.execute(params);
}
void Comms::process_message_pitch_pid_params()
{
    Pitch_PID::Params params;
    if (m_channel.unpack(params) != Channel::Unpack_Result::OK)
    {
        SILK_WARNING("Failed to receive pitch pid params");
        return;
    }
    pitch_pid_params_received.execute(params);
}
void Comms::process_message_roll_pid_params()
{
    Roll_PID::Params params;
    if (m_channel.unpack(params) != Channel::Unpack_Result::OK)
    {
        SILK_WARNING("Failed to receive roll pid params");
        return;
    }
    roll_pid_params_received.execute(params);
}
void Comms::process_message_altitude_pid_params()
{
    Altitude_PID::Params params;
    if (m_channel.unpack(params) != Channel::Unpack_Result::OK)
    {
        SILK_WARNING("Failed to receive altitude pid params");
        return;
    }
    altitude_pid_params_received.execute(params);
}

void Comms::process_message_uav_ping()
{
    uint32_t v;
    if (m_channel.unpack(v) != Channel::Unpack_Result::OK)
    {
        SILK_WARNING("Failed to receive uav ping");
        return;
    }
    m_uav_alive_duration = std::chrono::milliseconds(v);
}

void Comms::process_message_uav_rotation()
{
    if (m_channel.unpack(m_uav.rotation.value) != Channel::Unpack_Result::OK)
    {
        SILK_WARNING("Failed to receive uav rotation");
        return;
    }
    m_uav.rotation.timestamp = q::Clock::now();
    m_uav.rotation.sample_idx++;
}
void Comms::process_message_uav_linear_acceleration()
{
    if (m_channel.unpack(m_uav.linear_acceleration.value) != Channel::Unpack_Result::OK)
    {
        SILK_WARNING("Failed to receive uav linear_acceleration");
        return;
    }
    m_uav.linear_acceleration.timestamp = q::Clock::now();
    m_uav.linear_acceleration.sample_idx++;
}
void Comms::process_message_uav_velocity()
{
    if (m_channel.unpack(m_uav.velocity.value) != Channel::Unpack_Result::OK)
    {
        SILK_WARNING("Failed to receive uav velocity");
        return;
    }
    m_uav.velocity.timestamp = q::Clock::now();
    m_uav.velocity.sample_idx++;
}
void Comms::process_message_uav_position()
{
    if (m_channel.unpack(m_uav.position.value) != Channel::Unpack_Result::OK)
    {
        SILK_WARNING("Failed to receive uav position");
        return;
    }
    m_uav.position.timestamp = q::Clock::now();
    m_uav.position.sample_idx++;
}

auto Comms::get_uav_alive_duration() const -> q::Clock::duration
{
    return m_uav_alive_duration;
}

void Comms::set_accelerometer_calibration_data(math::vec3f const& bias, math::vec3f const& scale)
{
    m_channel.send(Message::CALIBRATION_ACCELEROMETER, bias, scale);
}

void Comms::request_accelerometer_calibration_data() const
{
    m_channel.send(Message::CALIBRATION_ACCELEROMETER);
}

void Comms::set_gyroscope_calibration_data(math::vec3f const& bias)
{
    m_channel.send(Message::CALIBRATION_GYROSCOPE, bias);
}
void Comms::request_gyroscope_calibration_data() const
{
    m_channel.send(Message::CALIBRATION_GYROSCOPE);
}

void Comms::set_compass_calibration_data(math::vec3f const& bias)
{
    m_channel.send(Message::CALIBRATION_COMPASS, bias);
}
void Comms::request_compass_calibration_data() const
{
    m_channel.send(Message::CALIBRATION_COMPASS);
}

void Comms::process()
{
    if (!is_connected())
    {
        return;
    }

    m_sensor_data.gyroscope.value.angular_velocities.clear();
    m_sensor_data.accelerometer.value.accelerations.clear();

    while (auto msg = m_channel.get_next_message())
    {
        switch (msg.get())
        {
            case Message::SENSORS:  process_message_sensors(); break;

            case Message::YAW_RATE_PID_PARAMS: process_message_yaw_rate_pid_params(); break;
            case Message::PITCH_RATE_PID_PARAMS: process_message_pitch_rate_pid_params(); break;
            case Message::ROLL_RATE_PID_PARAMS: process_message_roll_rate_pid_params(); break;
            case Message::ALTITUDE_RATE_PID_PARAMS: process_message_altitude_rate_pid_params(); break;

            case Message::YAW_PID_PARAMS: process_message_yaw_pid_params(); break;
            case Message::PITCH_PID_PARAMS: process_message_pitch_pid_params(); break;
            case Message::ROLL_PID_PARAMS: process_message_roll_pid_params(); break;
            case Message::ALTITUDE_PID_PARAMS: process_message_altitude_pid_params(); break;

            case Message::UAV_PING:  process_message_uav_ping(); break;

            case Message::CALIBRATION_ACCELEROMETER:  process_message_calibration_accelerometer(); break;
            case Message::CALIBRATION_GYROSCOPE:  process_message_calibration_gyroscope(); break;
            case Message::CALIBRATION_COMPASS:  process_message_calibration_compass(); break;

            case Message::UAV_ROTATION:  process_message_uav_rotation(); break;
            case Message::UAV_LINEAR_ACCELERATION:  process_message_uav_linear_acceleration(); break;
            case Message::UAV_VELOCITY:  process_message_uav_velocity(); break;
            case Message::UAV_POSITION:  process_message_uav_position(); break;
            default:
                SILK_WARNING("Received unhandled message: {}", static_cast<int>(msg.get()));
                m_error_count++;
            break;
        }
    }
}


