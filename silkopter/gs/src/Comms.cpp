#include "stdafx.h"
#include "Comms.h"

using namespace silk;
using namespace boost::asio;

Comms::Comms(boost::asio::io_service& io_service)
    : m_io_service(io_service)
    , m_socket(io_service)
    , m_channel(m_socket)
{
    m_ping.last_time_point = q::Clock::now();

    m_ping.rtts.set_capacity(100);
}

auto Comms::connect(boost::asio::ip::address const& address, uint16_t port) -> Result
{
    try
    {
        disconnect();

        m_remote_endpoint = ip::tcp::endpoint(address, port);
        boost::system::error_code error = boost::asio::error::host_not_found;
        m_socket.connect(m_remote_endpoint, error);

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

auto Comms::get_remote_clock() const -> Remote_Clock const&
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
    m_channel.send(detail::Comm_Message::YAW_RATE_PID_PARAMS, params);
}
void Comms::request_yaw_rate_pid_params()
{
    m_channel.send(detail::Comm_Message::YAW_RATE_PID_PARAMS);
}
void Comms::set_pitch_rate_pid_params(Pitch_Rate_PID::Params const& params)
{
    m_channel.send(detail::Comm_Message::PITCH_RATE_PID_PARAMS, params);
}
void Comms::request_pitch_rate_pid_params()
{
    m_channel.send(detail::Comm_Message::PITCH_RATE_PID_PARAMS);
}
void Comms::set_roll_rate_pid_params(Roll_Rate_PID::Params const& params)
{
    m_channel.send(detail::Comm_Message::ROLL_RATE_PID_PARAMS, params);
}
void Comms::request_roll_rate_pid_params()
{
    m_channel.send(detail::Comm_Message::ROLL_RATE_PID_PARAMS);
}
void Comms::set_altitude_rate_pid_params(Altitude_Rate_PID::Params const& params)
{
    m_channel.send(detail::Comm_Message::ALTITUDE_RATE_PID_PARAMS, params);
}
void Comms::request_altitude_rate_pid_params()
{
    m_channel.send(detail::Comm_Message::ALTITUDE_RATE_PID_PARAMS);
}
void Comms::set_yaw_pid_params(Yaw_PID::Params const& params)
{
    m_channel.send(detail::Comm_Message::YAW_PID_PARAMS, params);
}
void Comms::request_yaw_pid_params()
{
    m_channel.send(detail::Comm_Message::YAW_PID_PARAMS);
}
void Comms::set_pitch_pid_params(Pitch_PID::Params const& params)
{
    m_channel.send(detail::Comm_Message::PITCH_PID_PARAMS, params);
}
void Comms::request_pitch_pid_params()
{
    m_channel.send(detail::Comm_Message::PITCH_PID_PARAMS);
}
void Comms::set_roll_pid_params(Roll_PID::Params const& params)
{
    m_channel.send(detail::Comm_Message::ROLL_PID_PARAMS, params);
}
void Comms::request_roll_pid_params()
{
    m_channel.send(detail::Comm_Message::ROLL_PID_PARAMS);
}
void Comms::set_altitude_pid_params(Altitude_PID::Params const& params)
{
    m_channel.send(detail::Comm_Message::ALTITUDE_PID_PARAMS, params);
}
void Comms::request_altitude_pid_params()
{
    m_channel.send(detail::Comm_Message::ALTITUDE_PID_PARAMS);
}

void Comms::process_message_sensors()
{
    detail::Comm_Message_Sensors sensors;
    if (!m_channel.begin_unpack() || !m_channel.unpack_param(sensors))
    {
        SILK_WARNING("Failed to receive sensors data");
        return;
    }

    bool res = true;
    if (res && sensors.test(detail::Comm_Message_Sensor::ACCELEROMETER))
    {
        res = m_channel.unpack_param(m_sensor_samples.accelerometer);
    }
    if (res && sensors.test(detail::Comm_Message_Sensor::GYROSCOPE))
    {
        res = m_channel.unpack_param(m_sensor_samples.gyroscope);
    }
    if (res && sensors.test(detail::Comm_Message_Sensor::COMPASS))
    {
        res = m_channel.unpack_param(m_sensor_samples.compass);
    }
    if (res && sensors.test(detail::Comm_Message_Sensor::BAROMETER))
    {
        res = m_channel.unpack_param(m_sensor_samples.barometer);
    }
    if (res && sensors.test(detail::Comm_Message_Sensor::THERMOMETER))
    {
        res = m_channel.unpack_param(m_sensor_samples.thermometer);
    }
    if (res && sensors.test(detail::Comm_Message_Sensor::SONAR))
    {
        res = m_channel.unpack_param(m_sensor_samples.sonar);
    }
    if (res && sensors.test(detail::Comm_Message_Sensor::VOLTAGE))
    {
        res = m_channel.unpack_param(m_sensor_samples.voltage);
    }
    if (res && sensors.test(detail::Comm_Message_Sensor::CURRENT))
    {
        res = m_channel.unpack_param(m_sensor_samples.current);
    }
    if (res && sensors.test(detail::Comm_Message_Sensor::GPS))
    {
        res = m_channel.unpack_param(m_sensor_samples.gps);
    }

    m_channel.end_unpack();

    if (!res)
    {
        SILK_WARNING("Failed to receive sensors data");
        return;
    }
}


void Comms::process_message_calibration_accelerometer()
{
    math::vec3f bias, scale;
    if (!m_channel.unpack(bias, scale))
    {
        SILK_WARNING("Failed to receive accelerometer calibration");
        return;
    }
    accelerometer_calibration_data_received.execute(bias, scale);
}
void Comms::process_message_calibration_gyroscope()
{
    math::vec3f bias;
    if (!m_channel.unpack(bias))
    {
        SILK_WARNING("Failed to receive gyroscope calibration");
        return;
    }
    gyroscope_calibration_data_received.execute(bias);
}
void Comms::process_message_calibration_compass()
{
    math::vec3f bias;
    if (!m_channel.unpack(bias))
    {
        SILK_WARNING("Failed to receive compass calibration");
        return;
    }
    compass_calibration_data_received.execute(bias);
}

void Comms::process_message_yaw_rate_pid_params()
{
    Yaw_Rate_PID::Params params;
    if (!m_channel.unpack(params))
    {
        SILK_WARNING("Failed to receive yaw rate pid params");
        return;
    }
    yaw_rate_pid_params_received.execute(params);
}
void Comms::process_message_pitch_rate_pid_params()
{
    Pitch_Rate_PID::Params params;
    if (!m_channel.unpack(params))
    {
        SILK_WARNING("Failed to receive pitch rate pid params");
        return;
    }
    pitch_rate_pid_params_received.execute(params);
}
void Comms::process_message_roll_rate_pid_params()
{
    Roll_Rate_PID::Params params;
    if (!m_channel.unpack(params))
    {
        SILK_WARNING("Failed to receive roll rate pid params");
        return;
    }
    roll_rate_pid_params_received.execute(params);
}
void Comms::process_message_altitude_rate_pid_params()
{
    Altitude_Rate_PID::Params params;
    if (!m_channel.unpack(params))
    {
        SILK_WARNING("Failed to receive altitude rate pid params");
        return;
    }
    altitude_rate_pid_params_received.execute(params);
}

void Comms::process_message_yaw_pid_params()
{
    Yaw_PID::Params params;
    if (!m_channel.unpack(params))
    {
        SILK_WARNING("Failed to receive yaw pid params");
        return;
    }
    yaw_pid_params_received.execute(params);
}
void Comms::process_message_pitch_pid_params()
{
    Pitch_PID::Params params;
    if (!m_channel.unpack(params))
    {
        SILK_WARNING("Failed to receive pitch pid params");
        return;
    }
    pitch_pid_params_received.execute(params);
}
void Comms::process_message_roll_pid_params()
{
    Roll_PID::Params params;
    if (!m_channel.unpack(params))
    {
        SILK_WARNING("Failed to receive roll pid params");
        return;
    }
    roll_pid_params_received.execute(params);
}
void Comms::process_message_altitude_pid_params()
{
    Altitude_PID::Params params;
    if (!m_channel.unpack(params))
    {
        SILK_WARNING("Failed to receive altitude pid params");
        return;
    }
    altitude_pid_params_received.execute(params);
}

void Comms::process_message_ping()
{
    Remote_Clock::rep remote_now = 0;
    uint32_t seq = 0;
    if (!m_channel.unpack(remote_now, seq))
    {
        SILK_WARNING("Failed to unpack ping");
        return;
    }

    m_channel.send(detail::Comm_Message::PONG, seq);
    m_channel.flush();

    if (remote_now < m_remote_clock.now().time_since_epoch().count())
    {
        SILK_WARNING("Setting remote time in the past!!!");
    }
    m_remote_clock.set_now(Remote_Clock::duration(remote_now));
    //SILK_INFO("Remote Clock set to {}", m_remote_clock.now());
}
void Comms::process_message_pong()
{
    uint32_t seq = 0;
    if (!m_channel.unpack(seq))
    {
        SILK_WARNING("Failed to unpack pong");
        return;
    }

    auto it = m_ping.seq_sent.find(seq);
    if (it != m_ping.seq_sent.end())
    {
        auto rtt = q::Clock::now() - it->second;
        m_ping.rtts.push_back(rtt);
        m_ping.seq_sent.erase(it);
    }
    else
    {
        SILK_WARNING("invalid ping seq received: {}", seq);
    }
}

void Comms::process_message_uav_rotation_l2w()
{
    if (!m_channel.unpack_param(m_uav.rotation_l2w))
    {
        SILK_WARNING("Failed to receive uav rotation");
        return;
    }
}
void Comms::process_message_uav_linear_acceleration_w()
{
    if (!m_channel.unpack_param(m_uav.linear_acceleration_w))
    {
        SILK_WARNING("Failed to receive uav linear_acceleration");
        return;
    }
}
void Comms::process_message_uav_velocity_w()
{
    if (!m_channel.unpack_param(m_uav.velocity_w))
    {
        SILK_WARNING("Failed to receive uav velocity");
        return;
    }
}
void Comms::process_message_uav_position_w()
{
    if (!m_channel.unpack_param(m_uav.position_w))
    {
        SILK_WARNING("Failed to receive uav position");
        return;
    }
}

void Comms::set_accelerometer_calibration_data(math::vec3f const& bias, math::vec3f const& scale)
{
    m_channel.send(detail::Comm_Message::CALIBRATION_ACCELEROMETER, bias, scale);
}

void Comms::request_accelerometer_calibration_data() const
{
    m_channel.send(detail::Comm_Message::CALIBRATION_ACCELEROMETER);
}

void Comms::set_gyroscope_calibration_data(math::vec3f const& bias)
{
    m_channel.send(detail::Comm_Message::CALIBRATION_GYROSCOPE, bias);
}
void Comms::request_gyroscope_calibration_data() const
{
    m_channel.send(detail::Comm_Message::CALIBRATION_GYROSCOPE);
}

void Comms::set_compass_calibration_data(math::vec3f const& bias)
{
    m_channel.send(detail::Comm_Message::CALIBRATION_COMPASS, bias);
}
void Comms::request_compass_calibration_data() const
{
    m_channel.send(detail::Comm_Message::CALIBRATION_COMPASS);
}

void Comms::process()
{
    if (!is_connected())
    {
        return;
    }

    while (auto msg = m_channel.get_next_message())
    {
        switch (msg.get())
        {
            case detail::Comm_Message::SENSORS:  process_message_sensors(); break;

            case detail::Comm_Message::YAW_RATE_PID_PARAMS: process_message_yaw_rate_pid_params(); break;
            case detail::Comm_Message::PITCH_RATE_PID_PARAMS: process_message_pitch_rate_pid_params(); break;
            case detail::Comm_Message::ROLL_RATE_PID_PARAMS: process_message_roll_rate_pid_params(); break;
            case detail::Comm_Message::ALTITUDE_RATE_PID_PARAMS: process_message_altitude_rate_pid_params(); break;

            case detail::Comm_Message::YAW_PID_PARAMS: process_message_yaw_pid_params(); break;
            case detail::Comm_Message::PITCH_PID_PARAMS: process_message_pitch_pid_params(); break;
            case detail::Comm_Message::ROLL_PID_PARAMS: process_message_roll_pid_params(); break;
            case detail::Comm_Message::ALTITUDE_PID_PARAMS: process_message_altitude_pid_params(); break;

            case detail::Comm_Message::PING:  process_message_ping(); break;
            case detail::Comm_Message::PONG:  process_message_pong(); break;

            case detail::Comm_Message::CALIBRATION_ACCELEROMETER:  process_message_calibration_accelerometer(); break;
            case detail::Comm_Message::CALIBRATION_GYROSCOPE:  process_message_calibration_gyroscope(); break;
            case detail::Comm_Message::CALIBRATION_COMPASS:  process_message_calibration_compass(); break;

            case detail::Comm_Message::UAV_ROTATION_L2W:  process_message_uav_rotation_l2w(); break;
            case detail::Comm_Message::UAV_LINEAR_ACCELERATION_W:  process_message_uav_linear_acceleration_w(); break;
            case detail::Comm_Message::UAV_VELOCITY_W:  process_message_uav_velocity_w(); break;
            case detail::Comm_Message::UAV_POSITION_W:  process_message_uav_position_w(); break;
            default:
                SILK_WARNING("Received unhandled message: {}", static_cast<int>(msg.get()));
                m_error_count++;
            break;
        }
    }

    auto now = q::Clock::now();
    if (now - m_ping.last_time_point >= std::chrono::milliseconds(100))
    {
        m_ping.last_time_point = now;

        if (m_ping.seq_sent.size() > 1000)
        {
            std::chrono::seconds max_d(1);
            for (auto it = m_ping.seq_sent.begin(); it != m_ping.seq_sent.end(); )
            {
                if (now - it->second > max_d)
                {
                    it = m_ping.seq_sent.erase(it);
                }
                else
                {
                    ++it;
                }
            }
        }
        m_ping.seq_sent[m_ping.seq] = now;
        m_channel.send(detail::Comm_Message::PING, static_cast<uint64_t>(now.time_since_epoch().count()), m_ping.seq);
        m_ping.seq++;
    }
}


