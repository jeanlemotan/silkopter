#include "Stick_Actuators_Throttle_DRV883x.h"

#include "utils/hw/pigpio.h"

constexpr q::Clock::duration PERIOD = std::chrono::milliseconds(10);

namespace silk
{

Stick_Actuators_Throttle_DRV883x::Stick_Actuators_Throttle_DRV883x(ISticks const& sticks)
    : m_sticks(sticks)
{

}

Stick_Actuators_Throttle_DRV883x::~Stick_Actuators_Throttle_DRV883x()
{
    gpioSetMode(m_enable_gpio, PI_INPUT);
    gpioSetMode(m_phase_gpio, PI_INPUT);
}

ts::Result<void> Stick_Actuators_Throttle_DRV883x::init()
{
    m_phase_gpio = 4;
    m_enable_gpio = 5;

    if (gpioSetMode(m_enable_gpio, PI_OUTPUT) != 0)
    {
        return ts::Error("Cannot set 'enable' as output on GPIO " + std::to_string(m_enable_gpio));
    }

    if (gpioSetMode(m_phase_gpio, PI_OUTPUT) != 0)
    {
        gpioSetMode(m_enable_gpio, PI_INPUT);
        return ts::Error("Cannot set 'phase' as output on GPIO " + std::to_string(m_phase_gpio));
    }

    gpioWrite(m_enable_gpio, 0);

    gpioPWM(m_phase_gpio, 0);
    gpioSetPWMfrequency(m_phase_gpio, 20000);

    gpioWrite(m_enable_gpio, 1);
    gpioPWM(m_phase_gpio, 127);

    PID::Params params;

    //for 9V
//    params.kp = 3.3f;
//    params.ki = 10.0f;
//    params.max_i = 0.1f;
//    params.kd = 0.03f;

    //for 5v
    params.kp = 8.3f;
    params.ki = 20.0f;
    params.max_i = 0.2f;
    params.kd = 0.03f;

    params.d_filter = 0.0f;
    params.rate = std::chrono::duration<float>(1) / PERIOD;
    m_throttle_pid.set_params(params);

    m_last_tp = q::Clock::now();

    return detect_direction();
}

//void Stick_Actuators_Throttle_DRV883x::set_p(float a, float b)
//{
//    PID::Params params;
//    params.kp = 3.4f;
//    params.ki = a;
//    params.max_i = a / 100.f;
//    params.kd = b;
//    params.d_filter = 0.9f;
//    params.rate = std::chrono::duration<float>(1) / PERIOD;
//    m_throttle_pid.set_params(params);

//    QLOGI("a = {}, b = {}", a, b);
//}

ts::Result<void> Stick_Actuators_Throttle_DRV883x::detect_direction()
{
    return ts::success;
}

bool Stick_Actuators_Throttle_DRV883x::set_target_yaw(boost::optional<float> value)
{
    return false;
}

bool Stick_Actuators_Throttle_DRV883x::set_target_pitch(boost::optional<float> value)
{
    return false;
}

bool Stick_Actuators_Throttle_DRV883x::set_target_roll(boost::optional<float> value)
{
    return false;
}

bool Stick_Actuators_Throttle_DRV883x::set_target_throttle(boost::optional<float> value)
{
    m_target_throttle = value;
    if (!value)
    {
        gpioWrite(m_enable_gpio, 0);
        gpioPWM(m_phase_gpio, 127);
    }
    else
    {
        gpioWrite(m_enable_gpio, 1);
    }
    return true;
}

void Stick_Actuators_Throttle_DRV883x::process()
{
    if (!m_target_throttle)
    {
        return;
    }

    auto now = q::Clock::now();
    auto dt = now - m_last_tp;
    if (dt <= std::chrono::milliseconds(10))
    {
        return;
    }

    float crt_throttle = m_sticks.get_throttle();
    float target_throttle = *m_target_throttle;

//    if (math::abs(target_throttle - crt_throttle) < 0.01f)
//    {
//        crt_throttle = target_throttle;
//    }

    float factor = m_throttle_pid.process(crt_throttle, target_throttle);

    int pwm = static_cast<int>(factor * 128.f) + 127;
    pwm = math::clamp(pwm, 0, 255);

    gpioPWM(m_phase_gpio, pwm);

    m_last_tp = now;
}

}
