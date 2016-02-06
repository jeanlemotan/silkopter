#pragma once

#include <_qmath.h>
#include <board/board.h>
#include <QBase.h>
#include "utils/storage.h"
#include "utils/Buffer.h"

#include "utils/Serial_Channel.h"

namespace silk
{

    struct UART_Adapter
    {
        UART_Adapter() = default;
        UART_Adapter(board::UART& uart) : m_uart(uart) {}
        template<class CB> void read(CB& dst)
        {
            QASSERT_MSG(m_uart.get_last_error() == board::UART::Error::NONE, "UART error {}", static_cast<int>(m_uart.get_last_error()));
            uint8_t ch = 0;
            auto available = m_uart.get_data_size();
            auto sz = math::min(available, dst.get_space_left());
            while (sz > 0)
            {
                m_uart.read(ch);
                QASSERT_MSG(m_uart.get_last_error() == board::UART::Error::NONE, "UART error {}", static_cast<int>(m_uart.get_last_error()));
                dst.push_back(ch);
                sz--;
            }
        }
        template<class CB> void write(CB const& data, size_t size)
        {
            QASSERT_MSG(m_uart.get_last_error() == board::UART::Error::NONE, "UART error {}", static_cast<int>(m_uart.get_last_error()));
            auto ws = m_uart.write(data.data(), size);
            QASSERT_MSG(m_uart.get_last_error() == board::UART::Error::NONE, "UART error {}", static_cast<int>(m_uart.get_last_error()));
            QASSERT(ws == size);
        }
    private:
        board::UART& m_uart;
    };

class Comms
{
public:
    Comms();

    void process();

private:
    enum class Message : uint8_t
    {
        STAY_ALIVE,
        CONFIG,
        SENSOR_DATA,
        GPS_DATA,
        //---
        RATE_TARGETS,
        THROTTLE,
        //---
        PWM_CONFIG,
    };

    enum class Sensor : uint8_t
    {
        ACCELEROMETER   = 1 << 0,   //uint8 - dt ms, vec3s16 - acceleration
        GYROSCOPE       = 1 << 1,   //uint8 - dt ms,  vec3s16 - angular velocity
        COMPASS         = 1 << 2,   //vec3s16 - quantized normalized direction
        BAROMETER       = 1 << 3,   //float - pressure
        THERMOMETER     = 1 << 4,   //int16 - temperature
        SONAR           = 1 << 5,   //uint16 - distance
        VOLTAGE         = 1 << 6,   //uint16 - voltage
        CURRENT         = 1 << 7,   //uint16 - amperes / second
    };
    typedef q::util::Flag_Set<Sensor, uint8_t> Sensors;

    enum class State : uint8_t
    {
        DISCONNECTED,   //not connected yet
        HANDSHAKE,
        RUNNING,
    };

    Message m_message;
    bool m_has_message = false;
    chrono::time_ms m_last_message_timestamp;

    State m_state = State::HANDSHAKE;

    void set_state(State state);
    void process_state_handshake();
    void process_state_running();

    void process_watchdog();
    void process_config_requests();

    UART_Adapter m_uart_adapter;
    typedef util::Serial_Channel<Message, UART_Adapter> Serial_Channel;
    Serial_Channel m_channel;

    enum class PWM_Frequency : uint8_t
    {
        _50HZ,
        _100HZ,
        _250HZ,
        _500HZ
    };

    PWM_Frequency m_pwm_frequency = PWM_Frequency::_50HZ;

    board::Accelerometer::Data m_accelerometer_data;
    board::Gyroscope::Data m_gyroscope_data;
    board::Compass::Data m_compass_data;
    board::Barometer::Data m_barometer_data;
    board::Thermometer::Data m_thermometer_data;
    board::Sonar::Data m_sonar_data;
    board::GPS::Data m_gps_data;

    chrono::time_us m_accelerometer_timestamp;
    chrono::time_us m_gyroscope_timestamp;
    chrono::time_us m_compass_timestamp;
    chrono::time_us m_barometer_timestamp;
    chrono::time_us m_thermometer_timestamp;
    chrono::time_us m_sonar_timestamp;
    chrono::time_us m_gps_timestamp;
};



}
