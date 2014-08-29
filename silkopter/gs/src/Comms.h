#pragma once

#include "common/input/Camera_Input.h"
#include "common/input/UAV_Input.h"
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

    void process();

    auto get_error_count() const -> size_t;
    auto get_uav_alive_duration() const -> q::Clock::duration;

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

    template<class T> struct Data
    {
        typedef T data_t;
        Data() : value() {}

        T value;
        uint32_t sample_idx = 0;
        q::Clock::time_point timestamp;
    };

    struct Accelerometer
    {
        math::vec3f acceleration; //meters per second^2
        q::Clock::duration dt;
    };

    typedef Data<Accelerometer> Accelerometer_Data;
    auto get_sensor_accelerometer_data() const -> Accelerometer_Data;


    struct Gyroscope
    {
        math::vec3f angular_velocity; //radians per second
        q::Clock::duration dt;
    };
    typedef Data<Gyroscope> Gyroscope_Data;
    auto get_sensor_gyroscope_data() const -> Gyroscope_Data;


    typedef Data<math::vec3f> Compass_Data;
    auto get_sensor_compass_data() const -> Compass_Data;


    typedef Data<float> Barometer_Data; //kp
    auto get_sensor_barometer_data() const -> Barometer_Data;

    typedef Data<float> Sonar_Data; //meters
    auto get_sensor_sonar_data() const -> Sonar_Data;

    typedef Data<float> Thermometer_Data; //degrees celsius
    auto get_sensor_thermometer_data() const -> Thermometer_Data;

    struct GPS
    {
        uint8_t fix_count = 0;
        float precision = 0;
        double latitude = 0;
        double longitude = 0;
    };
    typedef Data<GPS> GPS_Data;
    auto get_sensor_gps_data() const -> GPS_Data;

    typedef Data<math::quatf> UAV_Rotation_Data; //local to world
    auto get_uav_rotation_data() const -> UAV_Rotation_Data;

    typedef Data<math::vec3f> UAV_Linear_Acceleration_Data; //meters / second^2
    auto get_uav_linear_acceleration_data() const -> UAV_Linear_Acceleration_Data;

    typedef Data<math::vec3f> UAV_Velocity_Data; //meters / second
    auto get_uav_velocity_data() const -> UAV_Velocity_Data;

    typedef Data<math::vec3f> UAV_Position_Data; //meters
    auto get_uav_position_data() const -> UAV_Position_Data;

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
        UAV_PING,
        UAV_ROTATION, //local to world
        UAV_LINEAR_ACCELERATION,
        UAV_VELOCITY,
        UAV_POSITION
    };

    boost::asio::ip::tcp::socket m_socket;
    boost::asio::ip::tcp::endpoint m_remote_endpoint;

    typedef util::Channel<Message,
                        uint16_t,
                        boost::asio::ip::tcp::socket> Channel;

    mutable Channel m_channel;
    q::Clock::time_point m_timeout_started;

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

    void process_message_uav_ping();
    void process_message_uav_rotation();
    void process_message_uav_linear_acceleration();
    void process_message_uav_velocity();
    void process_message_uav_position();


    struct Sensor_Data
    {
        Accelerometer_Data accelerometer;
        Gyroscope_Data gyroscope;
        Compass_Data compass;
        Barometer_Data barometer;
        Thermometer_Data thermometer;
        Sonar_Data sonar;
        GPS_Data gps;
    } m_sensor_data;

    struct UAV
    {
        UAV_Rotation_Data rotation;
        UAV_Linear_Acceleration_Data linear_acceleration;
        UAV_Velocity_Data velocity;
        UAV_Position_Data position;
    } m_uav;

    q::Clock::duration m_uav_alive_duration;
};

}
