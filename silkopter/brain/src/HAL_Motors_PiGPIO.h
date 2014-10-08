#pragma once

#include "HAL_Motors.h"
#include "PiGPIO.h"

namespace silk
{

class HAL_Motors_PiGPIO : public HAL_Motors
{
public:
    HAL_Motors_PiGPIO(PiGPIO& pigpio);
    ~HAL_Motors_PiGPIO();

    enum class Result
    {
        OK,
        FAILED
    };

    auto init() -> Result;
    void shutdown();

    //----------------------------------------------------------------------
    //motors

    void set_throttles(float const* throttles, size_t count);
    void cut_throttle();

    //----------------------------------------------------------------------
    void process();

private:
    enum class PWM_Frequency : uint8_t
    {
        SERVO_50HZ,
        SERVO_100HZ,
        SERVO_250HZ,
        SERVO_500HZ,
        PWM_1000Hz,
    };

    PiGPIO& m_pigpio;
    bool m_is_initialized = false;

    struct Motor
    {
        float throttle = 0;
    };
    std::vector<Motor> m_motors;

    bool m_is_connected = false;
    size_t m_error_count = 0;

    struct Impl;
    std::shared_ptr<Impl> m_impl;

    struct Settings
    {
        size_t count = 4;
        PWM_Frequency frequency = PWM_Frequency::PWM_1000Hz;
        rapidjson::Document document;
    } m_settings;

    auto load_settings() -> bool;
    void save_settings();

    
};

}
