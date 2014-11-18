#pragma once

#include "common/input/UAV_Input.h"
#include "common/input/Camera_Input.h"
#include "common/sensors/Sensor_Samples.h"
#include "common/Comm_Data.h"
#include "HAL.h"
#include "UAV.h"
#include "utils/Channel.h"
#include "utils/RUDP.h"

namespace silk
{

class Comms : q::util::Noncopyable
{
public:
    Comms(boost::asio::io_service& io_service, HAL& hal, UAV& uav);

    auto start(uint16_t send_port, uint16_t receive_port) -> bool;

    auto is_connected() const -> bool;
    auto get_remote_address() const -> boost::asio::ip::address;
    auto get_remote_clock() const -> Manual_Clock const&;

    auto get_rudp() -> util::RUDP&;

    void process();

    auto get_error_count() const -> size_t;

private:
    boost::asio::io_service& m_io_service;

    void handle_accept(boost::system::error_code const& error);

//    void process_message_ping();
//    void process_message_pong();

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

    void send_sensor_samples();
    void send_raw_sensor_samples(detail::Comm_Message_Sensors sensors);

    void store_raw_sensor_samples();
    void clear_raw_sensor_samples();

    struct Sensor_Samples
    {
        Accelerometer_Sample accelerometer;
        Gyroscope_Sample gyroscope;
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
        q::Clock::time_point last_sent;
        std::vector<Accelerometer_Sample> accelerometer;
        std::vector<Gyroscope_Sample> gyroscope;
        std::vector<Compass_Sample> compass;
        std::vector<Barometer_Sample> barometer;
        std::vector<Thermometer_Sample> thermometer;
        std::vector<Sonar_Sample> sonar;
        std::vector<Voltage_Sample> voltage;
        std::vector<Current_Sample> current;
        std::vector<GPS_Sample> gps;
    } m_raw_sensor_samples;

    HAL& m_hal;
    UAV& m_uav;
    q::Clock::time_point m_uav_sent_timestamp;
    void send_uav_data();

    Manual_Clock m_remote_clock;

//    std::unique_ptr<boost::asio::ip::tcp::socket> m_socket;
//    std::unique_ptr<boost::asio::ip::tcp::acceptor> m_acceptor;

    uint16_t m_send_port = 0;
    uint16_t m_receive_port = 0;
    boost::asio::ip::udp::socket m_socket;
    util::RUDP m_rudp;

    typedef util::Channel<detail::Comm_Message, uint16_t> Channel;
    Channel m_channel;
//    q::Clock::time_point m_timeout_started;

    bool m_is_connected = false;

    size_t m_error_count = 0;

//    struct Ping
//    {
//        uint32_t seq = 0;
//        typedef std::vector<std::pair<uint32_t, q::Clock::time_point>> Seq_Sent;
//        Seq_Sent seq_sent;
//        boost::circular_buffer<q::Clock::duration> rtts;
//        q::Clock::time_point last_time_point;
//    } m_ping;

};


}
