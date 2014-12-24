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

static constexpr std::array<uint32_t, 6> MOTOR_GPIOS = {17, 27, 22, 23, 18, 24};
//constexpr uint32_t GPIO_PWM_RANGE = 10000;


constexpr size_t MIN_PULSE = 1000;
constexpr size_t MAX_PULSE = 2000;

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
        if (set_pull_up_down(gpio, PI_PUD_DOWN) < 0)
        {
            SILK_ERR("GPIO {}: Cannot set pull down mode", gpio);
            return false;
        }
        if (set_mode(gpio, PI_OUTPUT) < 0)
        {
            SILK_ERR("GPIO {}: Cannot set GPIO mode to output", gpio);
            return false;
        }
    }

    //now configure the pin for PWM or servo
    //if (m_settings.frequency >= PWM_Frequency::PWM_1000Hz)
    {
        uint32_t freq = 0;
        switch (m_settings.frequency)
        {
            case PWM_Frequency::SERVO_50HZ: freq = 50; break;
            case PWM_Frequency::SERVO_100HZ: freq = 100; break;
            case PWM_Frequency::SERVO_200HZ: freq = 200; break;
            case PWM_Frequency::SERVO_400HZ: freq = 400; break;
            default:
            {
                SILK_ERR("Cannot recognize pwm frequency {}", static_cast<int>(m_settings.frequency));
                return false;
            }
        }

        for (auto gpio: MOTOR_GPIOS)
        {
            auto f = set_PWM_frequency(gpio, freq);
            if (f < 0)
            {
                SILK_ERR("GPIO {}: Cannot set pwm frequency {}", gpio, freq);
                return false;
            }
            m_range = 1000000 / freq;
            if (set_PWM_range(gpio, m_range) < 0)
            {
                SILK_ERR("GPIO {}: Cannot set pwm range {} on gpio {}", gpio, m_range);
                return false;
            }

            SILK_INFO("GPIO {}: PWM frequency {} (requested {}), range {}", gpio, f, freq, m_range);
        }
    }
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

void HAL_Motors_PiGPIO::cut_throttle()
{
    QASSERT(m_is_initialized);
    if (!m_is_initialized)
    {
        return;
    }

    for (auto gpio: MOTOR_GPIOS)
    {
        set_PWM_dutycycle(gpio, MIN_PULSE);
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

        uint32_t pulse = throttle * (MAX_PULSE - MIN_PULSE);
        set_PWM_dutycycle(MOTOR_GPIOS[i], MIN_PULSE + pulse);
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
