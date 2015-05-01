#include "BrainStdAfx.h"
#include "ADS1115.h"
#include "physics/constants.h"
#include "utils/Timed_Scope.h"

#include "sz_math.hpp"
#include "sz_ADS1115.hpp"

namespace silk
{
namespace node
{


ADS1115::ADS1115(HAL& hal)
    : m_hal(hal)
    , m_init_params(new sz::ADS1115::Init_Params())
    , m_config(new sz::ADS1115::Config())
{
    autojsoncxx::to_document(*m_init_params, m_init_paramsj);
}

auto ADS1115::get_outputs() const -> std::vector<Output>
{
    std::vector<Output> outputs =
    {{
        { stream::IADC::TYPE, "ADC0", m_adc[0] },
        { stream::IADC::TYPE, "ADC1", m_adc[1] },
        { stream::IADC::TYPE, "ADC2", m_adc[2] },
        { stream::IADC::TYPE, "ADC3", m_adc[3] }
     }};
    return outputs;
}
auto ADS1115::init(rapidjson::Value const& init_params) -> bool
{
    QLOG_TOPIC("ADS1115::init");

    sz::ADS1115::Init_Params sz;
    autojsoncxx::error::ErrorStack result;
    if (!autojsoncxx::from_value(sz, init_params, result))
    {
        std::ostringstream ss;
        ss << result;
        QLOGE("Cannot deserialize ADS1115 data: {}", ss.str());
        return false;
    }
    jsonutil::clone_value(m_init_paramsj, init_params, m_init_paramsj.GetAllocator());
    *m_init_params = sz;
    return init();
}

auto ADS1115::init() -> bool
{
    m_i2c = m_hal.get_buses().find_by_name<bus::II2C>(m_init_params->bus);

    auto i2c = m_i2c.lock();
    if (!i2c)
    {
        QLOGE("No bus configured");
        return false;
    }

    m_adc[0] = std::make_shared<Stream>();
    m_adc[1] = std::make_shared<Stream>();
    m_adc[2] = std::make_shared<Stream>();
    m_adc[3] = std::make_shared<Stream>();

    m_init_params->rate = math::clamp<size_t>(m_init_params->rate, 1, 800);

    m_adc[0]->rate = m_init_params->rate;
    m_adc[1]->rate = m_init_params->rate;
    m_adc[2]->rate = m_init_params->rate;
    m_adc[3]->rate = m_init_params->rate;

    m_dt = std::chrono::milliseconds(1000 / m_init_params->rate);

    return true;
}


void ADS1115::process()
{
    QLOG_TOPIC("ADS1115::process");

    m_adc[0]->samples.clear();
    m_adc[1]->samples.clear();
    m_adc[2]->samples.clear();
    m_adc[3]->samples.clear();

    auto i2c = m_i2c.lock();
    if (!i2c)
    {
        return;
    }

    auto now = q::Clock::now();
    if (now - m_last_tp < m_dt)
    {
        return;
    }

    m_last_tp = now;

}

auto ADS1115::set_config(rapidjson::Value const& json) -> bool
{
    QLOG_TOPIC("ADS1115::set_config");

    sz::ADS1115::Config sz;
    autojsoncxx::error::ErrorStack result;
    if (!autojsoncxx::from_value(sz, json, result))
    {
        std::ostringstream ss;
        ss << result;
        QLOGE("Cannot deserialize ADS1115 config data: {}", ss.str());
        return false;
    }

    *m_config = sz;
    return true;
}
auto ADS1115::get_config() const -> rapidjson::Document
{
    rapidjson::Document json;
    autojsoncxx::to_document(*m_config, json);
    return std::move(json);
}

auto ADS1115::get_init_params() const -> rapidjson::Document const&
{
    return m_init_paramsj;
}

auto ADS1115::send_message(rapidjson::Value const& /*json*/) -> rapidjson::Document
{
    return rapidjson::Document();
}

}
}
