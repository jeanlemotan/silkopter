#pragma once

#include "common/input/Camera_Input.h"
#include "common/input/UAV_Input.h"
#include "common/sensors/Sensor_Samples.h"
#include "common/Manual_Clock.h"
#include "common/Comm_Data.h"
#include "utils/PID.h"

namespace silk
{


class Comms : q::util::Noncopyable
{
public:
    Comms(boost::asio::io_service& io_service);

    enum class Result
    {
        OK,
        FAILED,
    };

    auto start(boost::asio::ip::address const& address, uint16_t send_port, uint16_t receive_port) -> Result;
    void disconnect();

    auto is_connected() const -> bool;
    auto get_remote_address() const -> boost::asio::ip::address;

    auto get_remote_clock() const -> Manual_Clock const&;
    auto get_rudp() -> util::RUDP&;

    void process();

    auto get_error_count() const -> size_t;

    template<typename... Params>
    void send_camera_input(camera_input::Input input, Params&&... params)
    {
        m_comms_channel.pack(detail::Comm_Message::CAMERA_INPUT, input, params...);
    }

    template<typename... Params>
    void send_uav_input(uav_input::Input input, Params&&... params)
    {
        m_comms_channel.pack(detail::Comm_Message::UAV_INPUT, input, params...);
    }


    auto get_accelerometer_sample() const  -> Accelerometer_Sample const&;
    auto get_gyroscope_sample() const      -> Gyroscope_Sample const&;
    auto get_compass_sample() const        -> Compass_Sample const&;
    auto get_barometer_sample() const      -> Barometer_Sample const&;
    auto get_sonar_sample() const          -> Sonar_Sample const&;
    auto get_thermometer_sample() const    -> Thermometer_Sample const&;
    auto get_voltage_sample() const        -> Voltage_Sample const&;
    auto get_current_sample() const        -> Current_Sample const&;
    auto get_gps_sample() const            -> GPS_Sample const&;

    auto get_uav_rotation_l2w() const -> math::quatf const&;
    auto get_uav_linear_acceleration_w() const -> math::vec3f const&;
    auto get_uav_velocity_w() const -> math::vec3f const&;
    auto get_uav_position_w() const -> math::vec3f const&;

    //----------------------------------------------------------------------
    //calibration

    void set_accelerometer_calibration_data(math::vec3f const& bias, math::vec3f const& scale);
    q::util::Signal<void(math::vec3f const& bias, math::vec3f const& scale)> accelerometer_calibration_data_received;
    void request_accelerometer_calibration_data() const;

    void set_gyroscope_calibration_data(math::vec3f const& bias);
    q::util::Signal<void(math::vec3f const& bias)> gyroscope_calibration_data_received;
    void request_gyroscope_calibration_data() const;

    void set_compass_calibration_data(math::vec3f const& bias);
    q::util::Signal<void(math::vec3f const& bias)> compass_calibration_data_received;
    void request_compass_calibration_data() const;

    //----------------------------------------------------------------------
    // pids
    typedef util::PID<float> Yaw_Rate_PID;
    typedef util::PID<float> Pitch_Rate_PID;
    typedef util::PID<float> Roll_Rate_PID;
    typedef util::PID<float> Altitude_Rate_PID;

    typedef util::PID<float> Yaw_PID;
    typedef util::PID<float> Pitch_PID;
    typedef util::PID<float> Roll_PID;
    typedef util::PID<float> Altitude_PID;

    void set_yaw_rate_pid_params(Yaw_Rate_PID::Params const& params);
    q::util::Signal<void(Yaw_Rate_PID::Params const&)> yaw_rate_pid_params_received;
    void request_yaw_rate_pid_params();

    void set_pitch_rate_pid_params(Pitch_Rate_PID::Params const& params);
    q::util::Signal<void(Pitch_Rate_PID::Params const&)> pitch_rate_pid_params_received;
    void request_pitch_rate_pid_params();

    void set_roll_rate_pid_params(Roll_Rate_PID::Params const& params);
    q::util::Signal<void(Roll_Rate_PID::Params const&)> roll_rate_pid_params_received;
    void request_roll_rate_pid_params();

    void set_altitude_rate_pid_params(Altitude_Rate_PID::Params const& params);
    q::util::Signal<void(Altitude_Rate_PID::Params const&)> altitude_rate_pid_params_received;
    void request_altitude_rate_pid_params();


    void set_yaw_pid_params(Yaw_PID::Params const& params);
    q::util::Signal<void(Yaw_PID::Params const&)> yaw_pid_params_received;
    void request_yaw_pid_params();

    void set_pitch_pid_params(Pitch_PID::Params const& params);
    q::util::Signal<void(Pitch_PID::Params const&)> pitch_pid_params_received;
    void request_pitch_pid_params();

    void set_roll_pid_params(Roll_PID::Params const& params);
    q::util::Signal<void(Roll_PID::Params const&)> roll_pid_params_received;
    void request_roll_pid_params();

    void set_altitude_pid_params(Altitude_PID::Params const& params);
    q::util::Signal<void(Altitude_PID::Params const&)> altitude_pid_params_received;
    void request_altitude_pid_params();

    //----------------------------------------------------------------------

private:
    boost::asio::io_service& m_io_service;

    boost::asio::ip::udp::socket m_socket;
    boost::asio::ip::udp::endpoint m_remote_endpoint;

    util::RUDP m_rudp;

    typedef util::Channel<detail::Comm_Message, uint16_t> Comms_Channel;
    typedef util::Channel<detail::Telemetry_Message, uint16_t> Telemetry_Channel;
    mutable Comms_Channel m_comms_channel;
    q::Clock::time_point m_last_comms_sent_time_stamp = q::Clock::now();

    mutable Telemetry_Channel m_telemetry_channel;
    q::Clock::time_point m_last_telemetry_sent_time_stamp = q::Clock::now();

    Manual_Clock m_remote_clock;

    size_t m_error_count = 0;

    template<class SAMPLE_T>
    auto unpack_sensor_sample(SAMPLE_T& sample) -> bool;
    template<class SAMPLE_T>
    auto unpack_sensor_samples(std::vector<SAMPLE_T>& samples) -> bool;

    void process_message_sensors();

    void process_message_calibration_accelerometer();
    void process_message_calibration_gyroscope();
    void process_message_calibration_compass();

    void process_message_yaw_rate_pid_params();
    void process_message_pitch_rate_pid_params();
    void process_message_roll_rate_pid_params();
    void process_message_altitude_rate_pid_params();

    void process_message_yaw_pid_params();
    void process_message_pitch_pid_params();
    void process_message_roll_pid_params();
    void process_message_altitude_pid_params();

    void process_message_uav_rotation_l2w();
    void process_message_uav_linear_acceleration_w();
    void process_message_uav_velocity_w();
    void process_message_uav_position_w();


    struct Sensor_Samples
    {
        Accelerometer_Sample accelerometer;
        Gyroscope_Sample gyroscope;
        Compass_Sample compass;
        Barometer_Sample barometer;
        Sonar_Sample sonar;
        Thermometer_Sample thermometer;
        Voltage_Sample voltage;
        Current_Sample current;
        GPS_Sample gps;
    } m_sensor_samples;

    struct UAV
    {
        math::quatf rotation_l2w;
        math::vec3f linear_acceleration_w;
        math::vec3f velocity_w;
        math::vec3f position_w;
    } m_uav;
};

}
