#pragma once

#include "common/input/UAV_Input.h"
#include "common/input/Camera_Input.h"
#include "common/sensors/Sensor_Samples.h"
#include "common/Comm_Data.h"
#include "HAL.h"
#include "UAV.h"

namespace silk
{

class Comms : q::util::Noncopyable
{
public:
    Comms(boost::asio::io_service& io_service, HAL& hal, UAV& uav);

    auto start_listening(uint16_t port) -> bool;
    void disconnect();

    auto is_listening() const -> bool;
    auto is_connected() const -> bool;
    auto get_remote_address() const -> boost::asio::ip::address;
    auto get_remote_clock() const -> Remote_Clock const&;

    void process();

    auto get_error_count() const -> size_t;

private:
    boost::asio::io_service& m_io_service;
    uint16_t m_port = 0;

    void handle_accept(boost::system::error_code const& error);

    void process_message_ping();
    void process_message_pong();

    void process_message_camera_input();
    void process_message_uav_input();

    void process_message_raw_sensors();

    void process_message_assist_params();

    void process_message_yaw_rate_pid_params();
    void process_message_pitch_rate_pid_params();
    void process_message_roll_rate_pid_params();
    void process_message_altitude_rate_pid_params();
    void process_message_yaw_pid_params();
    void process_message_pitch_pid_params();
    void process_message_roll_pid_params();
    void process_message_altitude_pid_params();

    void process_message_calibration_accelerometer();
    void process_message_calibration_gyroscope();
    void process_message_calibration_compass();

    void send_sensor_data();
    void send_raw_sensor_data(detail::Comm_Message_Sensors sensors);
    void store_raw_sensor_data();

    struct Sensor_Samples
    {
        Gyroscope_Sample gyroscope;
        Accelerometer_Sample accelerometer;
        Compass_Sample compass;
        Barometer_Sample barometer;
        Thermometer_Sample thermometer;
        Sonar_Sample sonar;
        Voltage_Sample voltage;
        Current_Sample current;
        GPS_Sample gps;
        q::Clock::time_point last_sent_timestamp;
    } m_sensor_samples;
    struct Raw_Sensor_Samples
    {
        static const size_t MAX_SIZE = 2048;
        boost::circular_buffer<Gyroscope_Sample> gyroscope;
        boost::circular_buffer<Accelerometer_Sample> accelerometer;
        boost::circular_buffer<Compass_Sample> compass;
        boost::circular_buffer<Barometer_Sample> barometer;
        boost::circular_buffer<Thermometer_Sample> thermometer;
        boost::circular_buffer<Sonar_Sample> sonar;
        boost::circular_buffer<Voltage_Sample> voltage;
        boost::circular_buffer<Current_Sample> current;
        boost::circular_buffer<GPS_Sample> gps;
    } m_raw_sensor_samples;

    HAL& m_hal;
    UAV& m_uav;
    q::Clock::time_point m_uav_sent_timestamp;
    void send_uav_data();

    Remote_Clock m_remote_clock;

    std::unique_ptr<boost::asio::ip::tcp::socket> m_socket;
    std::unique_ptr<boost::asio::ip::tcp::acceptor> m_acceptor;

    typedef util::Channel<detail::Comm_Message,
                        uint32_t,
                        boost::asio::ip::tcp::socket> Channel;

    std::unique_ptr<Channel> m_channel;
    q::Clock::time_point m_timeout_started;

    bool m_is_listening = false;

    size_t m_error_count = 0;

    struct Ping
    {
        uint32_t seq = 0;
        std::map<uint32_t, q::Clock::time_point> seq_sent;
        std::deque<q::Clock::duration> rtts;
        q::Clock::time_point last_time_point;
    } m_ping;

};


}
