#include "BrainStdAfx.h"
#include "PCA9685.h"

#include "sz_math.hpp"
#include "sz_PCA9685.hpp"


namespace silk
{
namespace node
{

PCA9685::PCA9685(HAL& hal)
    : m_hal(hal)
    , m_init_params(new sz::PCA9685::Init_Params())
    , m_config(new sz::PCA9685::Config())
{
    autojsoncxx::to_document(*m_init_params, m_init_paramsj);
}

auto PCA9685::get_inputs() const -> std::vector<Input>
{
    std::vector<Input> inputs =
    {{
        { stream::IPWM::TYPE, m_init_params->channel_1.rate, "Channel 1" },
        { stream::IPWM::TYPE, m_init_params->channel_2.rate, "Channel 2" },
        { stream::IPWM::TYPE, m_init_params->channel_3.rate, "Channel 3" },
        { stream::IPWM::TYPE, m_init_params->channel_4.rate, "Channel 4" },
        { stream::IPWM::TYPE, m_init_params->channel_5.rate, "Channel 5" },
        { stream::IPWM::TYPE, m_init_params->channel_6.rate, "Channel 6" },
        { stream::IPWM::TYPE, m_init_params->channel_7.rate, "Channel 7" },
        { stream::IPWM::TYPE, m_init_params->channel_8.rate, "Channel 8" },
        { stream::IPWM::TYPE, m_init_params->channel_9.rate, "Channel 9" },
        { stream::IPWM::TYPE, m_init_params->channel_10.rate, "Channel 10" },
        { stream::IPWM::TYPE, m_init_params->channel_11.rate, "Channel 11" },
        { stream::IPWM::TYPE, m_init_params->channel_12.rate, "Channel 12" },
        { stream::IPWM::TYPE, m_init_params->channel_13.rate, "Channel 13" },
        { stream::IPWM::TYPE, m_init_params->channel_14.rate, "Channel 14" },
        { stream::IPWM::TYPE, m_init_params->channel_15.rate, "Channel 15" },
        { stream::IPWM::TYPE, m_init_params->channel_16.rate, "Channel 16" },
    }};
    return inputs;
}


auto PCA9685::init(rapidjson::Value const& init_params) -> bool
{
    QLOG_TOPIC("PCA9685::init");

    sz::PCA9685::Init_Params sz;
    autojsoncxx::error::ErrorStack result;
    if (!autojsoncxx::from_value(sz, init_params, result))
    {
        std::ostringstream ss;
        ss << result;
        QLOGE("Cannot deserialize PCA9685 data: {}", ss.str());
        return false;
    }
    jsonutil::clone_value(m_init_paramsj, init_params, m_init_paramsj.GetAllocator());
    *m_init_params = sz;
    return init();
}

auto PCA9685::init() -> bool
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

    QLOGI("Initializing PCA9685");
    if (gpioCfgClock(period, 1, 0) < 0 ||
        gpioCfgPermissions(static_cast<uint64_t>(-1)) ||
        gpioCfgInterfaces(PI_DISABLE_SOCK_IF | PI_DISABLE_FIFO_IF))
    {
        QLOGE("Cannot configure PCA9685");
        return false;
    }
    if (gpioInitialise() < 0)
    {
        QLOGE("Cannot initialize PCA9685");
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
    QLOGE("PCA9685 only supported on the raspberry pi");
    return false;
#endif
}

void PCA9685::set_pwm_value(size_t idx, float value)
{
    QLOG_TOPIC("PCA9685::set_pwm_value");

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

void PCA9685::process()
{
    QLOG_TOPIC("PCA9685::process");

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


auto PCA9685::set_config(rapidjson::Value const& json) -> bool
{
    QLOG_TOPIC("PCA9685::set_config");

    sz::PCA9685::Config sz;
    autojsoncxx::error::ErrorStack result;
    if (!autojsoncxx::from_value(sz, json, result))
    {
        std::ostringstream ss;
        ss << result;
        QLOGE("Cannot deserialize PCA9685 config data: {}", ss.str());
        return false;
    }

    *m_config = sz;

    READ_CONFIG(0, 1);
    READ_CONFIG(1, 2);
    READ_CONFIG(2, 3);
    READ_CONFIG(3, 4);
    READ_CONFIG(4, 5);
    READ_CONFIG(5, 6);
    READ_CONFIG(6, 7);
    READ_CONFIG(7, 8);
    READ_CONFIG(8, 9);
    READ_CONFIG(9, 10);
    READ_CONFIG(10, 11);
    READ_CONFIG(11, 12);
    READ_CONFIG(12, 13);
    READ_CONFIG(13, 14);
    READ_CONFIG(14, 15);
    READ_CONFIG(15, 16);

    return true;
}
auto PCA9685::get_config() const -> rapidjson::Document
{
    rapidjson::Document json;
    autojsoncxx::to_document(*m_config, json);
    return std::move(json);
}

auto PCA9685::get_init_params() const -> rapidjson::Document const&
{
    return m_init_paramsj;
}
auto PCA9685::send_message(rapidjson::Value const& /*json*/) -> rapidjson::Document
{
    return rapidjson::Document();
}


}
}
