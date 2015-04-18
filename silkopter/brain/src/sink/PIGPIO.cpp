#include "BrainStdAfx.h"
#include "PIGPIO.h"

#include "sz_math.hpp"
#include "sz_PIGPIO.hpp"

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

PIGPIO::PIGPIO(HAL& hal)
    : m_hal(hal)
    , m_init_params(new sz::PIGPIO::Init_Params())
    , m_config(new sz::PIGPIO::Config())
{
    autojsoncxx::to_document(*m_init_params, m_init_paramsj);
}

auto PIGPIO::get_inputs() const -> std::vector<Input>
{
    std::vector<Input> inputs =
    {{
        { stream::IPWM::TYPE, m_init_params->channel_4.rate, "Channel 4" },
        { stream::IPWM::TYPE, m_init_params->channel_17.rate, "Channel 17" },
        { stream::IPWM::TYPE, m_init_params->channel_18.rate, "Channel 18" },
        { stream::IPWM::TYPE, m_init_params->channel_22.rate, "Channel 22" },
        { stream::IPWM::TYPE, m_init_params->channel_23.rate, "Channel 23" },
        { stream::IPWM::TYPE, m_init_params->channel_24.rate, "Channel 24" },
        { stream::IPWM::TYPE, m_init_params->channel_25.rate, "Channel 25" },
        { stream::IPWM::TYPE, m_init_params->channel_27.rate, "Channel 27" }
    }};
    return inputs;
}


auto PIGPIO::init(rapidjson::Value const& init_params) -> bool
{
    QLOG_TOPIC("pigpio::init");

    sz::PIGPIO::Init_Params sz;
    autojsoncxx::error::ErrorStack result;
    if (!autojsoncxx::from_value(sz, init_params, result))
    {
        std::ostringstream ss;
        ss << result;
        QLOGE("Cannot deserialize PIGPIO data: {}", ss.str());
        return false;
    }
    jsonutil::clone_value(m_init_paramsj, init_params, m_init_paramsj.GetAllocator());
    *m_init_params = sz;
    return init();
}

auto PIGPIO::init() -> bool
{
    size_t period = m_init_params->period_micro;

    std::vector<size_t> periods = {1, 2, 4, 5, 8, 10};
    if (std::find(periods.begin(), periods.end(), period) == periods.end())
    {
        QLOGE("Invalid period {}. Supported are: {}", period, periods);
        return false;
    }

    std::vector<size_t> rates = { 40000, 20000, 10000, 8000, 5000, 4000, 2500, 2000, 1600, 1250, 1000, 800, 500, 400, 250, 200, 100, 50 };
    for (auto& f: rates)
    {
        auto x = math::round(double(f) / double(period));
        f = static_cast<size_t>(x);
    }

    m_pwm_channels.resize(8);
    m_pwm_channels[0].gpio = 4;
    m_pwm_channels[0].config = &m_init_params->channel_4;
    m_pwm_channels[1].gpio = 17;
    m_pwm_channels[1].config = &m_init_params->channel_17;
    m_pwm_channels[2].gpio = 18;
    m_pwm_channels[2].config = &m_init_params->channel_18;
    m_pwm_channels[3].gpio = 22;
    m_pwm_channels[3].config = &m_init_params->channel_22;
    m_pwm_channels[4].gpio = 23;
    m_pwm_channels[4].config = &m_init_params->channel_23;
    m_pwm_channels[5].gpio = 24;
    m_pwm_channels[5].config = &m_init_params->channel_24;
    m_pwm_channels[6].gpio = 25;
    m_pwm_channels[6].config = &m_init_params->channel_25;
    m_pwm_channels[7].gpio = 27;
    m_pwm_channels[7].config = &m_init_params->channel_27;

#if defined (RASPBERRY_PI)

    //first validate
    for (size_t i = 0; i < m_pwm_channels.size(); i++)
    {
        auto const& ch = *m_pwm_channels[i].config;
        auto gpio = m_pwm_channels[i].gpio;
        if (ch.min >= ch.max)
        {
            QLOGE("channel {}: min ({}) is bigger than max ({})", gpio, ch.min, ch.max);
            return false;
        }
        if (ch.max > ch.range)
        {
            QLOGE("channel {}: max ({}) is bigger than the range ({})", gpio, ch.max, ch.range);
            return false;
        }
        if (std::find(rates.begin(), rates.end(), ch.rate) == rates.end())
        {
            QLOGE("channel {}: invalid rate {}. Supported are: {}", gpio, ch.rate, rates);
            return false;
        }
    }

    QLOGI("Initializing pigpio");
    if (gpioCfgClock(period, 1, 0) < 0 ||
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
    for (size_t i = 0; i < m_pwm_channels.size(); i++)
    {
        auto gpio = m_pwm_channels[i].gpio;
        if (gpioSetPullUpDown(gpio, PI_PUD_DOWN) < 0)
        {
            QLOGE("channel {} on GPIO {}: Cannot set pull down mode", i, gpio);
            return false;
        }
        if (gpioSetMode(gpio, PI_OUTPUT) < 0)
        {
            QLOGE("channel {} on GPIO {}: Cannot set GPIO mode to output", i, gpio);
            return false;
        }
     }

    //now configure the pin for PWM or servo
    for (size_t i = 0; i < m_pwm_channels.size(); i++)
    {
        auto const& ch = *m_pwm_channels[i].config;
        auto gpio = m_pwm_channels[i].gpio;
        auto f = gpioSetPWMfrequency(gpio, ch.rate);
        if (f < 0)
        {
            QLOGE("channel {}: Cannot set pwm rate {}", gpio, ch.rate);
            return false;
        }
        if (gpioSetPWMrange(gpio, ch.range) < 0)
        {
            QLOGE("channel {}: Cannot set pwm range {} on gpio {}", gpio, ch.range);
            return false;
        }
        QLOGI("channel {}: rate {} range {}", gpio, ch.rate, ch.range);
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
    auto const& ch = *m_pwm_channels[idx].config;
    value = math::clamp(value, 0.f, 1.f);
    int pulse = value * (ch.max - ch.min);
    gpioPWM(m_pwm_channels[idx].gpio, ch.min + pulse);
#else
    QUNUSED(idx);
    QUNUSED(value);
    QASSERT(0);
#endif
}

void PIGPIO::process()
{
    QLOG_TOPIC("pigpio::process");

    for (size_t i = 0; i < m_pwm_channels.size(); i++)
    {
        auto& ch = m_pwm_channels[i];
        auto stream = ch.stream.lock();
        if (stream)
        {
            auto const& samples = stream->get_samples();
            if (!samples.empty())
            {
//                if (samples.size() > 20)
//                {
//                    QLOGW("channel {} on GPIO {} is too slow. {} samples are queued", i, ch.gpio, samples.size());
//                }

                set_pwm_value(i, samples.back().value);
            }
        }
    }
}


#define READ_CONFIG(IDX, CH)\
{\
    auto input_stream = m_hal.get_streams().find_by_name<stream::IPWM>(sz.inputs.channel_##CH);\
    auto rate = input_stream ? input_stream->get_rate() : 0u;\
    if (rate != m_init_params->channel_##CH.rate)\
    {\
        m_config->inputs.channel_##CH.clear();\
        QLOGW("Bad input stream '{}'. Expected rate {}Hz, got {}Hz", sz.inputs.channel_##CH, m_init_params->channel_##CH.rate, rate);\
        m_pwm_channels[IDX].stream.reset();\
    }\
    else\
    {\
        m_pwm_channels[IDX].stream = input_stream;\
    }\
}


auto PIGPIO::set_config(rapidjson::Value const& json) -> bool
{
    QLOG_TOPIC("pigpio::set_config");

    sz::PIGPIO::Config sz;
    autojsoncxx::error::ErrorStack result;
    if (!autojsoncxx::from_value(sz, json, result))
    {
        std::ostringstream ss;
        ss << result;
        QLOGE("Cannot deserialize PIGPIO config data: {}", ss.str());
        return false;
    }

    *m_config = sz;

    READ_CONFIG(0, 4);
    READ_CONFIG(1, 17);
    READ_CONFIG(2, 18);
    READ_CONFIG(3, 22);
    READ_CONFIG(4, 23);
    READ_CONFIG(5, 24);
    READ_CONFIG(6, 25);
    READ_CONFIG(7, 27);

    return true;
}
auto PIGPIO::get_config() const -> rapidjson::Document
{
    rapidjson::Document json;
    autojsoncxx::to_document(*m_config, json);
    return std::move(json);
}

auto PIGPIO::get_init_params() const -> rapidjson::Document const&
{
    return m_init_paramsj;
}
auto PIGPIO::send_message(rapidjson::Value const& /*json*/) -> rapidjson::Document
{
    return rapidjson::Document();
}


}
}
