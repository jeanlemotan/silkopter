#include "BrainStdAfx.h"

#ifdef RASPBERRY_PI

#include "HAL_Camera_Mount_PiGPIO.h"
#include "utils/Json_Helpers.h"
#include "utils/Timed_Scope.h"

extern "C"
{
    #include <pigpio.h>
}

//yaw pitch roll
static constexpr std::array<std::pair<int32_t, char const*>, 3> SERVOS =
{{
    {18, "PITCH"},
    {-1, "ROLL"},
    {-1, "YAW"}
}};

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

    for (size_t i = 0; i < SERVOS.size(); i++)
    {
        auto gpio = SERVOS[i].first;
        auto name = SERVOS[i].second;
        if (gpio >= 0)
        {
            auto f = gpioSetPWMfrequency(gpio, freq);
            if (f < 0)
            {
                SILK_ERR("{} GPIO {}: Cannot set pwm frequency {}", name, gpio, freq);
                return false;
            }
            auto range = 1000000 / freq;
            if (gpioSetPWMrange(gpio, range) < 0)
            {
                SILK_ERR("{} GPIO {}: Cannot set pwm range {} on gpio {}", name, gpio, range);
                return false;
            }
            gpioPWM(gpio, 1500);
            SILK_INFO("{} GPIO {}: PWM frequency {} (requested {}), range {}", name, gpio, f, freq, range);
        }
        else
        {
            SILK_INFO("{} Servo disabled", name);
        }
    }

    m_last_time_point = q::Clock::now();

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

    constexpr std::chrono::milliseconds UPDATE_PERIOD(20);

    auto now = q::Clock::now();
    if (now - m_last_time_point < UPDATE_PERIOD)
    {
        return;
    }
    m_last_time_point = now;

    if (m_rotation != m_last_rotation)
    {
        m_last_rotation = m_rotation;



        //x - pitch
        if (SERVOS[0].first >= 0)
        {
            auto pulse = compute_pulse(m_rotation.x, m_settings.x_range);
            //SILK_INFO("mount rotation: {}", pulse);
            gpioPWM(SERVOS[0].first, pulse);
        }

        //y - roll
        if (SERVOS[1].first >= 0)
        {
            auto pulse = compute_pulse(m_rotation.y, m_settings.y_range);
            gpioPWM(SERVOS[1].first, pulse);
        }

        //z - yaw
        if (SERVOS[2].first >= 0)
        {
            auto pulse = compute_pulse(m_rotation.z, m_settings.z_range);
            gpioPWM(SERVOS[2].first, pulse);
        }
    }
}

auto HAL_Camera_Mount_PiGPIO::compute_pulse(float angle, Settings::Range const& range) -> uint32_t
{
    //restrict the input angle to the range
    float v = math::clamp(angle, range.min.angle, range.max.angle);

    //calculate the deviation we have
    float mu = (v - range.min.angle) / (range.max.angle - range.min.angle);

    //now we can calculate the pulse
    uint32_t pulse = range.min.pulse + mu * (range.max.pulse - range.min.pulse);

    return pulse;
}

void HAL_Camera_Mount_PiGPIO::set_rotation(math::vec3f const& rot)
{
    m_rotation = rot;
}

#endif
