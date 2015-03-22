#include "BrainStdAfx.h"
#include "Factor_Generator.h"

#include "sz_math.hpp"
#include "sz_Factor_Generator.hpp"

namespace silk
{
namespace node
{

Factor_Generator::Factor_Generator(HAL& hal)
    : m_hal(hal)
    , m_init_params(new sz::Factor_Generator::Init_Params())
    , m_config(new sz::Factor_Generator::Config())
{
    autojsoncxx::to_document(*m_init_params, m_init_paramsj);
}

auto Factor_Generator::init(rapidjson::Value const& init_params) -> bool
{
    QLOG_TOPIC("factor_generator::init");

    sz::Factor_Generator::Init_Params sz;
    autojsoncxx::error::ErrorStack result;
    if (!autojsoncxx::from_value(sz, init_params, result))
    {
        std::ostringstream ss;
        ss << result;
        QLOGE("Cannot deserialize Factor_Generator data: {}", ss.str());
        return false;
    }
    jsonutil::clone_value(m_init_paramsj, init_params, m_init_paramsj.GetAllocator());
    *m_init_params = sz;
    return init();
}
auto Factor_Generator::init() -> bool
{
    m_output_stream = std::make_shared<Stream>();
    if (m_init_params->rate == 0)
    {
        QLOGE("Bad rate: {}Hz", m_init_params->rate);
        return false;
    }
    if (m_init_params->component_count == 0)
    {
        QLOGE("Needs at least one frequency component");
        return false;
    }
    m_output_stream->rate = m_init_params->rate;

    m_config->components.resize(m_init_params->component_count);

    m_last_tp = q::Clock::now();
    m_dt = std::chrono::microseconds(1000000 / m_init_params->rate);
    return true;
}

auto Factor_Generator::get_outputs() const -> std::vector<Output>
{
    std::vector<Output> outputs(1);
    outputs[0].type = stream::IFactor::TYPE;
    outputs[0].name = "Factor";
    outputs[0].stream = m_output_stream;
    return outputs;
}

void Factor_Generator::process()
{
    QLOG_TOPIC("factor_generator::process");

    m_output_stream->samples.clear();

    auto now = q::Clock::now();
    auto dt = now - m_last_tp;
    if (dt < m_dt)
    {
        return;
    }
    auto tp = m_last_tp + m_dt;
    m_last_tp = now;

    m_output_stream->samples.reserve(dt / m_dt);

    while (dt >= m_dt)
    {
       Stream::Sample vs;
       vs.dt = m_dt;
       vs.tp = tp;
       vs.sample_idx = ++m_output_stream->sample_idx;
       vs.value = 0;

       m_period += q::Seconds(m_dt).count();
//           if (m_period > math::anglef::_2pi)
//           {
//               m_period -= math::anglef::_2pi;
//           }
       float a = m_period * math::anglef::_2pi;
       for (auto& c: m_config->components)
       {
           vs.value += math::sin(a * c.frequency) * 0.5f * c.amplitude;
       }
       if (m_config->square)
       {
           vs.value = vs.value < 0.f ? -0.5f : 0.5f;
       }
       vs.value += m_rnd_distribution(m_rnd_engine);

       vs.value *= m_config->amplitude;

       m_output_stream->samples.push_back(vs);

       tp += m_dt;
       dt -= m_dt;
    }

    //reminder for next process
    m_last_tp -= dt;
}

auto Factor_Generator::set_config(rapidjson::Value const& json) -> bool
{
    QLOG_TOPIC("factor_generator::set_config");

    sz::Factor_Generator::Config sz;
    autojsoncxx::error::ErrorStack result;
    if (!autojsoncxx::from_value(sz, json, result))
    {
        std::ostringstream ss;
        ss << result;
        QLOGE("Cannot deserialize Factor_Generator config data: {}", ss.str());
        return false;
    }

    *m_config = sz;

    m_rnd_distribution = std::uniform_real_distribution<float>(-m_config->noise*0.5f, m_config->noise*0.5f);

    return true;
}
auto Factor_Generator::get_config() const -> rapidjson::Document
{
    rapidjson::Document json;
    autojsoncxx::to_document(*m_config, json);
    return std::move(json);
}

auto Factor_Generator::get_init_params() const -> rapidjson::Document const&
{
    return m_init_paramsj;
}


}
}
