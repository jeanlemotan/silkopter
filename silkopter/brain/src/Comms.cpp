#include "BrainStdAfx.h"
#include "Comms.h"
#include "utils/Timed_Scope.h"

#include "common/node/stream/IAcceleration.h"
#include "common/node/stream/IAngular_Velocity.h"
#include "common/node/stream/IADC_Value.h"
#include "common/node/stream/IBattery_State.h"
#include "common/node/stream/ICardinal_Points.h"
#include "common/node/stream/ICurrent.h"
#include "common/node/stream/IDistance.h"
#include "common/node/stream/ILinear_Acceleration.h"
#include "common/node/stream/ILocation.h"
#include "common/node/stream/IMagnetic_Field.h"
#include "common/node/stream/IPressure.h"
#include "common/node/stream/IPWM_Value.h"
#include "common/node/stream/IReference_Frame.h"
#include "common/node/stream/ITemperature.h"
#include "common/node/stream/IVideo.h"
#include "common/node/stream/IVoltage.h"


using namespace silk;
using namespace boost::asio;

constexpr uint8_t SETUP_CHANNEL = 10;
constexpr uint8_t INPUT_CHANNEL = 15;
constexpr uint8_t TELEMETRY_CHANNEL = 20;
constexpr uint8_t VIDEO_CHANNEL = 4;
constexpr uint8_t SIMULATOR_CHANNEL = 30;

constexpr q::Clock::duration RUDP_PERIOD = std::chrono::milliseconds(30);

Comms::Comms(boost::asio::io_service& io_service, HAL& hal)
    : m_io_service(io_service)
    , m_hal(hal)
    , m_socket(io_service)
    , m_rudp(m_socket)
    , m_setup_channel(m_rudp, SETUP_CHANNEL)
    , m_input_channel(m_rudp, INPUT_CHANNEL)
    , m_telemetry_channel(m_rudp, TELEMETRY_CHANNEL)
{
    {
        util::RUDP::Send_Params params;
        params.mtu = 100;
        params.is_compressed = true;
        params.is_reliable = true;
        params.importance = 126;
        m_rudp.set_send_params(SETUP_CHANNEL, params);
    }
    {
        util::RUDP::Send_Params params;
        params.mtu = 100;
        params.is_compressed = true;
        params.is_reliable = false;
        params.importance = 127;
        m_rudp.set_send_params(INPUT_CHANNEL, params);
    }

    {
        util::RUDP::Send_Params params;
        params.mtu = 100;
        params.is_compressed = true;
        params.is_reliable = true;
        params.importance = 127;
        m_rudp.set_send_params(SIMULATOR_CHANNEL, params);
    }

    {
        util::RUDP::Send_Params params;
        params.mtu = 8192;
        params.is_compressed = true;
        params.is_reliable = false;
        params.importance = 0;
        params.cancel_on_new_data = true;
        params.cancel_after = std::chrono::milliseconds(200);
        m_rudp.set_send_params(TELEMETRY_CHANNEL, params);
    }

    {
        util::RUDP::Send_Params params;
        params.mtu = 16000;
        params.is_compressed = false;
        params.is_reliable = false;
        params.importance = 10;
//        params.cancel_on_new_data = true;
        params.cancel_after = std::chrono::milliseconds(100);
        m_rudp.set_send_params(VIDEO_CHANNEL, params);
    }

    {
        util::RUDP::Receive_Params params;
        params.max_receive_time = std::chrono::seconds(999999);
        m_rudp.set_receive_params(SETUP_CHANNEL, params);
    }
    {
        util::RUDP::Receive_Params params;
        params.max_receive_time = std::chrono::milliseconds(100);
        m_rudp.set_receive_params(INPUT_CHANNEL, params);
    }

    {
        util::RUDP::Receive_Params params;
        params.max_receive_time = std::chrono::seconds(999999);
        m_rudp.set_receive_params(SIMULATOR_CHANNEL, params);
    }

    {
        util::RUDP::Receive_Params params;
        params.max_receive_time = std::chrono::milliseconds(500);
        m_rudp.set_receive_params(TELEMETRY_CHANNEL, params);
    }
}

auto Comms::start(uint16_t send_port, uint16_t receive_port) -> bool
{
    try
    {
        m_socket.open(ip::udp::v4());
        m_socket.set_option(ip::udp::socket::reuse_address(true));
        //m_socket.set_option(socket_base::send_buffer_size(65536));
        m_socket.bind(ip::udp::endpoint(ip::udp::v4(), receive_port));
        //m_rudp.set_send_endpoint(ip::udp::endpoint(ip::address::from_string("192.168.1.37"), send_port));

        m_rudp.start();
    }
    catch(std::exception e)
    {
        QLOGW("Cannot start comms on ports s:{} r:{}", send_port, receive_port);
        return false;
    }

    m_send_port = send_port;
    m_receive_port = receive_port;

    m_is_connected = true;
    QLOGI("Started sending on ports s:{} r:{}", send_port, receive_port);

    return true;
}

auto Comms::is_connected() const -> bool
{
    return m_is_connected;
}

auto Comms::get_remote_address() const -> boost::asio::ip::address
{
    return m_rudp.get_send_endpoint().address();
}

auto Comms::get_remote_clock() const -> Manual_Clock const&
{
    return m_remote_clock;
}

//void Comms::recieve_camera_params()
//{
//    //comms::Camera_Params v;
//    //if (!m_setup_channel.unpack(v))
//    {
//        return;
//    }

//    if (m_hal.camera)
//    {
//        m_hal.camera->set_iso(v.iso);
//        m_hal.camera->set_shutter_speed(v.shutter_speed);
//        m_hal.camera->set_stream_quality(v.stream_quality);
//        if (v.toggles.record)
//        {
//            m_hal.camera->start_recording();
//        }
//        else
//        {
//            m_hal.camera->stop_recording();
//        }
//    }
    
//    QLOGI("Camera params received");
//}
//void Comms::recieve_uav_input()
//{
//    comms::UAV_Input v;
//    if (m_input_channel.unpack(v))
//    {
//        m_uav.set_uav_input(v);
//    }
//}
//void Comms::recieve_camera_mount_input()
//{
//    comms::Camera_Mount_Input v;
//    if (m_input_channel.unpack(v))
//    {
//        m_uav.set_camera_mount_input(v);
//    }
//}
//void Comms::recieve_motor_test_input()
//{
//    comms::Motor_Test_Input v;
//    if (m_input_channel.unpack(v))
//    {
//        m_uav.set_motor_test_input(v);
//    }
//}

//void Comms::recieve_request_uav_input()
//{
//    m_setup_channel.pack(comms::Setup_Message::REQUEST_UAV_INPUT, m_uav.get_uav_input());
//}
//void Comms::recieve_request_camera_mount_input()
//{
//    m_setup_channel.pack(comms::Setup_Message::REQUEST_CAMERA_MOUNT_INPUT, m_uav.get_camera_mount_input());
//}
//void Comms::recieve_request_motor_test_input()
//{
//    m_setup_channel.pack(comms::Setup_Message::REQUEST_MOTOR_TEST_INPUT, m_uav.get_motor_test_input());
//}
//void Comms::recieve_operation_mode()
//{
//    comms::Mode v;
//    if (m_setup_channel.get_message_size() == 0)
//    {
//        QLOGI("operation mode get");
//        m_setup_channel.pack(comms::Setup_Message::OPERATION_MODE, m_uav.get_operation_mode());
//    }
//    else if (m_setup_channel.unpack(v))
//    {
//        auto old = m_uav.get_operation_mode();
//        m_uav.set_mode(v);
//        QLOGI("operation mode changed from {} to {}/{}", static_cast<int>(old), static_cast<int>(v), static_cast<int>(m_uav.get_operation_mode()));
//    }
//}

//void Comms::recieve_yaw_rate_pid_params()
//{
//    auto params = m_uav.get_yaw_rate_pid_params();
//    if (m_setup_channel.get_message_size() == 0)
//    {
//        QLOGI("yaw rate pid params get");
//        m_setup_channel.pack(comms::Setup_Message::YAW_RATE_PID_PARAMS, params);
//    }
//    else if (m_setup_channel.unpack(params))
//    {
//        QLOGI("yaw rate pid params changed");
//        m_uav.set_yaw_rate_pid_params(params);
//    }
//}
//void Comms::recieve_pitch_rate_pid_params()
//{
//    auto params = m_uav.get_pitch_rate_pid_params();
//    if (m_setup_channel.get_message_size() == 0)
//    {
//        QLOGI("pitch rate pid params get");
//        m_setup_channel.pack(comms::Setup_Message::PITCH_RATE_PID_PARAMS, params);
//    }
//    else if (m_setup_channel.unpack(params))
//    {
//        QLOGI("pitch rate pid params changed");
//        m_uav.set_pitch_rate_pid_params(params);
//    }
//}
//void Comms::recieve_roll_rate_pid_params()
//{
//    auto params = m_uav.get_roll_rate_pid_params();
//    if (m_setup_channel.get_message_size() == 0)
//    {
//        QLOGI("roll rate pid params get");
//        m_setup_channel.pack(comms::Setup_Message::ROLL_RATE_PID_PARAMS, params);
//    }
//    else if (m_setup_channel.unpack(params))
//    {
//        QLOGI("roll rate pid params changed");
//        m_uav.set_roll_rate_pid_params(params);
//    }
//}
//void Comms::recieve_altitude_rate_pid_params()
//{
//    auto params = m_uav.get_altitude_rate_pid_params();
//    if (m_setup_channel.get_message_size() == 0)
//    {
//        QLOGI("altitude rate pid params get");
//        m_setup_channel.pack(comms::Setup_Message::ALTITUDE_RATE_PID_PARAMS, params);
//    }
//    else if (m_setup_channel.unpack(params))
//    {
//        QLOGI("altitude rate pid params changed");
//        m_uav.set_altitude_rate_pid_params(params);
//    }
//}
//void Comms::recieve_yaw_pid_params()
//{
//    auto params = m_uav.get_yaw_pid_params();
//    if (m_setup_channel.get_message_size() == 0)
//    {
//        QLOGI("yaw pid params get");
//        m_setup_channel.pack(comms::Setup_Message::YAW_PID_PARAMS, params);
//    }
//    else if (m_setup_channel.unpack(params))
//    {
//        QLOGI("yaw pid params changed");
//        m_uav.set_yaw_pid_params(params);
//    }
//}
//void Comms::recieve_pitch_pid_params()
//{
//    auto params = m_uav.get_pitch_pid_params();
//    if (m_setup_channel.get_message_size() == 0)
//    {
//        QLOGI("pitch pid params get");
//        m_setup_channel.pack(comms::Setup_Message::PITCH_PID_PARAMS, params);
//    }
//    else if (m_setup_channel.unpack(params))
//    {
//        QLOGI("pitch pid params changed");
//        m_uav.set_pitch_pid_params(params);
//    }
//}
//void Comms::recieve_roll_pid_params()
//{
//    auto params = m_uav.get_roll_pid_params();
//    if (m_setup_channel.get_message_size() == 0)
//    {
//        QLOGI("roll pid params get");
//        m_setup_channel.pack(comms::Setup_Message::ROLL_PID_PARAMS, params);
//    }
//    else if (m_setup_channel.unpack(params))
//    {
//        QLOGI("roll pid params changed");
//        m_uav.set_roll_pid_params(params);
//    }
//}
//void Comms::recieve_altitude_pid_params()
//{
//    auto params = m_uav.get_altitude_pid_params();
//    if (m_setup_channel.get_message_size() == 0)
//    {
//        QLOGI("altitude pid params get");
//        m_setup_channel.pack(comms::Setup_Message::ALTITUDE_PID_PARAMS, params);
//    }
//    else if (m_setup_channel.unpack(params))
//    {
//        QLOGI("altitude pid params changed");
//        m_uav.set_altitude_pid_params(params);
//    }
//}

//void Comms::recieve_assist_params()
//{
//    auto params = m_uav.get_assist_params();
//    if (m_setup_channel.get_message_size() == 0)
//    {
//        QLOGI("assist params get");
//        m_setup_channel.pack(comms::Setup_Message::ASSIST_PARAMS, params);
//    }
//    else if (m_setup_channel.unpack(params))
//    {
//        QLOGI("assist params changed");
//        m_uav.set_assist_params(params);
//    }
//}

//void Comms::recieve_raw_sensors()
//{
//    QASSERT(m_hal.sensors);
//    comms::Sensors sensors;
//    if (m_setup_channel.unpack(sensors))
//    {
//        send_raw_sensor_samples(sensors);
//    }
//}


//void Comms::recieve_calibration_accelerometer()
//{
//    QASSERT(m_hal.sensors);
//    math::vec3f bias, scale;
//    if (m_setup_channel.get_message_size() == 0)
//    {
//        QLOGI("Accelerometer calibration set req");
//        m_hal.sensors->get_accelerometer_calibration_data(bias, scale);
//        m_setup_channel.pack(comms::Setup_Message::CALIBRATION_ACCELEROMETER, bias, scale);
//    }
//    else if (m_setup_channel.unpack(bias, scale))
//    {
//        QLOGI("Accelerometer calibration changed");
//        m_hal.sensors->set_accelerometer_calibration_data(bias, scale);
//        m_setup_channel.pack(comms::Setup_Message::CALIBRATION_ACCELEROMETER, bias, scale);
//    }
//}
//void Comms::recieve_calibration_gyroscope()
//{
//    QASSERT(m_hal.sensors);
//    math::vec3f bias;
//    if (m_setup_channel.get_message_size() == 0)
//    {
//        QLOGI("Gyroscope calibration set req");
//        m_hal.sensors->get_gyroscope_calibration_data(bias);
//        m_setup_channel.pack(comms::Setup_Message::CALIBRATION_GYROSCOPE, bias);
//    }
//    else if (m_setup_channel.unpack(bias))
//    {
//        QLOGI("Gyroscope calibration changed");
//        m_hal.sensors->set_gyroscope_calibration_data(bias);
//        m_setup_channel.pack(comms::Setup_Message::CALIBRATION_GYROSCOPE, bias);
//    }
//}
//void Comms::recieve_calibration_compass()
//{
//    math::vec3f bias;
//    if (m_setup_channel.get_message_size() == 0)
//    {
//        QLOGI("Compass calibration req, sending to GS");
//        m_hal.sensors->get_compass_calibration_data(bias);
//        m_setup_channel.pack(comms::Setup_Message::CALIBRATION_COMPASS, bias);
//    }
//    else if (m_setup_channel.unpack(bias))
//    {
//        QLOGI("Compass calibration changed");
//        m_hal.sensors->set_compass_calibration_data(bias);
//        m_setup_channel.pack(comms::Setup_Message::CALIBRATION_COMPASS, bias);
//    }
//}

//void Comms::send_raw_sensor_samples(comms::Sensors sensors)
//{
//    QASSERT(m_hal.sensors);
//    if (!sensors.any())
//    {
//        return;
//    }

//    m_telemetry_channel.begin_pack(comms::Telemetry_Message::SENSORS);

//    m_telemetry_channel.pack_param(sensors);

//    if (sensors.test(comms::Sensor::ACCELEROMETER))
//    {
//        m_telemetry_channel.pack_param(static_cast<uint32_t>(m_raw_sensor_samples.accelerometer.size()));
//        for (auto& a: m_raw_sensor_samples.accelerometer) { m_telemetry_channel.pack_param(a); }
//    }
//    if (sensors.test(comms::Sensor::GYROSCOPE))
//    {
//        m_telemetry_channel.pack_param(static_cast<uint32_t>(m_raw_sensor_samples.gyroscope.size()));
//        for (auto& a: m_raw_sensor_samples.gyroscope) { m_telemetry_channel.pack_param(a); }
//    }
//    if (sensors.test(comms::Sensor::COMPASS))
//    {
//        m_telemetry_channel.pack_param(static_cast<uint32_t>(m_raw_sensor_samples.compass.size()));
//        for (auto& a: m_raw_sensor_samples.compass) { m_telemetry_channel.pack_param(a); }
//    }
//    if (sensors.test(comms::Sensor::BAROMETER))
//    {
//        m_telemetry_channel.pack_param(static_cast<uint32_t>(m_raw_sensor_samples.barometer.size()));
//        for (auto& a: m_raw_sensor_samples.barometer) { m_telemetry_channel.pack_param(a); }
//    }
//    if (sensors.test(comms::Sensor::THERMOMETER))
//    {
//        m_telemetry_channel.pack_param(static_cast<uint32_t>(m_raw_sensor_samples.thermometer.size()));
//        for (auto& a: m_raw_sensor_samples.thermometer) { m_telemetry_channel.pack_param(a); }
//    }
//    if (sensors.test(comms::Sensor::SONAR))
//    {
//        m_telemetry_channel.pack_param(static_cast<uint32_t>(m_raw_sensor_samples.sonar.size()));
//        for (auto& a: m_raw_sensor_samples.sonar) { m_telemetry_channel.pack_param(a); }
//    }
//    if (sensors.test(comms::Sensor::VOLTAGE))
//    {
//        m_telemetry_channel.pack_param(static_cast<uint32_t>(m_raw_sensor_samples.voltage.size()));
//        for (auto& a: m_raw_sensor_samples.voltage) { m_telemetry_channel.pack_param(a); }
//    }
//    if (sensors.test(comms::Sensor::CURRENT))
//    {
//        m_telemetry_channel.pack_param(static_cast<uint32_t>(m_raw_sensor_samples.current.size()));
//        for (auto& a: m_raw_sensor_samples.current) { m_telemetry_channel.pack_param(a); }
//    }
//    if (sensors.test(comms::Sensor::GPS))
//    {
//        m_telemetry_channel.pack_param(static_cast<uint32_t>(m_raw_sensor_samples.gps.size()));
//        for (auto& a: m_raw_sensor_samples.gps) { m_telemetry_channel.pack_param(a); }
//    }

//    m_telemetry_channel.end_pack();

//    m_raw_sensor_samples.last_sent = q::Clock::now();
//    clear_raw_sensor_samples();
//}

//void Comms::clear_raw_sensor_samples()
//{
//    m_raw_sensor_samples.accelerometer.clear();
//    m_raw_sensor_samples.gyroscope.clear();
//    m_raw_sensor_samples.compass.clear();
//    m_raw_sensor_samples.barometer.clear();
//    m_raw_sensor_samples.thermometer.clear();
//    m_raw_sensor_samples.sonar.clear();
//    m_raw_sensor_samples.voltage.clear();
//    m_raw_sensor_samples.current.clear();
//    m_raw_sensor_samples.gps.clear();
//}

//void Comms::store_raw_sensor_samples()
//{
//    constexpr std::chrono::seconds MAX_DURATION{4};

//    auto now = q::Clock::now();
//    if (now - m_raw_sensor_samples.last_sent >= MAX_DURATION)
//    {
//        clear_raw_sensor_samples();
//        return;
//    }

//    {
//        auto const& samples = m_hal.sensors->get_accelerometer_samples();
//        std::copy(samples.begin(), samples.end(), std::back_inserter(m_raw_sensor_samples.accelerometer));
//    }
//    {
//        auto const& samples = m_hal.sensors->get_gyroscope_samples();
//        std::copy(samples.begin(), samples.end(), std::back_inserter(m_raw_sensor_samples.gyroscope));
//    }
//    {
//        auto const& samples = m_hal.sensors->get_compass_samples();
//        std::copy(samples.begin(), samples.end(), std::back_inserter(m_raw_sensor_samples.compass));
//    }
//    {
//        auto const& samples = m_hal.sensors->get_barometer_samples();
//        std::copy(samples.begin(), samples.end(), std::back_inserter(m_raw_sensor_samples.barometer));
//    }
//    {
//        auto const& samples = m_hal.sensors->get_thermometer_samples();
//        std::copy(samples.begin(), samples.end(), std::back_inserter(m_raw_sensor_samples.thermometer));
//    }
//    {
//        auto const& samples = m_hal.sensors->get_sonar_samples();
//        std::copy(samples.begin(), samples.end(), std::back_inserter(m_raw_sensor_samples.sonar));
//    }
//    {
//        auto const& samples = m_hal.sensors->get_voltage_samples();
//        std::copy(samples.begin(), samples.end(), std::back_inserter(m_raw_sensor_samples.voltage));
//    }
//    {
//        auto const& samples = m_hal.sensors->get_ammeter_samples();
//        std::copy(samples.begin(), samples.end(), std::back_inserter(m_raw_sensor_samples.current));
//    }
//    {
//        auto const& samples = m_hal.sensors->get_gps_samples();
//        std::copy(samples.begin(), samples.end(), std::back_inserter(m_raw_sensor_samples.gps));
//    }

//}


//void Comms::send_sensor_samples()
//{
//    QASSERT(m_hal.sensors);
//    auto now = q::Clock::now();
//    auto delay = std::chrono::milliseconds(30);

//    //LOG_INFO("acc: {}", m_sensors_samples.accelerometer.size());
//    if (now - m_sensor_samples.last_sent_timestamp < delay)
//    {
//        return;
//    }

//    comms::Sensors sensors;

//    {
//        static q::util::Rand rnd;
//        auto sample = m_hal.sensors->get_last_accelerometer_sample();
//        if (sample.sample_idx != m_sensor_samples.accelerometer.sample_idx)
//        {
//            sensors.set(comms::Sensor::ACCELEROMETER);
////            sample.value.set(rnd.get_float(), rnd.get_float(), rnd.get_float());
//            m_sensor_samples.accelerometer = sample;
//        }
//    }
//    {
//        auto const& sample = m_hal.sensors->get_last_gyroscope_sample();
//        if (sample.sample_idx != m_sensor_samples.gyroscope.sample_idx)
//        {
//            sensors.set(comms::Sensor::GYROSCOPE);
//            m_sensor_samples.gyroscope = sample;
//        }
//    }
//    {
//        auto const& sample = m_hal.sensors->get_last_compass_sample();
//        if (sample.sample_idx != m_sensor_samples.compass.sample_idx)
//        {
//            sensors.set(comms::Sensor::COMPASS);
//            m_sensor_samples.compass = sample;
//        }
//    }
//    {
//        auto const& sample = m_hal.sensors->get_last_barometer_sample();
//        if (sample.sample_idx != m_sensor_samples.barometer.sample_idx)
//        {
//            sensors.set(comms::Sensor::BAROMETER);
//            m_sensor_samples.barometer = sample;
//        }
//    }
//    {
//        auto const& sample = m_hal.sensors->get_last_thermometer_sample();
//        if (sample.sample_idx != m_sensor_samples.thermometer.sample_idx)
//        {
//            sensors.set(comms::Sensor::THERMOMETER);
//            m_sensor_samples.thermometer = sample;
//        }
//    }
//    {
//        auto const& sample = m_hal.sensors->get_last_sonar_sample();
//        if (sample.sample_idx != m_sensor_samples.sonar.sample_idx)
//        {
//            sensors.set(comms::Sensor::SONAR);
//            m_sensor_samples.sonar = sample;
//        }
//    }
//    {
//        auto const& sample = m_hal.sensors->get_last_voltage_sample();
//        if (sample.sample_idx != m_sensor_samples.voltage.sample_idx)
//        {
//            sensors.set(comms::Sensor::VOLTAGE);
//            m_sensor_samples.voltage = sample;
//        }
//    }
//    {
//        auto const& sample = m_hal.sensors->get_last_ammeter_sample();
//        if (sample.sample_idx != m_sensor_samples.current.sample_idx)
//        {
//            sensors.set(comms::Sensor::CURRENT);
//            m_sensor_samples.current = sample;
//        }
//    }
//    {
//        auto const& sample = m_hal.sensors->get_last_gps_sample();
//        if (sample.sample_idx != m_sensor_samples.gps.sample_idx)
//        {
//            sensors.set(comms::Sensor::GPS);
//            m_sensor_samples.gps = sample;
//        }
//    }

//    if (!sensors.any())
//    {
//        return;
//    }

//    m_telemetry_channel.begin_pack(comms::Telemetry_Message::SENSORS);
//    m_telemetry_channel.pack_param(sensors);

//    if (sensors.test(comms::Sensor::ACCELEROMETER))
//    {
//        m_telemetry_channel.pack_param(m_sensor_samples.accelerometer);
//    }
//    if (sensors.test(comms::Sensor::GYROSCOPE))
//    {
//        m_telemetry_channel.pack_param(m_sensor_samples.gyroscope);
//    }
//    if (sensors.test(comms::Sensor::COMPASS))
//    {
//        m_telemetry_channel.pack_param(m_sensor_samples.compass);
//    }
//    if (sensors.test(comms::Sensor::BAROMETER))
//    {
//        m_telemetry_channel.pack_param(m_sensor_samples.barometer);
//    }
//    if (sensors.test(comms::Sensor::THERMOMETER))
//    {
//        m_telemetry_channel.pack_param(m_sensor_samples.thermometer);
//    }
//    if (sensors.test(comms::Sensor::SONAR))
//    {
//        m_telemetry_channel.pack_param(m_sensor_samples.sonar);
//    }
//    if (sensors.test(comms::Sensor::VOLTAGE))
//    {
//        m_telemetry_channel.pack_param(m_sensor_samples.voltage);
//    }
//    if (sensors.test(comms::Sensor::CURRENT))
//    {
//        m_telemetry_channel.pack_param(m_sensor_samples.current);
//    }
//    if (sensors.test(comms::Sensor::GPS))
//    {
//        m_telemetry_channel.pack_param(m_sensor_samples.gps);
//    }

//    m_telemetry_channel.end_pack();

//    m_sensor_samples.last_sent_timestamp = now;
//}


//void Comms::send_uav_data()
//{
//    auto now = q::Clock::now();
//    auto delay = std::chrono::milliseconds(30);

//    if (now - m_uav_sent_time_point < delay)
//    {
//        return;
//    }
//    m_uav_sent_time_point = now;

//    m_telemetry_channel.pack(comms::Telemetry_Message::UAV_AHRS_ROTATION_L2W, m_uav.get_ahrs().get_quat_l2w());
//    m_telemetry_channel.pack(comms::Telemetry_Message::UAV_LINEAR_ACCELERATION_W, m_uav.get_linear_acceleration_w());
//    m_telemetry_channel.pack(comms::Telemetry_Message::UAV_VELOCITY_W, m_uav.get_velocity_w());
//    m_telemetry_channel.pack(comms::Telemetry_Message::UAV_POSITION_W, m_uav.get_position_w());
//    m_telemetry_channel.pack(comms::Telemetry_Message::UAV_POSITION_W, m_uav.get_position_w());

//    m_telemetry_channel.pack(comms::Telemetry_Message::UAV_BATTERY_CAPACITY_USED, m_uav.get_battery().get_capacity_used());
//    {
//        auto avg = m_uav.get_battery().get_average_current();
//        m_telemetry_channel.pack(comms::Telemetry_Message::UAV_BATTERY_CURRENT, avg.is_initialized(), avg.get_value_or(0.f));
//    }
//    {
//        auto avg = m_uav.get_battery().get_average_voltage();
//        m_telemetry_channel.pack(comms::Telemetry_Message::UAV_BATTERY_VOLTAGE, avg.is_initialized(), avg.get_value_or(0.f));
//    }
//}

//auto Comms::send_video_frame(Video_Flags flags, uint8_t const* data, size_t size) -> bool
//{
//    if (size == 0 || !data)
//    {
//        return true;
//    }
//    return m_rudp.try_sending(VIDEO_CHANNEL, data, size);
//}

template<class Stream> auto Comms::send_telemetry_stream(node::stream::IStream const& _stream) -> bool
{
    if (q::rtti::is_of_type<Stream>(_stream))
    {
        auto const& stream = static_cast<Stream const&>(_stream);
        auto const& samples = stream.get_samples();
        m_telemetry_channel.begin_pack(q::rtti::get_class_id(stream));
        m_telemetry_channel.pack_param(static_cast<uint32_t>(samples.size()));
        for (auto const& s: samples)
        {
            m_telemetry_channel.pack_param(s.value);
            m_telemetry_channel.pack_param(s.sample_idx);
            m_telemetry_channel.pack_param(static_cast<uint32_t>(std::chrono::duration_cast<std::chrono::microseconds>(s.dt).count()));
            m_telemetry_channel.pack_param(s.is_healthy);
        }
        m_telemetry_channel.end_pack();
        return true;
    }
    return false;
}

void Comms::send_telemetry_streams()
{
    for (auto const& stream: m_telemetry_streams)
    {
        QASSERT(stream);
        if (send_telemetry_stream<node::stream::IAcceleration>(*stream) ||
            send_telemetry_stream<node::stream::IAngular_Velocity>(*stream) ||
            send_telemetry_stream<node::stream::IMagnetic_Field>(*stream) ||
            send_telemetry_stream<node::stream::IPressure>(*stream) ||
            send_telemetry_stream<node::stream::IBattery_State>(*stream) ||
            send_telemetry_stream<node::stream::ILinear_Acceleration>(*stream) ||
            send_telemetry_stream<node::stream::ICardinal_Points>(*stream) ||
            send_telemetry_stream<node::stream::ICurrent>(*stream) ||
            send_telemetry_stream<node::stream::IVoltage>(*stream) ||
            send_telemetry_stream<node::stream::IDistance>(*stream) ||
            send_telemetry_stream<node::stream::ILocation>(*stream) ||
            send_telemetry_stream<node::stream::IPWM_Value>(*stream) ||
            send_telemetry_stream<node::stream::IReference_Frame>(*stream) ||
            send_telemetry_stream<node::stream::ITemperature>(*stream) ||
            send_telemetry_stream<node::stream::IADC_Value>(*stream)
//          send_telemetry_stream<node::stream::IVideo>(*stream)
                )
        {
            ;//nothing
        }
        else
        {
            QLOGW("Unrecognized stream type: {} / {}", stream->get_name(), q::rtti::get_class_name(*stream));
        }
    }
}





void Comms::handle_enumerate_sources()
{
    uint32_t req_id = 0;
    if (m_setup_channel.unpack(req_id))
    {
        QLOGI("Req Id: {} - enumerate sources", req_id);
        auto const& all = m_hal.get_sources().get_all();

        m_setup_channel.begin_pack(comms::Setup_Message::ENUMERATE_SOURCES);
        m_setup_channel.pack_param(req_id);
        m_setup_channel.pack_param(static_cast<uint32_t>(all.size()));

        for (auto const& n: all)
        {
            m_setup_channel.pack_param(n->get_name());
            m_setup_channel.pack_param(static_cast<uint32_t>(n->get_output_stream_count()));
            for (size_t i = 0; i < n->get_output_stream_count(); i++)
            {
                m_setup_channel.pack_param(n->get_output_stream(i).get_type());
                m_setup_channel.pack_param(n->get_output_stream(i).get_name());
            }
        }

        m_setup_channel.end_pack();
    }
    else
    {
        QLOGE("Error in enumerating sources");
    }
}

void Comms::handle_enumerate_sinks()
{
    uint32_t req_id = 0;
    if (m_setup_channel.unpack(req_id))
    {
        QLOGI("Req Id: {} - enumerate sinks", req_id);
        auto const& all = m_hal.get_sinks().get_all();

        m_setup_channel.begin_pack(comms::Setup_Message::ENUMERATE_SINKS);
        m_setup_channel.pack_param(req_id);
        m_setup_channel.pack_param(static_cast<uint32_t>(all.size()));

        for (auto const& n: all)
        {
            m_setup_channel.pack_param(n->get_name());
            m_setup_channel.pack_param(static_cast<uint32_t>(n->get_input_stream_count()));
            for (size_t i = 0; i < n->get_input_stream_count(); i++)
            {
                m_setup_channel.pack_param(n->get_input_stream(i).get_type());
                m_setup_channel.pack_param(n->get_input_stream(i).get_name());
            }
        }

        m_setup_channel.end_pack();
    }
    else
    {
        QLOGE("Error in enumerating sinks");
    }
}

void Comms::handle_enumerate_processors()
{
    uint32_t req_id = 0;
    if (m_setup_channel.unpack(req_id))
    {
        QLOGI("Req Id: {} - enumerate processors", req_id);
        auto const& all = m_hal.get_processors().get_all();

        m_setup_channel.begin_pack(comms::Setup_Message::ENUMERATE_PROCESSORS);
        m_setup_channel.pack_param(req_id);
        m_setup_channel.pack_param(static_cast<uint32_t>(all.size()));

        for (auto const& n: all)
        {
            m_setup_channel.pack_param(n->get_name());
            m_setup_channel.pack_param(static_cast<uint32_t>(n->get_input_stream_count()));
            for (size_t i = 0; i < n->get_input_stream_count(); i++)
            {
                m_setup_channel.pack_param(n->get_input_stream(i).get_type());
                m_setup_channel.pack_param(n->get_input_stream(i).get_name());
            }
            m_setup_channel.pack_param(static_cast<uint32_t>(n->get_output_stream_count()));
            for (size_t i = 0; i < n->get_output_stream_count(); i++)
            {
                m_setup_channel.pack_param(n->get_output_stream(i).get_type());
                m_setup_channel.pack_param(n->get_output_stream(i).get_name());
            }
        }

        m_setup_channel.end_pack();
    }
    else
    {
        QLOGE("Error in enumerating processors");
    }
}

void Comms::handle_enumerate_streams()
{
    uint32_t req_id = 0;
    if (m_setup_channel.unpack(req_id))
    {
        QLOGI("Req Id: {} - enumerate streams", req_id);
        auto const& all = m_hal.get_streams().get_all();

        m_setup_channel.begin_pack(comms::Setup_Message::ENUMERATE_STREAMS);
        m_setup_channel.pack_param(req_id);
        m_setup_channel.pack_param(static_cast<uint32_t>(all.size()));

        for (auto const& n: all)
        {
            m_setup_channel.pack_param(n->get_type());
            m_setup_channel.pack_param(n->get_name());
        }

        m_setup_channel.end_pack();
    }
    else
    {
        QLOGE("Error in enumerating streams");
    }
}

template<class Registry, class Node_Base>
void Comms::handle_node_config(comms::Setup_Message message, Registry const& registry)
{
    m_setup_channel.begin_unpack();
    uint32_t req_id = 0;
    std::string name;
    if (!m_setup_channel.unpack_param(req_id) ||
        !m_setup_channel.unpack_param(name))
    {
        QLOGE("Error in unpacking config rquest");
        return;
    }

    QLOGI("Req Id: {} - node config", req_id);
    auto* node = registry.template find_by_name<Node_Base>(name);
    if (!node)
    {
        QLOGE("Req Id: {} - cannot find node '{}'", req_id, name);
        return;
    }

    std::string config_str;
    m_setup_channel.unpack_param<std::string>(config_str);

    if (!config_str.empty())
    {
        rapidjson::Document config;
        if (config.Parse(config_str.c_str()).HasParseError())
        {
            QLOGE("Req Id: {} - failed to parse config for '{}': {}:{}", req_id, name, config.GetParseError(), config.GetErrorOffset());
            return;
        }
        node->set_config(config);
    }

    {
        config_str.clear();
        auto config = node->get_config();
        if (config)
        {
            rapidjson::StringBuffer s;
            rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(s);
            config->Accept(writer);    // Accept() traverses the DOM and generates Handler events.
            config_str = s.GetString();
        }
        m_setup_channel.pack(message, req_id, name, config_str);
    }
}

void Comms::handle_source_config()
{
    handle_node_config<decltype(m_hal.get_sources()), node::source::ISource>(
                comms::Setup_Message::SOURCE_CONFIG, m_hal.get_sources());
}
void Comms::handle_sink_config()
{
    handle_node_config<decltype(m_hal.get_sinks()), node::sink::ISink>(
                comms::Setup_Message::SINK_CONFIG, m_hal.get_sinks());
}
void Comms::handle_processor_config()
{
    handle_node_config<decltype(m_hal.get_processors()), node::processor::IProcessor>(
                comms::Setup_Message::PROCESSOR_CONFIG, m_hal.get_processors());
}
void Comms::handle_stream_config()
{
    handle_node_config<decltype(m_hal.get_streams()), node::stream::IStream>(
                comms::Setup_Message::STREAM_CONFIG, m_hal.get_streams());
}

void Comms::handle_telemetry_streams()
{
    uint32_t req_id = 0;
    if (!m_setup_channel.begin_unpack() ||
        !m_setup_channel.unpack_param(req_id))
    {
        QLOGE("Error in unpacking telemetry streams");
        return;
    }
    QLOGI("Req Id: {} - telemetry streams", req_id);

    uint32_t size = 0;
    if (m_setup_channel.unpack_param(size))
    {
        m_telemetry_streams.clear();
        m_telemetry_streams.reserve(size);

        std::string name;
        for (uint32_t i = 0; i < size; i++)
        {
            if (!m_setup_channel.unpack_param(name))
            {
                m_telemetry_streams.clear();
                QLOGE("Req Id: {} - error in unpacking telemetry streams", req_id);
                return;
            }
            auto* stream = m_hal.get_streams().find_by_name<node::stream::IStream>(name);
            if (stream)
            {
                m_telemetry_streams.push_back(stream);
            }
            else
            {
                QLOGW("Req Id: {} - cannot find stream '{}' for telemetry", req_id, name);
            }
        }
    }

    m_setup_channel.end_unpack();

    m_setup_channel.begin_pack(comms::Setup_Message::TELEMETRY_STREAMS);
    m_setup_channel.pack_param(req_id);
    m_setup_channel.pack_param(static_cast<uint32_t>(m_telemetry_streams.size()));
    for (auto const& s: m_telemetry_streams)
    {
        QASSERT(s);
        m_setup_channel.pack_param(s->get_name());
    }
    m_setup_channel.end_pack();
}

void Comms::process()
{
    if (!is_connected())
    {
        return;
    }

//    while (auto msg = m_input_channel.get_next_message())
//    {
//        switch (msg.get())
//        {
//            case comms::Input_Message::UAV_INPUT: recieve_uav_input(); break;
//            case comms::Input_Message::CAMERA_MOUNT_INPUT: recieve_camera_mount_input(); break;
//            case comms::Input_Message::MOTOR_TEST_INPUT: recieve_motor_test_input(); break;
//            default:
//                QLOGW("Received unhandled message: {}", static_cast<int>(msg.get()));
//                m_error_count++;
//            break;
//            //LOG_INFO("Received message: {}", static_cast<int>(msg.get()));
//        }
//    }


    while (auto msg = m_setup_channel.get_next_message())
    {
        switch (msg.get())
        {
        case comms::Setup_Message::ENUMERATE_SOURCES: handle_enumerate_sources(); break;
        case comms::Setup_Message::ENUMERATE_SINKS: handle_enumerate_sinks(); break;
        case comms::Setup_Message::ENUMERATE_STREAMS: handle_enumerate_streams(); break;
        case comms::Setup_Message::ENUMERATE_PROCESSORS: handle_enumerate_processors(); break;

        case comms::Setup_Message::SOURCE_CONFIG: handle_source_config(); break;
        case comms::Setup_Message::SINK_CONFIG: handle_sink_config(); break;
        case comms::Setup_Message::PROCESSOR_CONFIG: handle_processor_config(); break;
        case comms::Setup_Message::STREAM_CONFIG: handle_stream_config(); break;

        case comms::Setup_Message::TELEMETRY_STREAMS: handle_telemetry_streams(); break;

            default:
                QLOGW("Received unhandled message: {}", static_cast<int>(msg.get()));
//                m_error_count++;
            break;
            //LOG_INFO("Received message: {}", static_cast<int>(msg.get()));
        }
    }

    send_telemetry_streams();

    {
        if (m_rudp.get_send_endpoint().address().is_unspecified() && !m_rudp.get_last_receive_endpoint().address().is_unspecified())
        {
            auto endpoint = m_rudp.get_last_receive_endpoint();
            endpoint.port(m_send_port);
            m_rudp.set_send_endpoint(endpoint);
        }
    }

    m_rudp.process();

    auto now = q::Clock::now();
    if (now - m_last_rudp_time_stamp >= RUDP_PERIOD)
    {
        m_last_rudp_time_stamp = now;

        m_setup_channel.send();
        m_input_channel.send();
        m_telemetry_channel.try_sending();
    }

//    static std::vector<uint8_t> buf;
//    if (buf.empty())
//    {
//        buf.resize(16000000);
//        std::generate(buf.begin(), buf.end(), [](){ return rand() % 50;});
//    }

//    while(true)
//    {
//        m_rudp.send(12, buf.data(), buf.size());

//        m_rudp.process();
//        //m_channel.send(COMMS_CHANNEL);
//        static int xxx = 0;
//        LOG_INFO("{}", xxx);
//        xxx++;
//        boost::this_thread::sleep_for(boost::chrono::milliseconds(30));
//    }
}

auto Comms::get_rudp() -> util::RUDP&
{
    return m_rudp;
}

