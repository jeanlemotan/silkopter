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


auto Comms::get_accelerometer_samples() const  -> std::vector<Accelerometer_Sample> const&
{
    return m_sensor_samples.accelerometer;
}
auto Comms::get_gyroscope_samples() const      -> std::vector<Gyroscope_Sample> const&
{
    return m_sensor_samples.gyroscope;
}
auto Comms::get_compass_samples() const        -> std::vector<Compass_Sample> const&
{
    return m_sensor_samples.compass;
}
auto Comms::get_barometer_samples() const      -> std::vector<Barometer_Sample> const&
{
    return m_sensor_samples.barometer;
}
auto Comms::get_sonar_samples() const          -> std::vector<Sonar_Sample> const&
{
    return m_sensor_samples.sonar;
}
auto Comms::get_thermometer_samples() const    -> std::vector<Thermometer_Sample> const&
{
    return m_sensor_samples.thermometer;
}
auto Comms::get_voltage_samples() const        -> std::vector<Voltage_Sample> const&
{
    return m_sensor_samples.voltage;
}
auto Comms::get_current_samples() const        -> std::vector<Current_Sample> const&
{
    return m_sensor_samples.current;
}
auto Comms::get_gps_samples() const            -> std::vector<GPS_Sample> const&
{
    return m_sensor_samples.gps;
}

auto Comms::get_uav_rotation_sample() const -> UAV_Rotation_Sample
{
    return m_uav.rotation;
}
auto Comms::get_uav_linear_acceleration_sample() const -> UAV_Linear_Acceleration_Sample
{
    return m_uav.linear_acceleration;
}
auto Comms::get_uav_velocity_sample() const -> UAV_Velocity_Sample
{
    return m_uav.velocity;
}
auto Comms::get_uav_position_sample() const -> UAV_Position_Sample
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

template<class SAMPLE_T>
auto Comms::unpack_sensor_sample(SAMPLE_T& sample) -> bool
{
    uint32_t dt_us = 0;
    uint64_t time_point_us = 0;
    uint32_t sample_idx = 0;
    if (!m_channel.unpack_param(sample.value) ||
        !m_channel.unpack_param(sample_idx) ||
        !m_channel.unpack_param(dt_us) ||
        !m_channel.unpack_param(time_point_us))
    {
        return false;
    }
    sample.dt = std::chrono::microseconds(dt_us);
    sample.time_point = Remote_Clock::time_point(std::chrono::microseconds(time_point_us));
    sample.sample_idx = sample_idx;
    return true;
}

template<class SAMPLE_T>
auto Comms::unpack_sensor_samples(std::vector<SAMPLE_T>& samples) -> bool
{
    uint32_t count = 0;
    if (m_channel.unpack_param(count))
    {
        auto start = samples.size();
        samples.resize(start + count);
        for (size_t i = 0; i < count; i++)
        {
            if (!unpack_sensor_sample(samples[start + i]))
            {
                return false;
            }
        }
    }
    return true;
}

void Comms::process_message_sensors()
{
    Sensors sensors;
    if (!m_channel.begin_unpack() || !m_channel.unpack_param(sensors))
    {
        SILK_WARNING("Failed to receive sensors data");
        return;
    }

    bool res = true;
    if (m_remote_clock.is_set())
    {
        if (res && sensors.test(Sensor::ACCELEROMETER))
        {
            res = unpack_sensor_samples(m_sensor_samples.accelerometer);
        }
        if (res && sensors.test(Sensor::GYROSCOPE))
        {
            res = unpack_sensor_samples(m_sensor_samples.gyroscope);
        }
        if (res && sensors.test(Sensor::COMPASS))
        {
            res = unpack_sensor_samples(m_sensor_samples.compass);
        }
        if (res && sensors.test(Sensor::BAROMETER))
        {
            res = unpack_sensor_samples(m_sensor_samples.barometer);
        }
        if (res && sensors.test(Sensor::THERMOMETER))
        {
            res = unpack_sensor_samples(m_sensor_samples.thermometer);
        }
        if (res && sensors.test(Sensor::SONAR))
        {
            res = unpack_sensor_samples(m_sensor_samples.sonar);
        }
        if (res && sensors.test(Sensor::VOLTAGE))
        {
            res = unpack_sensor_samples(m_sensor_samples.voltage);
        }
        if (res && sensors.test(Sensor::CURRENT))
        {
            res = unpack_sensor_samples(m_sensor_samples.current);
        }
        if (res && sensors.test(Sensor::GPS))
        {
            res = unpack_sensor_samples(m_sensor_samples.gps);
        }
    }
    else
    {
        SILK_WARNING("Skipping sensor data because remote clock is not set");
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
    uint64_t remote_now = 0;
    uint32_t seq = 0;
    if (!m_channel.unpack(remote_now) || !m_channel.unpack(seq))
    {
        SILK_WARNING("Failed to unpack ping");
        return;
    }

    m_channel.send(Message::PONG, static_cast<uint64_t>(q::Clock::now().time_since_epoch().count()), seq);
    m_channel.flush();

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
    if (!m_channel.unpack(remote_now) || !m_channel.unpack(seq))
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

void Comms::process_message_uav_rotation()
{
    if (!unpack_sensor_sample(m_uav.rotation))
    {
        SILK_WARNING("Failed to receive uav rotation");
        return;
    }
}
void Comms::process_message_uav_linear_acceleration()
{
    if (!unpack_sensor_sample(m_uav.linear_acceleration))
    {
        SILK_WARNING("Failed to receive uav linear_acceleration");
        return;
    }
}
void Comms::process_message_uav_velocity()
{
    if (!unpack_sensor_sample(m_uav.velocity))
    {
        SILK_WARNING("Failed to receive uav velocity");
        return;
    }
}
void Comms::process_message_uav_position()
{
    if (!unpack_sensor_sample(m_uav.position))
    {
        SILK_WARNING("Failed to receive uav position");
        return;
    }
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

    m_sensor_samples.gyroscope.clear();
    m_sensor_samples.accelerometer.clear();
    m_sensor_samples.compass.clear();
    m_sensor_samples.barometer.clear();
    m_sensor_samples.thermometer.clear();
    m_sensor_samples.sonar.clear();
    m_sensor_samples.voltage.clear();
    m_sensor_samples.current.clear();
    m_sensor_samples.gps.clear();

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

            case Message::PING:  process_message_ping(); break;
            case Message::PONG:  process_message_pong(); break;

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

    auto now = q::Clock::now();
    if (now - m_ping.last_time_point >= std::chrono::milliseconds(100))
    {
        if (m_ping.seq_sent.size() > 100)
        {
            m_ping.seq_sent.clear();
        }
        m_ping.seq_sent[m_ping.seq] = now;
        m_channel.send(Message::PONG, static_cast<uint64_t>(now.time_since_epoch().count()), m_ping.seq);
        m_ping.seq++;
    }
}


