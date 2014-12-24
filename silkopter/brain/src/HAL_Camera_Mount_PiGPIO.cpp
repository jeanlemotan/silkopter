#include "BrainStdAfx.h"

#ifdef RASPBERRY_PI

#include "HAL_Camera_Mount_PiGPIO.h"
#include "utils/Json_Helpers.h"
#include "utils/Timed_Scope.h"

extern "C"
{
#include <pigpiod_if.h>
}

//yaw pitch roll
static constexpr std::array<int32_t, 3> SERVO_GPIOS_YPR = {-1, 18, -1};

using namespace silk;
using namespace boost::asio;

///////////////////////////////////////////////////////////////

HAL_Camera_Mount_PiGPIO::HAL_Camera_Mount_PiGPIO()
{
    load_settings();
}

HAL_Camera_Mount_PiGPIO::~HAL_Camera_Mount_PiGPIO()
{
    shutdown();
}

auto HAL_Camera_Mount_PiGPIO::load_settings() -> bool
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
void HAL_Camera_Mount_PiGPIO::save_settings()
{
}

auto HAL_Camera_Mount_PiGPIO::init() -> bool
{
    //QASSERT(!m_is_initialized);
    if (m_is_initialized)
    {
        return true;
    }

    m_is_initialized = true;

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

    std::array<char const*, 3> names = { "YAW", "PITCH", "ROLL" };
    for (size_t i = 0; i < SERVO_GPIOS_YPR.size(); i++)
    {
        auto gpio = SERVO_GPIOS_YPR[i];
        auto name = names[i];
        if (gpio >= 0)
        {
            auto f = set_PWM_frequency(gpio, freq);
            if (f < 0)
            {
                SILK_ERR("{} GPIO {}: Cannot set pwm frequency {}", name, gpio, freq);
                return false;
            }
            auto range = 1000000 / freq;
            if (set_PWM_range(gpio, range) < 0)
            {
                SILK_ERR("{} GPIO {}: Cannot set pwm range {} on gpio {}", name, gpio, range);
                return false;
            }
            SILK_INFO("{} GPIO {}: PWM frequency {} (requested {}), range {}", name, gpio, f, freq, range);
        }
        else
        {
            SILK_INFO("{} Servo disabled", name);
        }
    }

    m_is_initialized = true;
    return true;
}

void HAL_Camera_Mount_PiGPIO::shutdown()
{
    m_is_initialized = false;
}

void HAL_Camera_Mount_PiGPIO::process()
{
    QASSERT(m_is_initialized);
    if (!m_is_initialized)
    {
        return;
    }

}

void HAL_Camera_Mount_PiGPIO::set_rotation(math::quatf const& rot)
{
    //yaw

    //pitch
    {
        float x = 0.5f;
        uint32_t pulse = m_settings.min_pulse + x * (m_settings.max_pulse - m_settings.min_pulse);
        set_PWM_dutycycle(SERVO_GPIOS_YPR[1], pulse);
    }

    //roll
}

#endif
