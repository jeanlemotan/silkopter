#include "BrainStdAfx.h"

#ifdef RASPBERRY_PI

//#include "hal_motors_gpio.cfg.hpp"

#include "HAL_Motors_PiGPIO.h"
#include "utils/Json_Helpers.h"
#include "utils/Timed_Scope.h"

extern "C"
{
#include <pigpiod_if.h>
}

static constexpr std::array<int32_t, 6> MOTOR_GPIOS = {17, 27, 22, 23, -1, -1};
//constexpr uint32_t GPIO_PWM_RANGE = 10000;


constexpr size_t MIN_SERVO_PULSE = 1000;
constexpr size_t MAX_SERVO_PULSE = 2000;

constexpr size_t MIN_PWM_PULSE = 0;
constexpr size_t MAX_PWM_PULSE = 1000;

using namespace silk;
using namespace boost::asio;

///////////////////////////////////////////////////////////////

HAL_Motors_PiGPIO::HAL_Motors_PiGPIO()
{
    load_settings();
}

HAL_Motors_PiGPIO::~HAL_Motors_PiGPIO()
{
    shutdown();
}

auto HAL_Motors_PiGPIO::load_settings() -> bool
{
//    autojsoncxx::ParsingResult result;
//    GPIO_Pins m;
//    if (!autojsoncxx::from_json_file("motors_gpio.cfg", m, result))
//    {
//        SILK_WARNING("Failed to load motors_gpio.cfg: {}", result.description());
//        return false;
//    }

    return true;
}
void HAL_Motors_PiGPIO::save_settings()
{
}

auto HAL_Motors_PiGPIO::init() -> bool
{
    //QASSERT(!m_is_initialized);
    if (m_is_initialized)
    {
        return true;
    }

    m_is_initialized = true;

    //first of all turn on the pull-down to that if the board is reset of powerred off the motors don't start spinning
    //after a restart the GPIO pins are configured as inputs so their state is floating. Most of the time this results in a high pin
    for (auto gpio: MOTOR_GPIOS)
    {
        if (gpio >= 0)
        {
            if (gpioSetPullUpDown(gpio, PI_PUD_DOWN) < 0)
            {
                SILK_ERR("GPIO {}: Cannot set pull down mode", gpio);
                return false;
            }
            if (gpioSetMode(gpio, PI_OUTPUT) < 0)
            {
                SILK_ERR("GPIO {}: Cannot set GPIO mode to output", gpio);
                return false;
            }
        }
    }

    //now configure the pin for PWM or servo
    {
        uint32_t freq = 0;
        switch (m_settings.frequency)
        {
            case PWM_Frequency::SERVO_50HZ: freq = 50; break;
            case PWM_Frequency::SERVO_100HZ: freq = 100; break;
            case PWM_Frequency::SERVO_200HZ: freq = 200; break;
            case PWM_Frequency::SERVO_400HZ: freq = 400; break;
            case PWM_Frequency::PWM_1000HZ: freq = 1000; break;
            default:
            {
                SILK_ERR("Cannot recognize pwm frequency {}", static_cast<int>(m_settings.frequency));
                return false;
            }
        }

        for (auto gpio: MOTOR_GPIOS)
        {
            if (gpio >= 0)
            {
                auto f = gpioSetPWMfrequency(gpio, freq);
                if (f < 0)
                {
                    SILK_ERR("GPIO {}: Cannot set pwm frequency {}", gpio, freq);
                    return false;
                }
                int range = 0;
                if (m_settings.frequency < PWM_Frequency::PWM_1000HZ)
                {
                    range = 1000000 / freq;
                    if (gpioSetPWMrange(gpio, range) < 0)
                    {
                        SILK_ERR("GPIO {}: Cannot set pwm range {} on gpio {}", gpio, range);
                        return false;
                    }
                }
                else
                {
                    range = MAX_PWM_PULSE - MIN_PWM_PULSE;
                    if (gpioSetPWMrange(gpio, range) < 0)
                    {
                        SILK_ERR("GPIO {}: Cannot set pwm range {} on gpio {}", gpio, range);
                        return false;
                    }
                }

                SILK_INFO("GPIO {}: PWM frequency {} (requested {}), range {}", gpio, f, freq, range);
            }
        }
    }

    cut_throttle();
//    else
//    {
//        SILK_WARNING("Non-pwm ESC support not implemented");
//        return false;
//    }

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

auto HAL_Motors_PiGPIO::get_pulse_range() const -> std::pair<int, int>
{
    if (m_settings.frequency < PWM_Frequency::PWM_1000HZ)
    {
        return { MIN_SERVO_PULSE, MAX_SERVO_PULSE};
    }
    else
    {
        return { MIN_PWM_PULSE, MAX_PWM_PULSE};
    }
}

void HAL_Motors_PiGPIO::cut_throttle()
{
    QASSERT(m_is_initialized);
    if (!m_is_initialized)
    {
        return;
    }

    auto range = get_pulse_range();

    for (auto gpio: MOTOR_GPIOS)
    {
        if (gpio >= 0)
        {
            gpioPWM(gpio, range.first);
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

    auto range = get_pulse_range();

    for (size_t i = 0; i < count; i++)
    {
        auto gpio = MOTOR_GPIOS[i];
        if (gpio >= 0)
        {
            //        if (i != 0)
            //        {
            //            continue;
            //        }

            auto throttle = math::clamp(throttles[i], 0.f, 1.f);
            m_motors[i].throttle = throttle;

            int pulse = throttle * (range.second - range.first);
            gpioPWM(gpio, range.first + pulse);
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
