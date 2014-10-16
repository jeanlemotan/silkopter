﻿#include "BrainStdAfx.h"

#ifdef RASPBERRY_PI

#include "hal_motors_gpio.cfg.hpp"

#include "HAL_Motors_PiGPIO.h"
#include "utils/Json_Helpers.h"
#include "utils/Timed_Scope.h"
#include <pigpio.h>

static constexpr std::array<uint32_t, 6> MOTOR_GPIOS = {4, 17, 18, 27, 22, 23};
constexpr uint32_t GPIO_PWM_RANGE = 10000;

using namespace silk;
using namespace boost::asio;

///////////////////////////////////////////////////////////////

HAL_Motors_PiGPIO::HAL_Motors_PiGPIO(PiGPIO& pigpio)
    : m_pigpio(pigpio)
{
    load_settings();
}

HAL_Motors_PiGPIO::~HAL_Motors_PiGPIO()
{
    shutdown();
}

auto HAL_Motors_PiGPIO::load_settings() -> bool
{
    autojsoncxx::ParsingResult result;
    GPIO_Pins m;
    if (!autojsoncxx::from_json_file("motors_gpio.cfg", m, result))
    {
        SILK_WARNING("Failed to load motors_gpio.cfg: {}", result.description());
        return false;
    }

    return true;
}
void HAL_Motors_PiGPIO::save_settings()
{
}

auto HAL_Motors_PiGPIO::init() -> bool
{
    QASSERT(!m_is_initialized);
    if (m_is_initialized)
    {
        return true;
    }

    if (m_settings.frequency >= PWM_Frequency::PWM_1000Hz)
    {
        uint32_t freq = 0;
        switch (m_settings.frequency)
        {
        case PWM_Frequency::PWM_1000Hz: freq = 1000; break;
        default:
        {
            SILK_WARNING("Cannot recognize pwm frequency {}", static_cast<int>(m_settings.frequency));
            return false;
        }
        }

        for (auto gpio: MOTOR_GPIOS)
        {
            if (gpioSetPWMfrequency(gpio, freq) < 0)
            {
                SILK_WARNING("Cannot set pwm frequency {} on gpio {}", freq, gpio);
                return false;
            }
            if (gpioSetPWMrange(gpio, GPIO_PWM_RANGE) < 0)
            {
                SILK_WARNING("Cannot set pwm range {} on gpio {}", GPIO_PWM_RANGE, gpio);
                return false;
            }
        }
    }
    else
    {
        SILK_WARNING("Non-pwm ESC support not implemented");
        return false;
    }

    m_is_initialized = true;
    return true;
}

void HAL_Motors_PiGPIO::shutdown()
{
    if (m_is_initialized)
    {
        cut_throttle();
    }
    m_is_initialized = false;
}

void HAL_Motors_PiGPIO::cut_throttle()
{
    QASSERT(m_is_initialized);
    if (!m_is_initialized)
    {
        return;
    }

    for (auto gpio: MOTOR_GPIOS)
    {
        if (m_settings.frequency >= PWM_Frequency::PWM_1000Hz)
        {
            uint32_t pulse = 0;
            gpioPWM(gpio, pulse);
        }
        else
        {
            QASSERT(0);
        }
    }
}

void HAL_Motors_PiGPIO::set_throttles(float const* throttles, size_t count)
{
    QASSERT(m_is_initialized);
    if (!m_is_initialized)
    {
        return;
    }

    QASSERT(throttles != nullptr);
    if (!throttles)
    {
        return;
    }

    count = math::min(count, MOTOR_GPIOS.size());
    if (count > m_motors.size())
    {
        m_motors.resize(count);
    }

    for (size_t i = 0; i < count; i++)
    {
//        if (i != 0)
//        {
//            continue;
//        }

        auto throttle = math::clamp(throttles[i], 0.f, 0.6f);
        m_motors[i].throttle = throttle;

        if (m_settings.frequency >= PWM_Frequency::PWM_1000Hz)
        {
            uint32_t pulse = throttle * GPIO_PWM_RANGE;
            gpioPWM(MOTOR_GPIOS[i], pulse);
        }
        else
        {
            QASSERT(0);
        }
    }
}

void HAL_Motors_PiGPIO::process()
{
    QASSERT(m_is_initialized);
    if (!m_is_initialized)
    {
        return;
    }

}


#endif