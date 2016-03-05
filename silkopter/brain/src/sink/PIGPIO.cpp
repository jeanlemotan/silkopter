#include "BrainStdAfx.h"
#include "PIGPIO.h"

#include "sz_math.hpp"
#include "sz_PIGPIO.hpp"

#ifdef RASPBERRY_PI
extern "C"
{
    #include "hw/pigpio.h"
}
extern std::chrono::microseconds PIGPIO_PERIOD;

#endif

constexpr float MIN_SERVO_MS = 0.5f;
constexpr float MAX_SERVO_MS = 2.4f;


//
//PIN:  13 12 11 10  9  8  7  6  5  4  3  2  1
//GPIO:  5        6 12 13 16 17 19    25 18 24

namespace silk
{
namespace node
{

PIGPIO::PIGPIO(HAL& hal)
    : m_hal(hal)
    , m_init_params(new sz::PIGPIO::Init_Params())
    , m_config(new sz::PIGPIO::Config())
{
}

PIGPIO::~PIGPIO()
{
}


auto PIGPIO::get_inputs() const -> std::vector<Input>
{
    std::vector<Input> inputs;
    for (PWM_Channel const& channel: m_pwm_channels)
    {
        inputs.push_back({stream::IPWM::TYPE, channel.rate, q::util::format<std::string>("GPIO {}", channel.gpio), channel.stream_path});
    }
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
    *m_init_params = sz;
    return init();
}

auto PIGPIO::init() -> bool
{
#define SETUP_CHANNEL(GPIO)\
    {\
        sz::PIGPIO::PWM_Channel* init_params = &m_init_params->gpio_##GPIO;\
        if (init_params->enabled)\
        {\
            PWM_Channel ch;\
            ch.is_servo = init_params->servo;\
            ch.rate = init_params->rate;\
            ch.gpio = GPIO;\
            ch.config = &m_config->gpio_##GPIO;\
            m_pwm_channels.push_back(ch);\
        }\
    }

    SETUP_CHANNEL(2);
    SETUP_CHANNEL(3);
    SETUP_CHANNEL(4);
    SETUP_CHANNEL(5);
    SETUP_CHANNEL(6);
    SETUP_CHANNEL(7);
    SETUP_CHANNEL(8);
    SETUP_CHANNEL(9);
    SETUP_CHANNEL(10);
    SETUP_CHANNEL(11);
    SETUP_CHANNEL(12);
    SETUP_CHANNEL(13);
    SETUP_CHANNEL(14);
    SETUP_CHANNEL(15);
    SETUP_CHANNEL(16);
    SETUP_CHANNEL(17);
    SETUP_CHANNEL(18);
    SETUP_CHANNEL(19);
    SETUP_CHANNEL(20);
    SETUP_CHANNEL(21);
    SETUP_CHANNEL(22);
    SETUP_CHANNEL(23);
    SETUP_CHANNEL(24);
    SETUP_CHANNEL(25);
    SETUP_CHANNEL(26);
    SETUP_CHANNEL(27);
#undef SETUP_CHANNEL


#if defined (RASPBERRY_PI)
    size_t period = PIGPIO_PERIOD.count();

    std::vector<size_t> rates = { 40000, 20000, 10000, 8000, 5000, 4000, 2500, 2000, 1600, 1250, 1000, 800, 500, 400, 250, 200, 100, 50 };
    for (auto& f: rates)
    {
        auto x = math::round(double(f) / double(period));
        f = static_cast<size_t>(x);
    }

    //first validate
    for (size_t i = 0; i < m_pwm_channels.size(); i++)
    {
        auto const& channel = m_pwm_channels[i];
        if (channel.is_servo && channel.rate > 400)
        {
            QLOGE("channel {} on GPIO {}: max rate for servo channels is 400Hz", i, channel.gpio);
            return false;
        }
//        if (config.min >= config.max)
//        {
//            QLOGE("channel {}: min ({}) is bigger than max ({})", channel.gpio, config.min, config.max);
//            return false;
//        }
//        uint32_t range = channel.is_servo ? (1000000 / channel.rate) : 1000000;
//        if (config.max > config.range)
//        {
//            QLOGE("channel {}: max ({}) is bigger than the range ({})", channel.gpio, config.max, config.range);
//            return false;
//        }
        if (std::find(rates.begin(), rates.end(), channel.rate) == rates.end())
        {
            QLOGE("channel {}: invalid rate {}. Supported are: {}", channel.gpio, channel.rate, rates);
            return false;
        }
    }

    //first of all turn on the pull-down to that if the board is reset of powerred off the motors don't start spinning
    //after a restart the GPIO pins are configured as inputs so their state is floating. Most of the time this results in a high pin
    for (size_t i = 0; i < m_pwm_channels.size(); i++)
    {
        auto const& channel = m_pwm_channels[i];
        if (gpioSetPullUpDown(channel.gpio, PI_PUD_DOWN) < 0)
        {
            QLOGE("channel {} on GPIO {}: Cannot set pull down mode", i, channel.gpio);
            return false;
        }
        if (gpioSetMode(channel.gpio, PI_OUTPUT) < 0)
        {
            QLOGE("channel {} on GPIO {}: Cannot set GPIO mode to output", i, channel.gpio);
            return false;
        }
     }

    //now configure the pin for PWM or servo
    for (size_t i = 0; i < m_pwm_channels.size(); i++)
    {
        auto const& channel = m_pwm_channels[i];
        auto f = gpioSetPWMfrequency(channel.gpio, channel.rate);
        if (f < 0)
        {
            QLOGE("channel {}: Cannot set pwm rate {}", channel.gpio, channel.rate);
            return false;
        }
        if (channel.is_servo)
        {
            uint32_t range = 1000000 / channel.rate;
            if (gpioSetPWMrange(channel.gpio, range) < 0)
            {
                QLOGE("channel {}: Cannot set pwm range {} on gpio {}", channel.gpio, range);
                return false;
            }
            QLOGI("SERVO Channel {}: gpio {}, rate {}, range {}", i, channel.gpio, channel.rate, range);
        }
        else
        {
            QLOGI("PWM Channel {}: gpio {}, rate {}", i, channel.gpio, channel.rate);
        }
    }

    return true;
#else
    QLOGE("PIGPIO only supported on the raspberry pi");
    return false;
#endif
}

auto PIGPIO::start(q::Clock::time_point tp) -> bool
{
    return true;
}

void PIGPIO::set_pwm_value(size_t idx, float value)
{
    QLOG_TOPIC("pigpio::set_pwm_value");

#if defined RASPBERRY_PI
    auto const& channel = m_pwm_channels[idx];
    if (channel.is_servo)
    {
        auto const& config = *m_pwm_channels[idx].config;
        value = math::clamp(value, 0.f, 1.f);
        float pulse = value * (config.max_servo - config.min_servo);
        gpioPWM(m_pwm_channels[idx].gpio, (config.min_servo + pulse) * 1000);
    }
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



void PIGPIO::set_input_stream_path(size_t idx, q::Path const& path)
{
    auto input_stream = m_hal.get_streams().find_by_name<stream::IPWM>(path.get_as<std::string>());
    auto rate = input_stream ? input_stream->get_rate() : 0u;
    if (rate != m_pwm_channels[idx].rate)
    {
        if (input_stream)
        {
            QLOGW("Bad input stream '{}'. Expected rate {}Hz, got {}Hz", path, m_pwm_channels[idx].rate, rate);
        }
        m_pwm_channels[idx].stream.reset();
        m_pwm_channels[idx].stream_path = q::Path();
    }
    else
    {
        m_pwm_channels[idx].stream = input_stream;
        m_pwm_channels[idx].stream_path = path;
    }
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

    for (size_t i = 2; i <= 27; i++)
    {
        auto it = std::find_if(m_pwm_channels.begin(), m_pwm_channels.end(), [i](PWM_Channel const& channel) { return channel.gpio == i; });
        if (it != m_pwm_channels.end())
        {
            PWM_Channel& channel = *it;
            if (it->is_servo)
            {
                channel.config->min_servo = math::clamp(channel.config->min_servo, MIN_SERVO_MS, MAX_SERVO_MS);
                channel.config->max_servo = math::clamp(channel.config->max_servo, channel.config->min_servo, MAX_SERVO_MS);
            }
            else
            {
                channel.config->min_pwm = math::clamp(channel.config->min_pwm, 0.f, 1.f);
                channel.config->max_pwm = math::clamp(channel.config->max_pwm, channel.config->min_pwm, 1.f);
            }
        }
    }

    return true;
}
auto PIGPIO::get_config() const -> rapidjson::Document
{
    rapidjson::Document json;
    autojsoncxx::to_document(*m_config, json);

    for (size_t i = 2; i <= 27; i++)
    {
        auto it = std::find_if(m_pwm_channels.begin(), m_pwm_channels.end(), [i](PWM_Channel const& channel) { return channel.gpio == i; });
        if (it != m_pwm_channels.end())
        {
            if (it->is_servo)
            {
                jsonutil::remove_value(json, q::Path(q::util::format<q::String>("GPIO {}/Min PWM", i)));
                jsonutil::remove_value(json, q::Path(q::util::format<q::String>("GPIO {}/Max PWM", i)));
            }
            else
            {
                jsonutil::remove_value(json, q::Path(q::util::format<q::String>("GPIO {}/Min Servo (ms)", i)));
                jsonutil::remove_value(json, q::Path(q::util::format<q::String>("GPIO {}/Max Servo (ms)", i)));
            }
        }
        else
        {
            jsonutil::remove_value(json, q::Path(q::util::format<q::String>("GPIO {}", i)));
        }
    }


    return std::move(json);
}

auto PIGPIO::get_init_params() const -> rapidjson::Document
{
    rapidjson::Document json;
    autojsoncxx::to_document(*m_init_params, json);
    return std::move(json);
}
auto PIGPIO::send_message(rapidjson::Value const& /*json*/) -> rapidjson::Document
{
    return rapidjson::Document();
}


}
}
