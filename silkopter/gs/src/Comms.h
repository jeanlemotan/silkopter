#pragma once

#include "common/input/Camera_Input.h"
#include "common/input/UAV_Input.h"
#include "common/sensors/Sensor_Samples.h"
#include "common/Remote_Clock.h"
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

    auto connect(boost::asio::ip::address const& address, uint16_t port) -> Result;
    void disconnect();

    auto is_connected() const -> bool;
    auto get_remote_address() const -> boost::asio::ip::address;

    auto get_remote_clock() const -> Remote_Clock const&;

    void process();

    auto get_error_count() const -> size_t;

    template<typename... Params>
    void send_camera_input(camera_input::Input input, Params&&... params)
    {
        m_channel.send(Message::CAMERA_INPUT, input, params...);
    }

    template<typename... Params>
    void send_uav_input(uav_input::Input input, Params&&... params)
    {
        m_channel.send(Message::UAV_INPUT, input, params...);
    }


    auto get_accelerometer_samples() const  -> std::vector<Accelerometer_Sample> const&;
    auto get_gyroscope_samples() const      -> std::vector<Gyroscope_Sample> const&;
    auto get_compass_samples() const        -> std::vector<Compass_Sample> const&;
    auto get_barometer_samples() const      -> std::vector<Barometer_Sample> const&;
    auto get_sonar_samples() const          -> std::vector<Sonar_Sample> const&;
    auto get_thermometer_samples() const    -> std::vector<Thermometer_Sample> const&;
    auto get_voltage_samples() const        -> std::vector<Voltage_Sample> const&;
    auto get_current_samples() const        -> std::vector<Current_Sample> const&;
    auto get_gps_samples() const            -> std::vector<GPS_Sample> const&;

    typedef Sensor_Sample<math::quatf> UAV_Rotation_Sample; //local to world
    auto get_uav_rotation_sample() const -> UAV_Rotation_Sample;

    typedef Sensor_Sample<math::vec3f> UAV_Linear_Acceleration_Sample; //meters / second^2
    auto get_uav_linear_acceleration_sample() const -> UAV_Linear_Acceleration_Sample;

    typedef Sensor_Sample<math::vec3f> UAV_Velocity_Sample; //meters / second
    auto get_uav_velocity_sample() const -> UAV_Velocity_Sample;

    typedef Sensor_Sample<math::vec3f> UAV_Position_Sample; //meters
    auto get_uav_position_sample() const -> UAV_Position_Sample;

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
    uint16_t m_port = 0;

    enum class Message : uint16_t
    {
        PING,
        PONG,

        //------------------------
        CAMERA_INPUT,
        UAV_INPUT,

        //------------------------
        //sensors
        SENSORS,

        //------------------------
        //calibration

        CALIBRATION_ACCELEROMETER,
        CALIBRATION_GYROSCOPE,
        CALIBRATION_COMPASS,

        //------------------------
        //pids

        YAW_RATE_PID_PARAMS,
        PITCH_RATE_PID_PARAMS,
        ROLL_RATE_PID_PARAMS,
        ALTITUDE_RATE_PID_PARAMS,

        YAW_PID_PARAMS,
        PITCH_PID_PARAMS,
        ROLL_PID_PARAMS,
        ALTITUDE_PID_PARAMS,

        //------------------------
        //uav misc params
        ASSIST_PARAMS,

        //------------------------
        //UAV
        UAV_ROTATION, //local to world
        UAV_LINEAR_ACCELERATION,
        UAV_VELOCITY,
        UAV_POSITION
    };

    boost::asio::ip::tcp::socket m_socket;
    boost::asio::ip::tcp::endpoint m_remote_endpoint;

    typedef util::Channel<Message,
                        uint32_t,
                        boost::asio::ip::tcp::socket> Channel;

    mutable Channel m_channel;
    q::Clock::time_point m_timeout_started;

    Remote_Clock m_remote_clock;

    size_t m_error_count = 0;

    enum class Sensor : uint16_t
    {
        ACCELEROMETER   = 1 << 0,
        GYROSCOPE       = 1 << 1,
        COMPASS         = 1 << 2,
        BAROMETER       = 1 << 3,
        THERMOMETER     = 1 << 4,
        SONAR           = 1 << 5,
        GPS             = 1 << 6,
        VOLTAGE         = 1 << 7,
        CURRENT         = 1 << 8,
    };
    typedef q::util::Flag_Set<Sensor, uint16_t> Sensors;


    void process_message_ping();
    void process_message_pong();

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

    void process_message_uav_rotation();
    void process_message_uav_linear_acceleration();
    void process_message_uav_velocity();
    void process_message_uav_position();


    struct Sensor_Samples
    {
        std::vector<Accelerometer_Sample> accelerometer;
        std::vector<Gyroscope_Sample> gyroscope;
        std::vector<Compass_Sample> compass;
        std::vector<Barometer_Sample> barometer;
        std::vector<Sonar_Sample> sonar;
        std::vector<Thermometer_Sample> thermometer;
        std::vector<Voltage_Sample> voltage;
        std::vector<Current_Sample> current;
        std::vector<GPS_Sample> gps;
    } m_sensor_samples;

    struct UAV
    {
        UAV_Rotation_Sample rotation;
        UAV_Linear_Acceleration_Sample linear_acceleration;
        UAV_Velocity_Sample velocity;
        UAV_Position_Sample position;
    } m_uav;

    struct Ping
    {
        uint32_t seq = 0;
        std::map<uint32_t, q::Clock::time_point> seq_sent;
        std::deque<q::Clock::duration> rtts;
        q::Clock::time_point last_time_point;
    } m_ping;
};

}
