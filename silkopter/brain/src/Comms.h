#pragma once

#include "common/node/source/IAccelerometer.h"
#include "common/node/source/IGyroscope.h"
#include "common/node/source/ICompass.h"
#include "common/node/source/IBarometer.h"
#include "common/node/source/IThermometer.h"
#include "common/node/source/ISonar.h"
#include "common/node/source/IGPS.h"
#include "common/node/source/IAmmeter.h"
#include "common/node/source/IVoltmeter.h"

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

    void recieve_uav_input();
    void recieve_camera_mount_input();
    void recieve_motor_test_input();

    void recieve_request_uav_input();
    void recieve_request_camera_mount_input();
    void recieve_request_motor_test_input();
    void recieve_operation_mode();

    void recieve_camera_params();

    void recieve_raw_sensors();

    void recieve_assist_params();

    void recieve_yaw_rate_pid_params();
    void recieve_pitch_rate_pid_params();
    void recieve_roll_rate_pid_params();
    void recieve_altitude_rate_pid_params();
    void recieve_yaw_pid_params();
    void recieve_pitch_pid_params();
    void recieve_roll_pid_params();
    void recieve_altitude_pid_params();

    void recieve_calibration_accelerometer();
    void recieve_calibration_gyroscope();
    void recieve_calibration_compass();

    void send_sensor_samples();
    void send_raw_sensor_samples(comms::Sensors sensor);

    void store_raw_sensor_samples();
    void clear_raw_sensor_samples();

//    struct Sensor_Samples
//    {
//        node::IAccelerometer::Sample accelerometer;
//        node::IGyroscope::Sample gyroscope;
//        node::ICompass::Sample compass;
//        node::IBarometer::Sample barometer;
//        node::IThermometer::Sample thermometer;
//        node::ISonar::Sample sonar;
//        node::IVoltmeter::Sample voltage;
//        node::IAmmeter::Sample current;
//        node::IGPS::Sample gps;
//        q::Clock::time_point last_sent_timestamp;
//    } m_sensor_samples;
//    struct Raw_Sensor_Samples
//    {
//        q::Clock::time_point last_sent;
//        std::vector<node::IAccelerometer::Sample> accelerometer;
//        std::vector<node::IGyroscope::Sample> gyroscope;
//        std::vector<node::ICompass::Sample> compass;
//        std::vector<node::IBarometer::Sample> barometer;
//        std::vector<node::IThermometer::Sample> thermometer;
//        std::vector<node::ISonar::Sample> sonar;
//        std::vector<node::IVoltmeter::Sample> voltage;
//        std::vector<node::IAmmeter::Sample> current;
//        std::vector<node::IGPS::Sample> gps;
//    } m_raw_sensor_samples;

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
