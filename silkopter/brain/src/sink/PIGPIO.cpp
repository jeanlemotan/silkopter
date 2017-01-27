#include "BrainStdAfx.h"
#include "PIGPIO.h"

#include "hal.def.h"

#ifdef RASPBERRY_PI
extern "C"
{
    #include "utils/hw/pigpio.h"
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

struct PIGPIO::Channel
{
    hal::PIGPIO_Config::IChannel const* config = nullptr;
    bool is_servo = false;
    uint32_t rate = 0;
    uint32_t gpio = 0;
    std::string stream_path;
    std::weak_ptr<stream::IPWM> stream;
};


PIGPIO::PIGPIO(HAL& hal)
    : m_hal(hal)
    , m_descriptor(new hal::PIGPIO_Descriptor())
    , m_config(new hal::PIGPIO_Config())
{
}

PIGPIO::~PIGPIO()
{
}


auto PIGPIO::get_inputs() const -> std::vector<Input>
{
    std::vector<Input> inputs;
    for (std::unique_ptr<Channel> const& channel: m_channels)
    {
        inputs.push_back({stream::IPWM::TYPE, channel->rate, q::util::format<std::string>("gpio_{}", channel->gpio), channel->stream_path});
    }
    return inputs;
}


ts::Result<void> PIGPIO::init(hal::INode_Descriptor const& descriptor)
{
    QLOG_TOPIC("pigpio::init");

    auto specialized = dynamic_cast<hal::PIGPIO_Descriptor const*>(&descriptor);
    if (!specialized)
    {
        return make_error("Wrong descriptor type");
    }
    *m_descriptor = *specialized;

    return init();
}

ts::Result<void> PIGPIO::init()
{
#define SETUP_CHANNEL(GPIO)\
    {\
        hal::PIGPIO_Descriptor::Channel const& channel_descriptor = m_descriptor->get_gpio_##GPIO();\
        if (channel_descriptor.get_enabled())\
        {\
            Channel ch;\
            ch.is_servo = channel_descriptor.get_servo_signal();\
            ch.rate = channel_descriptor.get_rate();\
            ch.gpio = GPIO;\
            if (ch.is_servo)\
            {\
                m_config->set_gpio_##GPIO(hal::Poly<hal::PIGPIO_Config::IChannel>(new hal::PIGPIO_Config::Servo_Channel));\
            }\
            else\
            {\
                m_config->set_gpio_##GPIO(hal::Poly<hal::PIGPIO_Config::IChannel>(new hal::PIGPIO_Config::PWM_Channel));\
            }\
            ch.config = m_config->get_gpio_##GPIO().get();\
            m_channels.emplace_back(new Channel(ch));\
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
    for (size_t i = 0; i < m_channels.size(); i++)
    {
        Channel const& channel = *m_channels[i];
        if (channel.is_servo && channel.rate > 400)
        {
            return make_error("channel {} on GPIO {}: max rate for servo channels is 400Hz", i, channel.gpio);
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
            return make_error("channel {}: invalid rate {}. Supported are: {}", channel.gpio, channel.rate, rates);
        }
    }

    //first of all turn on the pull-down to that if the board is reset of powerred off the motors don't start spinning
    //after a restart the GPIO pins are configured as inputs so their state is floating. Most of the time this results in a high pin
    for (size_t i = 0; i < m_channels.size(); i++)
    {
        Channel const& channel = *m_channels[i];
        if (gpioSetPullUpDown(channel.gpio, PI_PUD_DOWN) < 0)
        {
            return make_error("channel {} on GPIO {}: Cannot set pull down mode", i, channel.gpio);
        }
        if (gpioSetMode(channel.gpio, PI_OUTPUT) < 0)
        {
            return make_error("channel {} on GPIO {}: Cannot set GPIO mode to output", i, channel.gpio);
        }
     }

    //now configure the pin for PWM or servo
    for (size_t i = 0; i < m_channels.size(); i++)
    {
        Channel const& channel = *m_channels[i];
        auto f = gpioSetPWMfrequency(channel.gpio, channel.rate);
        if (f < 0)
        {
            return make_error("channel {}: Cannot set pwm rate {}", channel.gpio, channel.rate);
        }
        if (channel.is_servo)
        {
            uint32_t range = 1000000 / channel.rate;
            if (gpioSetPWMrange(channel.gpio, range) < 0)
            {
                return make_error("channel {}: Cannot set pwm range {} on gpio {}", channel.gpio, range);
            }
            QLOGI("SERVO Channel {}: gpio {}, rate {}, range {}", i, channel.gpio, channel.rate, range);
        }
        else
        {
            QLOGI("PWM Channel {}: gpio {}, rate {}", i, channel.gpio, channel.rate);
        }
    }

    return ts::success;
#else
    return make_error("PIGPIO only supported on the raspberry pi");
#endif
}

ts::Result<void> PIGPIO::start(Clock::time_point tp)
{
    return ts::success;
}

void PIGPIO::set_pwm_value(size_t idx, float value)
{
    QLOG_TOPIC("pigpio::set_pwm_value");

#if defined RASPBERRY_PI
    Channel const& channel = *m_channels[idx];
    if (channel.is_servo)
    {
        hal::PIGPIO_Config::Servo_Channel const* config = static_cast<hal::PIGPIO_Config::Servo_Channel const*>(channel.config);
        value = math::clamp(value, 0.f, 1.f);
        float pulse = value * (config->get_max() - config->get_min());
        gpioPWM(channel.gpio, (config->get_min() + pulse) * 1000);
    }
    else
    {
        QASSERT(0);
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

    for (size_t i = 0; i < m_channels.size(); i++)
    {
        std::unique_ptr<Channel> const& ch = m_channels[i];
        auto stream = ch->stream.lock();
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



ts::Result<void> PIGPIO::set_input_stream_path(size_t idx, std::string const& path)
{
    auto input_stream = m_hal.get_stream_registry().find_by_name<stream::IPWM>(path);
    auto rate = input_stream ? input_stream->get_rate() : 0u;
    std::unique_ptr<Channel>& channel = m_channels[idx];
    if (rate != channel->rate)
    {
        channel->stream.reset();
        channel->stream_path = std::string();
        if (input_stream)
        {
            return make_error("Bad input stream '{}'. Expected rate {}Hz, got {}Hz", path, channel->rate, rate);
        }
    }
    else
    {
        channel->stream = input_stream;
        channel->stream_path = path;
    }
    return ts::success;
}


ts::Result<void> PIGPIO::set_config(hal::INode_Config const& config)
{
    QLOG_TOPIC("pigpio::set_config");

    auto specialized = dynamic_cast<hal::PIGPIO_Config const*>(&config);
    if (!specialized)
    {
        return make_error("Wrong config type");
    }
    *m_config = *specialized;

    std::map<size_t, hal::PIGPIO_Config::IChannel const*> channel_configs_by_gpio;

#define ADD_CHANNEL(GPIO)\
    channel_configs_by_gpio[GPIO] = m_config->get_gpio_##GPIO().get();

    ADD_CHANNEL(2);
    ADD_CHANNEL(3);
    ADD_CHANNEL(4);
    ADD_CHANNEL(5);
    ADD_CHANNEL(6);
    ADD_CHANNEL(7);
    ADD_CHANNEL(8);
    ADD_CHANNEL(9);
    ADD_CHANNEL(10);
    ADD_CHANNEL(11);
    ADD_CHANNEL(12);
    ADD_CHANNEL(13);
    ADD_CHANNEL(14);
    ADD_CHANNEL(15);
    ADD_CHANNEL(16);
    ADD_CHANNEL(17);
    ADD_CHANNEL(18);
    ADD_CHANNEL(19);
    ADD_CHANNEL(20);
    ADD_CHANNEL(21);
    ADD_CHANNEL(22);
    ADD_CHANNEL(23);
    ADD_CHANNEL(24);
    ADD_CHANNEL(25);
    ADD_CHANNEL(26);
    ADD_CHANNEL(27);
#undef ADD_CHANNEL

    //relink channels, as they have changed pointer for sure after reassigning them from the *specialized data structure!
    for (std::unique_ptr<Channel>& channel: m_channels)
    {
        channel->config = channel_configs_by_gpio[channel->gpio];
    }

    return ts::success;
}
auto PIGPIO::get_config() const -> std::shared_ptr<const hal::INode_Config>
{
    return m_config;
}

auto PIGPIO::get_descriptor() const -> std::shared_ptr<const hal::INode_Descriptor>
{
    return m_descriptor;
}
//auto PIGPIO::send_message(rapidjson::Value const& /*json*/) -> rapidjson::Document
//{
//    return rapidjson::Document();
//}


}
}
