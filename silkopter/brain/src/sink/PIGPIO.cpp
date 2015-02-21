#include "BrainStdAfx.h"
#include "PIGPIO.h"

#include "sz_math.hpp"
#include "sz_hal_nodes.hpp"

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

const size_t PIGPIO::MAX_PWM_CHANNELS;


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

auto PIGPIO::init(rapidjson::Value const& json) -> bool
{
    QLOG_TOPIC("pigpio::init");

    sz::PIGPIO sz;
    autojsoncxx::error::ErrorStack result;
    if (!autojsoncxx::from_value(sz, json, result))
    {
        std::ostringstream ss;
        ss << result;
        QLOGE("Cannot deserialize PIGPIO data: {}", ss.str());
        return false;
    }
    Init_Params params;
    params.name = sz.name;
    params.period = std::chrono::microseconds(sz.period_micro);
    params.pwm_channels.resize(sz.pwm_channels.size());
    for (size_t i = 0; i < sz.pwm_channels.size(); i++)
    {
        auto& ch = params.pwm_channels[i];
        ch.stream = m_hal.get_streams().find_by_name<stream::IPWM_Value>(sz.pwm_channels[i].stream);
        ch.gpio = sz.pwm_channels[i].gpio;
        ch.frequency = sz.pwm_channels[i].frequency;
        ch.range = sz.pwm_channels[i].range;
        ch.min = sz.pwm_channels[i].min;
        ch.max = sz.pwm_channels[i].max;
    }
    return init(params);
}
auto PIGPIO::init(Init_Params const& params) -> bool
{
    QLOG_TOPIC("pigpio::init");

    m_params = params;

    if (!init())
    {
        return false;
    }

    if (!m_params.name.empty())
    {
        for (size_t i = 0; i < MAX_PWM_CHANNELS; i++)
        {
            if (m_params.pwm_channels[i].gpio >= 0)
            {
                auto& pwm = m_pwm_channels[i];
                pwm.name = q::util::format2<std::string>("{}-pwm{}", m_params.name, i);
                if (!m_hal.get_sinks().add(pwm))
                {
                    return false;
                }
            }
        }
    }
}

auto PIGPIO::init() -> bool
{
    QLOG_TOPIC("pigpio::init");

#if defined (RASPBERRY_PI)
    if (m_params.pwm_channels.size() > MAX_PWM_CHANNELS)
    {
        QLOGE("Too many PWM channels. Max is {}", MAX_PWM_CHANNELS);
        return false;
    }

    size_t rate = m_params.period.count();

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
        auto& ch = m_params.pwm_channels[i];
        if (ch.min >= ch.max)
        {
            QLOGE("PIGPIO PWM channel {}: min ({}) is bigger than max ({})", i, ch.min, ch.max);
            return false;
        }
        if (ch.max > ch.range)
        {
            QLOGE("PIGPIO PWM channel {}: max ({}) is bigger than the range ({})", i, ch.max, ch.range);
            return false;
        }
        if (std::find(frequencies.begin(), frequencies.end(), ch.frequency) == frequencies.end())
        {
            QLOGE("Invalid frequency {}. Supported are: {}", ch.frequency, frequencies);
            return false;
        }
        if (!ch.stream)
        {
            QLOGE("PIGPIO PWM channel {} doesn't have a stream", i);
            return false;
        }
        if (ch.stream->get_rate() > ch.frequency)
        {
            QLOGE("PIGPIO stream {} is too fast for the PWM channel {}: {}Hz > {}Hz", ch.stream->get_name(), i, ch.stream->get_rate(), ch.frequency);
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
        auto& ch = m_params.pwm_channels[i];
        int gpio = ch.gpio;
        if (gpio >= 0)
        {
            auto f = gpioSetPWMfrequency(gpio, ch.frequency);
            if (f < 0)
            {
                QLOGE("GPIO {}: Cannot set pwm frequency {}", gpio, ch.frequency);
                return false;
            }
            if (gpioSetPWMrange(gpio, ch.range) < 0)
            {
                QLOGE("GPIO {}: Cannot set pwm range {} on gpio {}", gpio, ch.range);
                return false;
            }
            QLOGI("GPIO {}: PWM frequency {} range {}", gpio, ch.frequency, ch.range);
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
    QLOG_TOPIC("pigpio::set_pwm_value");

#if defined RASPBERRY_PI
    auto& ch = m_params.pwm_channels[idx];
    if (ch.gpio >= 0)
    {
        value = math::clamp(value, 0.f, 1.f);
        int pulse = value * (ch.max - ch.min);
        gpioPWM(ch.gpio, ch.min + pulse);
    }
#else
    QASSERT(0);
#endif
}

void PIGPIO::process()
{
    QLOG_TOPIC("pigpio::process");

    for (size_t i = 0; i < MAX_PWM_CHANNELS; i++)
    {
        auto& ch = m_params.pwm_channels[i];
        int gpio = ch.gpio;
        if (gpio >= 0)
        {
            QASSERT(ch.stream);
            auto const& samples = ch.stream->get_samples();
            if (!samples.empty())
            {
                if (samples.size() > 2)
                {
                    QLOGW("PWM channel {} is too slow. {} samples are queued", i, samples.size());
                }

                set_pwm_value(i, samples.back().value);
            }
        }
    }
}

}
}
}
