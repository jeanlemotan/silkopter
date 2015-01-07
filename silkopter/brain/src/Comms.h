#pragma once

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

    enum class Video_Flag : uint8_t
    {
        FLAG_KEYFRAME = 1 << 0,
    };
    typedef q::util::Flag_Set<Video_Flag, uint8_t> Video_Flags;
    //sends a video frame.
    //The data needs to be alive only for the duration of this call.
    auto send_video_frame(Video_Flags flags, uint8_t const* data, size_t size) -> bool;

private:
    boost::asio::io_service& m_io_service;

    void handle_accept(boost::system::error_code const& error);

    void process_message_uav_input();
    void process_message_camera_mount_input();


    void process_message_camera_params();

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
    void send_raw_sensor_samples(comms::Sensors sensors);

    void store_raw_sensor_samples();
    void clear_raw_sensor_samples();

    struct Sensor_Samples
    {
        sensors::Accelerometer_Sample accelerometer;
        sensors::Gyroscope_Sample gyroscope;
        sensors::Compass_Sample compass;
        sensors::Barometer_Sample barometer;
        sensors::Thermometer_Sample thermometer;
        sensors::Sonar_Sample sonar;
        sensors::Voltage_Sample voltage;
        sensors::Current_Sample current;
        sensors::GPS_Sample gps;
        q::Clock::time_point last_sent_timestamp;
    } m_sensor_samples;
    struct Raw_Sensor_Samples
    {
        q::Clock::time_point last_sent;
        std::vector<sensors::Accelerometer_Sample> accelerometer;
        std::vector<sensors::Gyroscope_Sample> gyroscope;
        std::vector<sensors::Compass_Sample> compass;
        std::vector<sensors::Barometer_Sample> barometer;
        std::vector<sensors::Thermometer_Sample> thermometer;
        std::vector<sensors::Sonar_Sample> sonar;
        std::vector<sensors::Voltage_Sample> voltage;
        std::vector<sensors::Current_Sample> current;
        std::vector<sensors::GPS_Sample> gps;
    } m_raw_sensor_samples;

    HAL& m_hal;
    UAV& m_uav;
    q::Clock::time_point m_uav_sent_time_point = q::Clock::now();
    void send_uav_data();

    Manual_Clock m_remote_clock;

    q::Clock::time_point m_last_rudp_time_stamp = q::Clock::now();

    uint16_t m_send_port = 0;
    uint16_t m_receive_port = 0;
    boost::asio::ip::udp::socket m_socket;
    util::RUDP m_rudp;

    typedef util::Channel<comms::Setup_Message, uint16_t> Setup_Channel;
    typedef util::Channel<comms::Input_Message, uint16_t> Input_Channel;
    typedef util::Channel<comms::Telemetry_Message, uint16_t> Telemetry_Channel;
    Setup_Channel m_setup_channel;
    Input_Channel m_input_channel;
    Telemetry_Channel m_telemetry_channel;

    bool m_is_connected = false;

    size_t m_error_count = 0;
};


}
