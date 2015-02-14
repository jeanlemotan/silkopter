#include "BrainStdAfx.h"
#include "PIGPIO.h"

#ifdef RASPBERRY_PI
extern "C"
{
    #include "pigpio.h"
}
#endif

namespace silk
{
namespace node
{
namespace sink
{

PIGPIO::PIGPIO(HAL& hal)
    : m_hal(hal)
{
    //now configure the pin for PWM or servo
    for (size_t i = 0; i < MAX_PWM_CHANNELS; i++)
    {
        auto& channel = m_pwm_channels[i];
        channel.pigpio = this;
        channel.idx = i;
    }
}

auto PIGPIO::get_pwm_channel(size_t idx) -> sink::IPWM*
{
    return m_params.pwm_channels[idx].gpio >= 0 ? &m_pwm_channels[idx] : nullptr;
}

auto PIGPIO::init(Init_Params const& params) -> bool
{
    QLOG_TOPIC("pigpio_pwm::init");

    m_params = params;

    if (!init())
    {
        return false;
    }

    for (size_t i = 0; i < sink::PIGPIO::MAX_PWM_CHANNELS; i++)
    {
        auto* pwm = get_pwm_channel(i);
        if (pwm)
        {
            if (!m_hal.get_sinks().add<IPWM>(q::util::format2<std::string>("{}-pwm{}", params.name, i), *pwm))
            {
                return false;
            }
        }
    }
}

auto PIGPIO::init() -> bool
{
    QLOG_TOPIC("pigpio_pwm::init");

#if defined (RASPBERRY_PI)
    size_t rate = m_params.rate.count();

    std::vector<size_t> rates = {1, 2, 4, 5, 8, 10};
    if (std::find(rates.begin(), rates.end(), rate) == rates.end())
    {
        QLOGE("Invalid rate {}. Supported are: {}", rate, rates);
        return false;
    }

    std::vector<size_t> frequencies = { 40000, 20000, 10000, 8000, 5000, 4000, 2500, 2000, 1600, 1250, 1000, 800, 500, 400, 250, 200, 100, 50 };
    for (auto& f: frequencies)
    {
        auto x = math::round(double(f) / double(rate));
        f = static_cast<size_t>(x);
    }

    //first validate
    for (size_t i = 0; i < MAX_PWM_CHANNELS; i++)
    {
        auto& channel = m_params.pwm_channels[i];
        if (channel.min >= channel.max)
        {
            QLOGE("PIGPIO PWM channel {}: min ({}) is bigger than max ({})", i, channel.min, channel.max);
            return false;
        }
        if (channel.max > channel.range)
        {
            QLOGE("PIGPIO PWM channel {}: max ({}) is bigger than the range ({})", i, channel.max, channel.range);
            return false;
        }
        if (std::find(frequencies.begin(), frequencies.end(), channel.frequency) == frequencies.end())
        {
            QLOGE("Invalid frequency {}. Supported are: {}", channel.frequency, frequencies);
            return false;
        }
    }

    QLOGI("Initializing pigpio");
    if (gpioCfgClock(rate, 1, 0) < 0 ||
        gpioCfgPermissions(static_cast<uint64_t>(-1)) ||
        gpioCfgInterfaces(PI_DISABLE_SOCK_IF | PI_DISABLE_FIFO_IF))
    {
        QLOGE("Cannot configure pigpio");
        return false;
    }
    if (gpioInitialise() < 0)
    {
        QLOGE("Cannot initialize pigpio");
        return false;
    }

    //first of all turn on the pull-down to that if the board is reset of powerred off the motors don't start spinning
    //after a restart the GPIO pins are configured as inputs so their state is floating. Most of the time this results in a high pin
    for (size_t i = 0; i < MAX_PWM_CHANNELS; i++)
    {
        auto gpio = m_params.pwm_channels[i].gpio;
        if (gpio >= 0)
        {
            if (gpioSetPullUpDown(gpio, PI_PUD_DOWN) < 0)
            {
                QLOGE("GPIO {}: Cannot set pull down mode", gpio);
                return false;
            }
            if (gpioSetMode(gpio, PI_OUTPUT) < 0)
            {
                QLOGE("GPIO {}: Cannot set GPIO mode to output", gpio);
                return false;
            }
        }
    }

    //now configure the pin for PWM or servo
    for (size_t i = 0; i < MAX_PWM_CHANNELS; i++)
    {
        auto& channel = m_params.pwm_channels[i];
        int gpio = channel.gpio;
        if (gpio >= 0)
        {
            auto f = gpioSetPWMfrequency(gpio, channel.frequency);
            if (f < 0)
            {
                QLOGE("GPIO {}: Cannot set pwm frequency {}", gpio, channel.frequency);
                return false;
            }
            if (gpioSetPWMrange(gpio, channel.range) < 0)
            {
                QLOGE("GPIO {}: Cannot set pwm range {} on gpio {}", gpio, channel.range);
                return false;
            }
            QLOGI("GPIO {}: PWM frequency {} range {}", gpio, channel.frequency, channel.range);
        }
    }

    return true;
#else
    QLOGE("PIGPIO only supported on the raspberry pi");
    return false;
#endif
}

void PIGPIO::set_pwm_value(size_t idx, float value)
{
#if defined RASPBERRY_PI
    auto& channel = m_params.pwm_channels[idx];
    if (channel.gpio >= 0)
    {
        value = math::clamp(value, 0.f, 1.f);
        int pulse = value * (channel.max - channel.min);
        gpioPWM(channel.gpio, channel.min + pulse);
    }
#else
    QASSERT(0);
#endif
}

void PIGPIO::process()
{
}

}
}
}
