#include "BrainStdAfx.h"
#include "PCA9685.h"

#include "sz_math.hpp"
#include "sz_PCA9685.hpp"


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


PCA9685::PCA9685(HAL& hal)
    : m_hal(hal)
    , m_init_params(new sz::PCA9685::Init_Params())
    , m_config(new sz::PCA9685::Config())
{
    autojsoncxx::to_document(*m_init_params, m_init_paramsj);
}

auto PCA9685::get_stream_inputs() const -> std::vector<Stream_Input>
{
    std::vector<Stream_Input> inputs =
    {{
        { stream::IPWM::TYPE, m_init_params->rate, "Channel 1" },
        { stream::IPWM::TYPE, m_init_params->rate, "Channel 2" },
        { stream::IPWM::TYPE, m_init_params->rate, "Channel 3" },
        { stream::IPWM::TYPE, m_init_params->rate, "Channel 4" },
        { stream::IPWM::TYPE, m_init_params->rate, "Channel 5" },
        { stream::IPWM::TYPE, m_init_params->rate, "Channel 6" },
        { stream::IPWM::TYPE, m_init_params->rate, "Channel 7" },
        { stream::IPWM::TYPE, m_init_params->rate, "Channel 8" },
        { stream::IPWM::TYPE, m_init_params->rate, "Channel 9" },
        { stream::IPWM::TYPE, m_init_params->rate, "Channel 10" },
        { stream::IPWM::TYPE, m_init_params->rate, "Channel 11" },
        { stream::IPWM::TYPE, m_init_params->rate, "Channel 12" },
        { stream::IPWM::TYPE, m_init_params->rate, "Channel 13" },
        { stream::IPWM::TYPE, m_init_params->rate, "Channel 14" },
        { stream::IPWM::TYPE, m_init_params->rate, "Channel 15" },
        { stream::IPWM::TYPE, m_init_params->rate, "Channel 16" },
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
    m_i2c = m_hal.get_buses().find_by_name<bus::II2C>(m_init_params->bus);
    auto i2c = m_i2c.lock();
    if (!i2c)
    {
        QLOGE("No bus configured");
        return false;
    }

    if (m_init_params->address > 127)
    {
        QLOGE("Invalid address {}", m_init_params->address);
        return false;
    }

    //                            4     5     6     8    10   12   15   20   24   30   40   60   75  80, 100 120 150 200 240
    std::vector<size_t> rates = { 1500, 1200, 1000, 750, 600, 500, 400, 300, 250, 200, 150, 100, 80, 75, 60, 50, 40, 30, 25 };
    if (std::find(rates.begin(), rates.end(), m_init_params->rate) == rates.end())
    {
        QLOGE("Invalid rate {}. Supported are: {}", m_init_params->rate, rates);
        return false;
    }

    bool res = i2c->write_register_u8(m_init_params->address, PCA9685_RA_MODE1, PCA9685_MODE1_AI_BIT);
    res &= i2c->write_register_u8(m_init_params->address, PCA9685_RA_MODE1, PCA9685_MODE1_SLEEP_BIT);
    res &= i2c->write_register_u8(m_init_params->address, PCA9685_RA_MODE1, PCA9685_MODE1_SLEEP_BIT | PCA9685_MODE1_EXTCLK_BIT);
    res &= i2c->write_register_u8(m_init_params->address, PCA9685_RA_MODE1, PCA9685_MODE1_RESTART_BIT | PCA9685_MODE1_EXTCLK_BIT | PCA9685_MODE1_AI_BIT);

    uint8_t data;
    res &= i2c->write_register_u8(m_init_params->address, PCA9685_RA_PRE_SCALE, data);
    if (!res && data <= 3)
    {
        QLOGE("PCA9685 not found on {}, address {}", m_init_params->bus, m_init_params->address);
        return false;
    }

    uint8_t prescale = math::round(24576000.f / 4096.f / m_init_params->rate) - 1;
    res &= i2c->write_register_u8(m_init_params->address, PCA9685_RA_PRE_SCALE, prescale);
    if (!res)
    {
        QLOGE("I2C error while setting rate {}, prescale {}", m_init_params->rate, prescale);
        return false;
    }

    m_pwm_channels.resize(16);

    return true;
}

void PCA9685::set_pwm_value(size_t idx, float value)
{
    QLOG_TOPIC("PCA9685::set_pwm_value");

    //auto const& ch = *m_pwm_channels[idx].config;
    value = math::clamp(value, 0.f, 1.f);
    int pulse = value * 4096;//(ch.max - ch.min);


    uint8_t data[4] = {0, 0, 0, 0};
    if (pulse == 0)
    {
        data[3] = 0x10;
    }
    else if (pulse >= 4096)
    {
        data[1] = 0x10;
    }
    else
    {
        //data[0] = offset & 0xFF;
        //data[1] = offset >> 8;
        data[2] = pulse & 0xFF;
        data[3] = pulse >> 8;
    }

    auto i2c = m_i2c.lock();
    if (!i2c)
    {
        QLOGE("Bus not found");
        return;
    }

    i2c->write_register(m_init_params->address, PCA9685_RA_LED0_ON_L + 4 * idx, data, 4);
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


#define READ_CONFIG(CH)\
{\
    auto input_stream = m_hal.get_streams().find_by_name<stream::IPWM>(sz.input_streams.channel_##CH);\
    m_pwm_channels[CH - 1].stream = input_stream;\
    m_pwm_channels[CH - 1].config = &m_config->ranges.channel_##CH;\
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

    READ_CONFIG(1);
    READ_CONFIG(2);
    READ_CONFIG(3);
    READ_CONFIG(4);
    READ_CONFIG(5);
    READ_CONFIG(6);
    READ_CONFIG(7);
    READ_CONFIG(8);
    READ_CONFIG(9);
    READ_CONFIG(10);
    READ_CONFIG(11);
    READ_CONFIG(12);
    READ_CONFIG(13);
    READ_CONFIG(14);
    READ_CONFIG(15);
    READ_CONFIG(16);

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
