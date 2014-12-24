#pragma once

#include "HAL_Camera_Mount.h"

namespace silk
{

class HAL_Camera_Mount_PiGPIO : public HAL_Camera_Mount
{
public:
    HAL_Camera_Mount_PiGPIO();
    ~HAL_Camera_Mount_PiGPIO();

    auto init() -> bool;
    void shutdown();

    //----------------------------------------------------------------------
    //mount

    void set_rotation(math::quatf const& rot);

    //----------------------------------------------------------------------
    void process();

private:
    enum class PWM_Frequency : uint8_t
    {
        SERVO_50HZ,
        SERVO_100HZ,
        SERVO_200HZ,
        SERVO_400HZ,
    };

    struct Settings
    {
        size_t min_pulse = 1000;
        size_t max_pulse = 2000;

        PWM_Frequency frequency = PWM_Frequency::SERVO_50HZ;
    } m_settings;

    bool m_is_initialized = false;

    auto load_settings() -> bool;
    void save_settings();
};

}
