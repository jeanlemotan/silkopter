#include "Stick_Actuators_Throttle_DRV883x.h"

#include "utils/hw/pigpio.h"

namespace silk
{

Stick_Actuators_Throttle_DRV883x::Stick_Actuators_Throttle_DRV883x(ISticks const& sticks)
    : m_sticks(sticks)
{

}

Stick_Actuators_Throttle_DRV883x::~Stick_Actuators_Throttle_DRV883x()
{

}

ts::Result<void> Stick_Actuators_Throttle_DRV883x::init()
{
    m_phase_gpio = 17;
    m_enable_gpio = 18;

    if (gpioSetMode(m_enable_gpio, PI_OUTPUT) != 0)
    {
        return ts::Error("Cannot set 'enable' as output on GPIO " + std::to_string(m_enable_gpio));
    }
    gpioPWM(m_enable_gpio, 0);

    if (gpioSetMode(m_phase_gpio, PI_OUTPUT) != 0)
    {
        gpioSetMode(m_enable_gpio, PI_INPUT);
        return ts::Error("Cannot set 'phase' as output on GPIO " + std::to_string(m_phase_gpio));
    }

    gpioSetPWMfrequency(m_enable_gpio, 1000);

    PID::Params params;
    params.kp = 10.f;
    params.ki = 1.0f;
    params.max_i = 1.0f;
    params.kd = 0.1f;
    params.d_filter = 0.0f;
    params.rate = 100;
    m_throttle_pid.set_params(params);

    m_last_tp = q::Clock::now();

    return detect_direction();
}

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

    float factor = m_throttle_pid.process(crt_throttle, target_throttle);
    if (factor < 0.f)
    {
        gpioWrite(m_phase_gpio, 0);
    }
    else
    {
        gpioWrite(m_phase_gpio, 1);
    }

    int pwm = std::abs(static_cast<int>(factor * 255.f));
    pwm = math::clamp(pwm, 0, 255);
    gpioPWM(m_enable_gpio, pwm);

    m_last_tp = now;
}

}
