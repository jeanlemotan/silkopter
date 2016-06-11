#include "BrainStdAfx.h"
#include "PIGPIO.h"

#include "uav.def.h"

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

struct PIGPIO::Channel
{
    std::shared_ptr<const uav::PIGPIO_Config::IChannel> config;
    bool is_servo = false;
    uint32_t rate = 0;
    uint32_t gpio = 0;
    q::Path stream_path;
    std::weak_ptr<stream::IPWM> stream;
};


PIGPIO::PIGPIO(UAV& uav)
    : m_uav(uav)
    , m_descriptor(new uav::PIGPIO_Descriptor())
    , m_config(new uav::PIGPIO_Config())
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
        inputs.push_back({stream::IPWM::TYPE, channel->rate, q::util::format<std::string>("GPIO {}", channel->gpio), channel->stream_path});
    }
    return inputs;
}


auto PIGPIO::init(std::shared_ptr<uav::INode_Descriptor> descriptor) -> bool
{
    QLOG_TOPIC("pigpio::init");

    auto specialized = std::dynamic_pointer_cast<uav::PIGPIO_Descriptor>(descriptor);
    if (specialized)
    {
        QLOGE("Wrong descriptor type");
        return false;
    }
    *m_descriptor = *specialized;
    return init();
}

auto PIGPIO::init() -> bool
{
#define SETUP_CHANNEL(GPIO)\
    {\
        uav::PIGPIO_Descriptor::Channel const& channel_descriptor = m_descriptor->get_gpio_##GPIO();\
        if (channel_descriptor.get_enabled())\
        {\
            Channel ch;\
            ch.is_servo = channel_descriptor.get_servo_signal();\
            ch.rate = channel_descriptor.get_rate();\
            ch.gpio = GPIO;\
            if (ch.is_servo)\
            {\
                m_config->set_gpio_##GPIO(std::shared_ptr<uav::PIGPIO_Config::IChannel>(new uav::PIGPIO_Config::Servo_Channel));\
            }\
            else\
            {\
                m_config->set_gpio_##GPIO(std::shared_ptr<uav::PIGPIO_Config::IChannel>(new uav::PIGPIO_Config::PWM_Channel));\
            }\
            ch.config = m_config->get_gpio_##GPIO();\
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
    for (size_t i = 0; i < m_channels.size(); i++)
    {
        Channel const& channel = *m_channels[i];
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
    for (size_t i = 0; i < m_channels.size(); i++)
    {
        Channel const& channel = *m_channels[i];
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
    Channel const& channel = *m_channels[idx];
    if (channel.is_servo)
    {
        PIGPIO_Config::Servo_Channel const& config = *(PIGPIO_Config::Servo_Channel const*)m_channels[idx].config.get();
        value = math::clamp(value, 0.f, 1.f);
        float pulse = value * (config.max - config.min);
        gpioPWM(channel.gpio, (config.min + pulse) * 1000);
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



void PIGPIO::set_input_stream_path(size_t idx, q::Path const& path)
{
    auto input_stream = m_uav.get_streams().find_by_name<stream::IPWM>(path.get_as<std::string>());
    auto rate = input_stream ? input_stream->get_rate() : 0u;
    std::unique_ptr<Channel>& channel = m_channels[idx];
    if (rate != channel->rate)
    {
        if (input_stream)
        {
            QLOGW("Bad input stream '{}'. Expected rate {}Hz, got {}Hz", path, channel->rate, rate);
        }
        channel->stream.reset();
        channel->stream_path = q::Path();
    }
    else
    {
        channel->stream = input_stream;
        channel->stream_path = path;
    }
}


auto PIGPIO::set_config(std::shared_ptr<uav::INode_Config> config) -> bool
{
    QLOG_TOPIC("pigpio::set_config");

    auto specialized = std::dynamic_pointer_cast<uav::PIGPIO_Config>(config);
    if (!specialized)
    {
        QLOGE("Wrong config type");
        return false;
    }

    *m_config = *specialized;

    return true;
}
auto PIGPIO::get_config() const -> std::shared_ptr<uav::INode_Config>
{
    return m_config;
}

auto PIGPIO::get_descriptor() const -> std::shared_ptr<uav::INode_Descriptor>
{
    return m_descriptor;
}
//auto PIGPIO::send_message(rapidjson::Value const& /*json*/) -> rapidjson::Document
//{
//    return rapidjson::Document();
//}


}
}
