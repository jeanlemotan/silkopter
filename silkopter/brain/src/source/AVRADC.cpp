#include "BrainStdAfx.h"
#include "AVRADC.h"
#include "physics/constants.h"
#include "utils/Timed_Scope.h"

#include "sz_math.hpp"
#include "sz_AVRADC.hpp"

namespace silk
{
namespace node
{


constexpr uint8_t AVRADC_ADDRESS            = 0x17;
constexpr uint8_t AVRADC_REG_ADC0           = 0x0;
constexpr uint8_t AVRADC_REG_ADC1           = 0x2;

constexpr std::chrono::milliseconds MIN_CONVERSION_DURATION(5);

AVRADC::AVRADC(HAL& hal)
    : m_hal(hal)
    , m_init_params(new sz::AVRADC::Init_Params())
    , m_config(new sz::AVRADC::Config())
{
    for (auto& adc: m_adcs)
    {
        adc = std::make_shared<Output_Stream>();
    }
}

auto AVRADC::get_outputs() const -> std::vector<Output>
{
    std::vector<Output> outputs =
    {{
        { "ADC0", m_adcs[0] },
        { "ADC1", m_adcs[1] }
     }};
    return outputs;
}
auto AVRADC::init(rapidjson::Value const& init_params) -> bool
{
    QLOG_TOPIC("AVRADC::init");

    sz::AVRADC::Init_Params sz;
    autojsoncxx::error::ErrorStack result;
    if (!autojsoncxx::from_value(sz, init_params, result))
    {
        std::ostringstream ss;
        ss << result;
        QLOGE("Cannot deserialize AVRADC data: {}", ss.str());
        return false;
    }
    *m_init_params = sz;
    return init();
}

auto AVRADC::init() -> bool
{
    m_i2c = m_hal.get_buses().find_by_name<bus::II2C>(m_init_params->bus);

    auto i2c = m_i2c.lock();
    if (!i2c)
    {
        QLOGE("No bus configured");
        return false;
    }

    i2c->lock();
    At_Exit at_exit([this, &i2c]()
    {
        i2c->unlock();
    });


    m_init_params->rate = math::clamp<size_t>(m_init_params->rate, 1, 500);

    m_adcs[0]->set_rate(m_init_params->rate);
    m_adcs[1]->set_rate(m_init_params->rate);

    m_crt_adc = 0;

    return true;
}

auto AVRADC::start(q::Clock::time_point tp) -> bool
{
    m_last_tp = tp;
    for (auto& adc: m_adcs)
    {
        adc->set_tp(tp);
    }
    return true;
}


void AVRADC::process()
{
    QLOG_TOPIC("AVRADC::process");

    for (auto& adc: m_adcs)
    {
        adc->clear();
    }

    auto i2c = m_i2c.lock();
    if (!i2c)
    {
        return;
    }

    i2c->lock();
    At_Exit at_exit([this, &i2c]()
    {
        i2c->unlock();
    });

    auto now = q::Clock::now();
    if (now - m_last_tp < MIN_CONVERSION_DURATION)
    {
        return;
    }

    //TODO - add healthy indication
    uint8_t data[4] = {0};
    if (!i2c->read(AVRADC_ADDRESS, data, sizeof(data)))
    {
        return;
    }

    for (size_t i = 0; i < m_adcs.size(); i++)
    {
        uint16_t idata = static_cast<uint16_t>((data[1 + i*2] << 8) | data[0 + i*2]);

        //scale to 0..1
        float value = math::clamp(idata / 32768.f, 0.f, 1.f) * 5.f;

        //add samples up to the sample rate
        auto samples_needed = m_adcs[i]->compute_samples_needed();
        while (samples_needed > 0)
        {
            m_adcs[i]->push_sample(value, true);
            samples_needed--;
        }
    }

    m_last_tp = now;
}

auto AVRADC::set_config(rapidjson::Value const& json) -> bool
{
    QLOG_TOPIC("AVRADC::set_config");

    sz::AVRADC::Config sz;
    autojsoncxx::error::ErrorStack result;
    if (!autojsoncxx::from_value(sz, json, result))
    {
        std::ostringstream ss;
        ss << result;
        QLOGE("Cannot deserialize AVRADC config data: {}", ss.str());
        return false;
    }

    *m_config = sz;
    return true;
}
auto AVRADC::get_config() const -> rapidjson::Document
{
    rapidjson::Document json;
    autojsoncxx::to_document(*m_config, json);
    return std::move(json);
}

auto AVRADC::get_init_params() const -> rapidjson::Document
{
    rapidjson::Document json;
    autojsoncxx::to_document(*m_init_params, json);
    return std::move(json);
}

auto AVRADC::send_message(rapidjson::Value const& /*json*/) -> rapidjson::Document
{
    return rapidjson::Document();
}

}
}
