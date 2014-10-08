#pragma once

#include "common/input/UAV_Input.h"
#include "common/input/Camera_Input.h"
#include "HAL.h"
#include "UAV.h"

namespace silk
{

class Comms : q::util::Noncopyable
{
public:
    Comms(boost::asio::io_service& io_service, HAL& hal, UAV& uav);

    enum class Result
    {
        OK,
        FAILED,
    };

    auto start_listening(uint16_t port) -> Result;
    void disconnect();

    auto is_listening() const -> bool;
    auto is_connected() const -> bool;
    auto get_remote_address() const -> boost::asio::ip::address;

    void process();

    auto get_error_count() const -> size_t;

private:
    boost::asio::io_service& m_io_service;
    uint16_t m_port = 0;

    void handle_accept(boost::system::error_code const& error);

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

    void message_camera_input();
    void message_uav_input();

    void message_assist_params();

    void message_yaw_rate_pid_params();
    void message_pitch_rate_pid_params();
    void message_roll_rate_pid_params();
    void message_altitude_rate_pid_params();
    void message_yaw_pid_params();
    void message_pitch_pid_params();
    void message_roll_pid_params();
    void message_altitude_pid_params();

    void message_calibration_accelerometer();
    void message_calibration_gyroscope();
    void message_calibration_compass();

    void send_sensor_data();
    struct Sensor_Samples
    {
        std::vector<HAL_Sensors::Gyroscope_Sample> gyroscope;
        std::vector<HAL_Sensors::Accelerometer_Sample> accelerometer;
        std::vector<HAL_Sensors::Compass_Sample> compass;
        std::vector<HAL_Sensors::Barometer_Sample> barometer;
        std::vector<HAL_Sensors::Thermometer_Sample> thermometer;
        std::vector<HAL_Sensors::Sonar_Sample> sonar;
        std::vector<HAL_Sensors::Voltage_Sample> voltage;
        std::vector<HAL_Sensors::Current_Sample> current;
        std::vector<HAL_Sensors::GPS_Sample> gps;
        q::Clock::time_point last_sent_timestamp;
    } m_sensors_samples;

    HAL& m_hal;
    UAV& m_uav;
    q::Clock::time_point m_uav_sent_timestamp;
    void send_uav_data();

    std::unique_ptr<boost::asio::ip::tcp::socket> m_socket;
    std::unique_ptr<boost::asio::ip::tcp::acceptor> m_acceptor;

    typedef util::Channel<Message,
                        uint32_t,
                        boost::asio::ip::tcp::socket> Channel;

    std::unique_ptr<Channel> m_channel;
    q::Clock::time_point m_timeout_started;

    bool m_is_listening = false;

    size_t m_error_count = 0;

    q::Clock::time_point m_ping_time;
    uint32_t m_alive_time_ms = 0;
};


}
