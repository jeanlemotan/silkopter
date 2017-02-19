#include "FCStdAfx.h"
#include "PCA9685.h"

#include "hal.def.h"
//#include "sz_PCA9685.hpp"

#ifdef RASPBERRY_PI
extern "C"
{
    #include "utils/hw/bcm2835.h"
}

#endif


namespace silk
{
namespace node
{


constexpr uint8_t PCA9685_RA_MODE1            = 0x00;
constexpr uint8_t PCA9685_RA_MODE2            = 0x01;
constexpr uint8_t PCA9685_RA_LED0_ON_L        = 0x06;
constexpr uint8_t PCA9685_RA_LED0_ON_H        = 0x07;
constexpr uint8_t PCA9685_RA_LED0_OFF_L       = 0x08;
constexpr uint8_t PCA9685_RA_LED0_OFF_H       = 0x09;
constexpr uint8_t PCA9685_RA_ALL_LED_ON_L     = 0xFA;
constexpr uint8_t PCA9685_RA_ALL_LED_ON_H     = 0xFB;
constexpr uint8_t PCA9685_RA_ALL_LED_OFF_L    = 0xFC;
constexpr uint8_t PCA9685_RA_ALL_LED_OFF_H    = 0xFD;
constexpr uint8_t PCA9685_RA_PRE_SCALE        = 0xFE;

constexpr uint8_t PCA9685_MODE1_RESTART_BIT   = 1 << 7;
constexpr uint8_t PCA9685_MODE1_EXTCLK_BIT    = 1 << 6;
constexpr uint8_t PCA9685_MODE1_AI_BIT        = 1 << 5;
constexpr uint8_t PCA9685_MODE1_SLEEP_BIT     = 1 << 4;
constexpr uint8_t PCA9685_MODE1_SUB1_BIT      = 1 << 3;
constexpr uint8_t PCA9685_MODE1_SUB2_BIT      = 1 << 2;
constexpr uint8_t PCA9685_MODE1_SUB3_BIT      = 1 << 1;
constexpr uint8_t PCA9685_MODE1_ALLCALL_BIT   = 1 << 0;

constexpr uint8_t PCA9685_MODE2_INVRT_BIT     = 1 << 4;
constexpr uint8_t PCA9685_MODE2_OCH_BIT       = 1 << 3;
constexpr uint8_t PCA9685_MODE2_OUTDRV_BIT    = 1 << 2;
constexpr uint8_t PCA9685_MODE2_OUTNE1_BIT    = 1 << 1;
constexpr uint8_t PCA9685_MODE2_OUTNE0_BIT    = 1 << 0;

std::mutex PCA9685::s_pwm_enabled_mutex;
size_t PCA9685::s_pwm_enabled_count = 0;


PCA9685::PCA9685(HAL& hal)
    : m_hal(hal)
    , m_descriptor(new hal::PCA9685_Descriptor())
    , m_config(new hal::PCA9685_Config())
{
    m_pwm_channels.resize(16);
}

PCA9685::~PCA9685()
{
    set_all_pwm_enabled(false);
}

auto PCA9685::get_inputs() const -> std::vector<Input>
{
    std::vector<Input> inputs =
    {{
        { stream::IPWM::TYPE, m_descriptor->get_rate(), "channel_1", m_pwm_channels[0].stream_path },
        { stream::IPWM::TYPE, m_descriptor->get_rate(), "channel_2", m_pwm_channels[1].stream_path },
        { stream::IPWM::TYPE, m_descriptor->get_rate(), "channel_3", m_pwm_channels[2].stream_path },
        { stream::IPWM::TYPE, m_descriptor->get_rate(), "channel_4", m_pwm_channels[3].stream_path },
        { stream::IPWM::TYPE, m_descriptor->get_rate(), "channel_5", m_pwm_channels[4].stream_path },
        { stream::IPWM::TYPE, m_descriptor->get_rate(), "channel_6", m_pwm_channels[5].stream_path },
        { stream::IPWM::TYPE, m_descriptor->get_rate(), "channel_7", m_pwm_channels[6].stream_path },
        { stream::IPWM::TYPE, m_descriptor->get_rate(), "channel_8", m_pwm_channels[7].stream_path },
        { stream::IPWM::TYPE, m_descriptor->get_rate(), "channel_9", m_pwm_channels[8].stream_path },
        { stream::IPWM::TYPE, m_descriptor->get_rate(), "channel_10",m_pwm_channels[9].stream_path },
        { stream::IPWM::TYPE, m_descriptor->get_rate(), "channel_11",m_pwm_channels[10].stream_path },
        { stream::IPWM::TYPE, m_descriptor->get_rate(), "channel_12",m_pwm_channels[11].stream_path },
        { stream::IPWM::TYPE, m_descriptor->get_rate(), "channel_13",m_pwm_channels[12].stream_path },
        { stream::IPWM::TYPE, m_descriptor->get_rate(), "channel_14",m_pwm_channels[13].stream_path },
        { stream::IPWM::TYPE, m_descriptor->get_rate(), "channel_15",m_pwm_channels[14].stream_path },
        { stream::IPWM::TYPE, m_descriptor->get_rate(), "channel_16",m_pwm_channels[15].stream_path },
    }};
    return inputs;
}


ts::Result<void> PCA9685::init(hal::INode_Descriptor const& descriptor)
{
    QLOG_TOPIC("PCA9685::init");

    auto specialized = dynamic_cast<hal::PCA9685_Descriptor const*>(&descriptor);
    if (!specialized)
    {
        return make_error("Wrong descriptor type");
    }
    *m_descriptor = *specialized;

    return init();
}

ts::Result<void> PCA9685::init()
{
    m_i2c = m_hal.get_bus_registry().find_by_name<bus::II2C>(m_descriptor->get_bus());
    auto i2c = m_i2c.lock();
    if (!i2c)
    {
        return make_error("No bus configured");
    }

    //todo - fix this
//    if (m_descriptor->address > 127)
//    {
//        QLOGE("Invalid address {}", m_descriptor->address);
//        return false;
//    }

//    //                            4     5     6     8    10   12   15   20   24   30   40   60   75  80, 100 120 150 200 240
//    std::vector<size_t> rates = { 1500, 1200, 1000, 750, 600, 500, 400, 300, 250, 200, 150, 100, 80, 75, 60, 50, 40, 30, 25 };
//    if (std::find(rates.begin(), rates.end(), m_descriptor->rate) == rates.end())
//    {
//        QLOGE("Invalid rate {}. Supported are: {}", m_descriptor->rate, rates);
//        return false;
//    }

//    bool res = i2c->write_register_u8(m_descriptor->address, PCA9685_RA_MODE1, PCA9685_MODE1_AI_BIT);
//    res &= restart(*i2c);

//    uint8_t data;
//    res &= i2c->read_register_u8(m_descriptor->address, PCA9685_RA_PRE_SCALE, data);
//    if (!res && data <= 3)
//    {
//        QLOGE("PCA9685 not found on {}, address {}", m_descriptor->bus, m_descriptor->address);
//        return false;
//    }

//    //set sleep mode so we can change the prescaler
//    i2c->write_register_u8(m_descriptor->address, PCA9685_RA_MODE1, PCA9685_MODE1_SLEEP_BIT);
//    std::this_thread::sleep_for(std::chrono::milliseconds(10));

//    uint8_t prescale = math::round(24576000.0 / 4096.0 / m_descriptor->rate) - 1;
//    res &= i2c->write_register_u8(m_descriptor->address, PCA9685_RA_PRE_SCALE, prescale);

//    //restart to activate the new prescaler
//    res &= restart(*i2c);
//    if (!res)
//    {
//        QLOGE("I2C error while setting rate {}, prescale {}", m_descriptor->rate, prescale);
//        return false;
//    }

//    m_pwm_channels[0].config = &m_config->channel_1;
//    m_pwm_channels[1].config = &m_config->channel_2;
//    m_pwm_channels[2].config = &m_config->channel_3;
//    m_pwm_channels[3].config = &m_config->channel_4;
//    m_pwm_channels[4].config = &m_config->channel_5;
//    m_pwm_channels[5].config = &m_config->channel_6;
//    m_pwm_channels[6].config = &m_config->channel_7;
//    m_pwm_channels[7].config = &m_config->channel_8;
//    m_pwm_channels[8].config = &m_config->channel_9;
//    m_pwm_channels[9].config = &m_config->channel_10;
//    m_pwm_channels[10].config = &m_config->channel_11;
//    m_pwm_channels[11].config = &m_config->channel_12;
//    m_pwm_channels[12].config = &m_config->channel_13;
//    m_pwm_channels[13].config = &m_config->channel_14;
//    m_pwm_channels[14].config = &m_config->channel_15;
//    m_pwm_channels[15].config = &m_config->channel_16;

//    //reseet all channels
//    for (size_t i = 0; i < m_pwm_channels.size(); i++)
//    {
//        set_pwm_value(*i2c, i, boost::none);
//    }

    return ts::success;
}

ts::Result<void> PCA9685::start(Clock::time_point tp)
{
    if (!set_all_pwm_enabled(true))
    {
        return make_error("Cannot enable global PWM!");
    }
    return ts::success;
}

auto PCA9685::set_all_pwm_enabled(bool val) -> bool
{
    if (val == m_has_enabled_all_pwm)
    {
        return true;
    }
    m_has_enabled_all_pwm = val;

    std::lock_guard<std::mutex> lg(s_pwm_enabled_mutex);

    s_pwm_enabled_count += val ? 1 : -1;
    bool is_enabled = s_pwm_enabled_count > 0;

#if defined RASPBERRY_PI
    QLOGI("Setting all pwm to {}", is_enabled ? "enabled" : "disabled");

    bcm2835_gpio_fsel(27, BCM2835_GPIO_FSEL_OUTP);
    bcm2835_gpio_write(27, is_enabled ? LEVEL_LOW : LEVEL_HIGH); //inverted output
#endif

    return true;
}

auto PCA9685::restart(bus::II2C& i2c) -> bool
{
    bool res = i2c.write_register_u8(m_descriptor->get_address(), PCA9685_RA_MODE1, PCA9685_MODE1_SLEEP_BIT);
    std::this_thread::sleep_for(std::chrono::milliseconds(2));
    res &= i2c.write_register_u8(m_descriptor->get_address(), PCA9685_RA_MODE1, PCA9685_MODE1_SLEEP_BIT | PCA9685_MODE1_EXTCLK_BIT);
    std::this_thread::sleep_for(std::chrono::milliseconds(2));
    res &= i2c.write_register_u8(m_descriptor->get_address(), PCA9685_RA_MODE1, PCA9685_MODE1_RESTART_BIT | PCA9685_MODE1_EXTCLK_BIT | PCA9685_MODE1_AI_BIT);
    std::this_thread::sleep_for(std::chrono::milliseconds(2));
    return res;
}


void PCA9685::set_pwm_value(bus::II2C& i2c, size_t idx, boost::optional<float> _value)
{
    QLOG_TOPIC("PCA9685::set_pwm_value");

    //todo - fix this
//    auto& ch = m_pwm_channels[idx];

//    float value = 0;
//    if (_value)
//    {
//        value = math::clamp(*_value, 0.f, 1.f);

//        if (ch.config->servo_signal)
//        {
//            float period_ms = 1000.f / m_descriptor->get_rate();
//            float servo_ms = math::lerp(ch.config->min_servo, ch.config->max_servo, value);
//            value = servo_ms / period_ms;
//        }
//        else
//        {
//            value = math::lerp(ch.config->min_pwm, ch.config->max_pwm, value);
//        }
//    }

//    int pulse = value * 4096;

//    uint8_t data[4] = {0, 0, 0, 0};
//    constexpr uint8_t ON_L = 0;
//    constexpr uint8_t ON_H = 1;
//    constexpr uint8_t OFF_L = 2;
//    constexpr uint8_t OFF_H = 3;

//    if (pulse == 0)
//    {
//        data[OFF_H] = 0x10;
//    }
//    else if (pulse >= 4096)
//    {
//        data[ON_H] = 0x10;
//    }
//    else
//    {
////        data[ON_L] = offset & 0xFF;
////        data[ON_H] = offset >> 8;
//        data[OFF_L] = pulse & 0xFF;
//        data[OFF_H] = pulse >> 8;
//    }

//    if (ch.last_data.pulse == pulse)
//    {
//        return;
//    }
//    ch.last_data.pulse = pulse;

//    i2c.write_register(m_descriptor->get_address(), PCA9685_RA_LED0_ON_L + 4 * idx, data, 4);
}

void PCA9685::process()
{
    QLOG_TOPIC("PCA9685::process");

    auto i2c = m_i2c.lock();
    if (!i2c)
    {
        return;
    }

    for (size_t i = 0; i < m_pwm_channels.size(); i++)
    {
        auto& ch = m_pwm_channels[i];
        auto stream = ch.stream.lock();
        if (stream)
        {
            auto const& samples = stream->get_samples();
            if (!samples.empty())
            {
                set_pwm_value(*i2c, i, samples.back().value);
            }
        }
    }
}

#define FIND_STREAM(CH)\
if (idx == CH)\
{\
    auto input_stream = m_hal.get_stream_registry().find_by_name<stream::IPWM>(path);\
    auto rate = input_stream ? input_stream->get_rate() : 0u;\
    if (rate != m_descriptor->get_rate())\
    {\
        m_pwm_channels[CH].stream.reset();\
        m_pwm_channels[CH].stream_path = std::string();\
        if (input_stream)\
        {\
            return make_error("Bad input stream '{}'. Expected rate {}Hz, got {}Hz", path, m_descriptor->get_rate(), rate);\
        }\
    }\
    else\
    {\
        QLOGI("Input stream '{}'", path, m_descriptor->get_rate(), rate);\
        m_pwm_channels[CH].stream = input_stream;\
        m_pwm_channels[CH].stream_path = path;\
    }\
    set_pwm_value(*i2c, idx, boost::none);\
}

ts::Result<void> PCA9685::set_input_stream_path(size_t idx, std::string const& path)
{
    QLOG_TOPIC("PCA9685::set_input_stream_path");

    auto i2c = m_i2c.lock();
    if (!i2c)
    {
        return make_error("Bus error");
    }

    FIND_STREAM(0);
    FIND_STREAM(1);
    FIND_STREAM(2);
    FIND_STREAM(3);
    FIND_STREAM(4);
    FIND_STREAM(5);
    FIND_STREAM(6);
    FIND_STREAM(7);
    FIND_STREAM(8);
    FIND_STREAM(9);
    FIND_STREAM(10);
    FIND_STREAM(11);
    FIND_STREAM(12);
    FIND_STREAM(13);
    FIND_STREAM(14);
    FIND_STREAM(15);
    return ts::success;
}


constexpr float MIN_SERVO_MS = 0.5f;
constexpr float MAX_SERVO_MS = 2.4f;

ts::Result<void> PCA9685::set_config(hal::INode_Config const& config)
{
    QLOG_TOPIC("PCA9685::set_config");

    auto specialized = dynamic_cast<hal::PCA9685_Config const*>(&config);
    if (!specialized)
    {
        return make_error("Wrong config type");
    }
    *m_config = *specialized;

    //todo - fix this
//    for (size_t i = 0; i < 16; i++)
//    {
//        if (m_pwm_channels[i].config->servo_signal)
//        {
//            m_pwm_channels[i].config->min_servo = math::clamp(m_pwm_channels[i].config->min_servo, MIN_SERVO_MS, MAX_SERVO_MS);
//            m_pwm_channels[i].config->max_servo = math::clamp(m_pwm_channels[i].config->max_servo, m_pwm_channels[i].config->min_servo, MAX_SERVO_MS);
//        }
//        else
//        {
//            m_pwm_channels[i].config->min_pwm = math::clamp(m_pwm_channels[i].config->min_pwm, 0.f, 1.f);
//            m_pwm_channels[i].config->max_pwm = math::clamp(m_pwm_channels[i].config->max_pwm, m_pwm_channels[i].config->min_pwm, 1.f);
//        }
//    }

    return ts::success;
}



auto PCA9685::get_config() const -> std::shared_ptr<const hal::INode_Config>
{
    //todo - fix this
//    rapidjson::Document json;
//    autojsoncxx::to_document(*m_config, json);

//    //rates lower than 50 and higher than 400Hz don't support servo signals
//    if (m_descriptor->rate > 400 || m_descriptor->rate < 50)
//    {
//        jsonutil::remove_value(json, std::string("Channel 1/Servo Signal"));
//        jsonutil::remove_value(json, std::string("Channel 2/Servo Signal"));
//        jsonutil::remove_value(json, std::string("Channel 3/Servo Signal"));
//        jsonutil::remove_value(json, std::string("Channel 4/Servo Signal"));
//        jsonutil::remove_value(json, std::string("Channel 5/Servo Signal"));
//        jsonutil::remove_value(json, std::string("Channel 6/Servo Signal"));
//        jsonutil::remove_value(json, std::string("Channel 7/Servo Signal"));
//        jsonutil::remove_value(json, std::string("Channel 8/Servo Signal"));
//        jsonutil::remove_value(json, std::string("Channel 9/Servo Signal"));
//        jsonutil::remove_value(json, std::string("Channel 10/Servo Signal"));
//        jsonutil::remove_value(json, std::string("Channel 11/Servo Signal"));
//        jsonutil::remove_value(json, std::string("Channel 12/Servo Signal"));
//        jsonutil::remove_value(json, std::string("Channel 13/Servo Signal"));
//        jsonutil::remove_value(json, std::string("Channel 14/Servo Signal"));
//        jsonutil::remove_value(json, std::string("Channel 15/Servo Signal"));
//        jsonutil::remove_value(json, std::string("Channel 16/Servo Signal"));
//    }

//    for (size_t i = 0; i < 16; i++)
//    {
//        if (m_pwm_channels[i].config->servo_signal)
//        {
//            jsonutil::remove_value(json, std::string(q::util::format<q::String>("Channel {}/Min PWM", i + 1)));
//            jsonutil::remove_value(json, std::string(q::util::format<q::String>("Channel {}/Max PWM", i + 1)));
//        }
//        else
//        {
//            jsonutil::remove_value(json, std::string(q::util::format<q::String>("Channel {}/Min Servo (ms)", i + 1)));
//            jsonutil::remove_value(json, std::string(q::util::format<q::String>("Channel {}/Max Servo (ms)", i + 1)));
//        }
//    }


//    return std::move(json);
    return m_config;
}

auto PCA9685::get_descriptor() const -> std::shared_ptr<const hal::INode_Descriptor>
{
    return m_descriptor;
}
ts::Result<std::shared_ptr<messages::INode_Message>> PCA9685::send_message(messages::INode_Message const& message)
{
    return make_error("Unknown message");
}


}
}
