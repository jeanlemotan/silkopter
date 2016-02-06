#include "Comms.h"
#include <board/board.h>
#include <QBase.h>
#include "utils/Timed_Scope.h"

using namespace silk;

constexpr uint32_t VERSION = 1;

Comms::Comms()
#ifdef Q_AVR
    : m_uart_adapter(board::get_comm_uart())
    , m_channel(m_uart_adapter)
#endif
{
}

void Comms::set_state(State state)
{
    IO_BOARD_INFO(F_STR("Changed from {} to {}"), static_cast<int>(m_state), static_cast<int>(state));

    m_state = state;
    m_last_message_timestamp = board::clock::now_ms();
    m_channel.cancel_send();
}

void Comms::process_state_handshake()
{
    static chrono::time_ms start = board::clock::now_ms();
    auto now = board::clock::now_ms();

    //set_state(State::RUNNING);

    if (m_channel.get_send_result() == Serial_Channel::Send_Result::SENT)
    {
        IO_BOARD_INFO(F_STR("Config sent, waiting for config..."));
        set_state(State::RUNNING);
        return;
    }

    if (now - start > chrono::millis(200))
    {
        start = now;

        IO_BOARD_INFO(F_STR("Sending config..."));
        m_channel.cancel_send();

        auto const& config = board::get_config();

        m_channel.send(Message::CONFIG,
                       VERSION,
                       config.has_barometer,
                       config.has_sonar,
                       config.has_gps,
                       config.has_voltage_sensor,
                       config.has_current_sensor,
                       board::get_accelerometer().get_data_config(),
                       board::get_gyroscope().get_data_config(),
                       board::get_compass().get_data_config(),
                       board::get_barometer() ? board::get_barometer()->get_data_config() : board::Barometer::Data_Config(),
                       board::get_sonar() ? board::get_sonar()->get_data_config() : board::Sonar::Data_Config(),
                       board::get_thermometer().get_data_config());
    }
}

static const auto accelerometer_delay = chrono::micros(0);
static const auto gyroscope_delay = chrono::micros(0);
static const auto compass_delay = chrono::micros(1000);
static const auto barometer_delay = chrono::micros(100000);
static const auto thermometer_delay = chrono::micros(1000000);
static const auto sonar_delay = chrono::micros(10000);

namespace board
{
    //extern volatile uint16_t s_skipping;
}

void Comms::process_state_running()
{
//    if (m_has_message && m_message == Message::STAY_ALIVE)
//    {
//        int32_t x = 0;
//        auto res = m_channel.unpack_next_message(x);
//        if (res == Serial_Channel::Unpack_Result::OK)
//        {
//            m_channel.stream(Message::STAY_ALIVE, x);
//        }
//    }

    auto now_us = board::clock::now_us();

    Sensors sensors;

    if (now_us - m_accelerometer_timestamp >= accelerometer_delay &&
        board::get_accelerometer().get_data(m_accelerometer_data))
    {
        m_accelerometer_timestamp = now_us;
        sensors.set(Sensor::ACCELEROMETER);
    }

    if (now_us - m_gyroscope_timestamp >= gyroscope_delay &&
        board::get_gyroscope().get_data(m_gyroscope_data))
    {
        m_gyroscope_timestamp = now_us;
        sensors.set(Sensor::GYROSCOPE);
    }

    if (now_us - m_compass_timestamp >= compass_delay &&
        board::get_compass().get_data(m_compass_data))
    {
        m_compass_timestamp = now_us;
        sensors.set(Sensor::COMPASS);
    }

    if (auto barometer = board::get_barometer())
    {
        if (now_us - m_barometer_timestamp >= barometer_delay &&
            barometer->get_data(m_barometer_data))
        {
            //q::quick_logf("sk: {}", board::s_skipping);
            //board::s_skipping = 0;
            m_barometer_timestamp = now_us;
            sensors.set(Sensor::BAROMETER);
        }
    }
    if (now_us - m_thermometer_timestamp >= thermometer_delay &&
        board::get_thermometer().get_data(m_thermometer_data))
    {
        m_thermometer_timestamp = now_us;
        sensors.set(Sensor::THERMOMETER);
    }
    if (auto sonar = board::get_sonar())
    {
        if (now_us - m_sonar_timestamp >= sonar_delay &&
            sonar->get_data(m_sonar_data))
        {
            m_sonar_timestamp = now_us;
            sensors.set(Sensor::SONAR);
        }
    }

    if (sensors.any())
    {
        m_channel.begin_stream();
        m_channel.add_to_stream(static_cast<uint8_t>(sensors.value()));

        if (sensors.test(Sensor::ACCELEROMETER))
        {
            m_channel.add_to_stream(m_accelerometer_data.dt_ms);
            m_channel.add_to_stream(m_accelerometer_data.acceleration);
        }
        if (sensors.test(Sensor::GYROSCOPE))
        {
            m_channel.add_to_stream(m_gyroscope_data.dt_ms);
            m_channel.add_to_stream(m_gyroscope_data.angular_velocity);
        }
        if (sensors.test(Sensor::COMPASS))
        {
            m_channel.add_to_stream(m_compass_data.direction);
        }
        if (sensors.test(Sensor::BAROMETER))
        {
            m_channel.add_to_stream(m_barometer_data.pressure);
        }
        if (sensors.test(Sensor::THERMOMETER))
        {
            m_channel.add_to_stream(m_thermometer_data.temperature);
        }
        if (sensors.test(Sensor::SONAR))
        {
            m_channel.add_to_stream(m_sonar_data.distance);
        }
        if (sensors.test(Sensor::VOLTAGE))
        {
//            uint16_t d = m_sonar_data.distance * 1000.f;
//            m_channel.add_to_stream(d);
        }
        if (sensors.test(Sensor::CURRENT))
        {
//            uint16_t d = m_sonar_data.distance * 1000.f;
//            m_channel.add_to_stream(d);
        }

        m_channel.end_stream(Message::SENSOR_DATA);
    }

}

void Comms::process_config_requests()
{
    if (!m_has_message)
    {
        return;
    }

//    if (m_message == Message::PWM_CONFIG)
//    {
//        auto res = m_channel.unpack(m_pwm_frequency);
//        if (res == Serial_Channel::Unpack_Result::OK)
//        {
//            IO_BOARD_INFO(F_STR("Received pwm frequency {}"), m_pwm_frequency);
//            m_channel.stream(Message::PID_CONFIG, m_pid_config.id);
//        }
//        else
//        {
//            IO_BOARD_WARNING(F_STR("Failed to receive pid config"));
//        }
//    }
//    else if (m_message == Message::CALIBRATION_CONFIG)
//    {
//        auto res = m_channel.unpack(m_calibration_config);
//        if (res == Serial_Channel::Unpack_Result::OK)
//        {
//            IO_BOARD_INFO(F_STR("Received calibration config {}"), m_calibration_config.id);
//            m_channel.stream(Message::CALIBRATION_CONFIG, m_calibration_config.id);

//            {
//                board::Accelerometer::Calibration_Data data;
//                data.bias = m_calibration_config.accelerometer_bias;
//                data.scale = m_calibration_config.accelerometer_scale;
//                board::get_accelerometer().set_calibration_data(data);
//            }
//            {
//                board::Gyroscope::Calibration_Data data;
//                data.bias = m_calibration_config.gyroscope_bias;
//                board::get_gyroscope().set_calibration_data(data);
//            }
//            {
//                board::Compass::Calibration_Data data;
//                data.bias = m_calibration_config.compass_bias;
//                board::get_compass().set_calibration_data(data);
//            }
//        }
//        else
//        {
//            IO_BOARD_WARNING(F_STR("Failed to receive calibration config"));
//        }
//    }
//    else if (m_message == Message::MOTOR_CONFIG)
//    {
//        m_channel.begin_unpack();
//        m_channel.unpack_param(m_motor_config.id);
//        m_channel.unpack_param(m_motor_config.motor_count);
//        m_channel.unpack_param(m_motor_config.frequency);

//        auto res = Serial_Channel::Unpack_Result::OK;
//        for (uint8_t i = 0; i < m_motor_config.motor_count; i++)
//        {
//            res = m_channel.unpack_param(m_motor_config.motors[i]);
//        }

//        if (res == Serial_Channel::Unpack_Result::OK)
//        {
//            IO_BOARD_INFO(F_STR("Received motor config {}"), m_motor_config.id);
//            m_channel.stream(Message::MOTOR_CONFIG, m_motor_config.id);
//        }
//        else
//        {
//            IO_BOARD_WARNING(F_STR("Failed to receive motor config"));
//        }
//    }
}

void Comms::process_watchdog()
{
    static chrono::time_ms s_last_watchdog_timestamp = board::clock::now_ms();
    auto now = board::clock::now_ms();
    if (now - s_last_watchdog_timestamp < chrono::millis(100))
    {
        return;
    }
    s_last_watchdog_timestamp = now;

    if (m_has_message)
    {
        if (m_state >= State::RUNNING && m_message == Message::CONFIG)
        {
            IO_BOARD_WARNING(F_STR("WATCHDOG: wrong message received. Going to HANDSHAKE"));
            set_state(State::HANDSHAKE);
            return;
        }
    }

    auto duration = now - m_last_message_timestamp;
    if (m_state != State::HANDSHAKE && duration > chrono::millis(1000))
    {
        IO_BOARD_WARNING(F_STR("WATCHDOG: no messages received in the last {}. Going to HANDSHAKE"), duration);
        set_state(State::HANDSHAKE);
        return;
    }
}

void Comms::process()
{
    static auto start = board::clock::now_us();

    do
    {
        m_has_message = m_channel.get_next_message(m_message);
        if (m_has_message)
        {
            m_last_message_timestamp = board::clock::now_ms();
        }

        switch (m_state)
        {
        case State::HANDSHAKE: process_state_handshake(); break;
        case State::RUNNING: process_state_running(); break;
        default:
            IO_BOARD_ERR(F_STR("unknown state: {}"), static_cast<int>(m_state));
        }

        process_watchdog();
        process_config_requests();
    } while (m_has_message);

    {
//        auto now = board::clock::now_us();
//        auto duration = now - start;
//        IO_BOARD_INFO(F_STR("{}"), duration);

//        start = board::clock::now_us();
    }
}

