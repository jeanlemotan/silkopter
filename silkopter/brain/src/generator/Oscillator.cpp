#include "BrainStdAfx.h"
#include "Oscillator.h"

#include "sz_math.hpp"
#include "sz_Oscillator.hpp"

namespace silk
{
namespace node
{

Oscillator::Oscillator(HAL& hal)
    : m_hal(hal)
    , m_init_params(new sz::Oscillator::Init_Params())
    , m_config(new sz::Oscillator::Config())
    , m_rnd_distribution(0, 0)
{
    autojsoncxx::to_document(*m_init_params, m_init_paramsj);
}

auto Oscillator::init(rapidjson::Value const& init_params) -> bool
{
    QLOG_TOPIC("Oscillator::init");

    sz::Oscillator::Init_Params sz;
    autojsoncxx::error::ErrorStack result;
    if (!autojsoncxx::from_value(sz, init_params, result))
    {
        std::ostringstream ss;
        ss << result;
        QLOGE("Cannot deserialize Oscillator data: {}", ss.str());
        return false;
    }
    jsonutil::clone_value(m_init_paramsj, init_params, m_init_paramsj.GetAllocator());
    *m_init_params = sz;
    return init();
}
auto Oscillator::init() -> bool
{
    m_output_stream = std::make_shared<Output_Stream>();
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
    m_output_stream->set_rate(m_init_params->rate);
    m_output_stream->set_tp(q::Clock::now());

    m_config->components.resize(m_init_params->component_count);

    return true;
}

auto Oscillator::get_stream_inputs() const -> std::vector<Stream_Input>
{
    return std::vector<Stream_Input>(0);
}

auto Oscillator::get_stream_outputs() const -> std::vector<Stream_Output>
{
    std::vector<Stream_Output> outputs =
    {{
         { stream::IFloat::TYPE, "Output", m_output_stream }
    }};
    return outputs;
}

void Oscillator::process()
{
    QLOG_TOPIC("Oscillator::process");

    m_output_stream->clear();

    float amplitude = m_config->amplitude;

    size_t samples_needed = m_output_stream->compute_samples_needed();
    while (samples_needed > 0)
    {
       float value = 0;

       m_period += q::Seconds(m_output_stream->get_dt()).count();
       //m_period = std::fmod(m_period, 1.f);

       float a = m_period * math::anglef::_2pi;
       for (auto& c: m_config->components)
       {
           value += math::sin(a * c.frequency) * 0.5f * c.amplitude;
       }
       if (m_config->square)
       {
           value = value < 0.f ? -0.5f : 0.5f;
       }
       value += m_rnd_distribution(m_rnd_engine);

       value *= amplitude;

       m_output_stream->push_sample(value, true);

       samples_needed--;
    }
}

void Oscillator::set_stream_input_path(size_t idx, q::Path const& path)
{
    QLOG_TOPIC("rate_controller::set_stream_input_path");
}

auto Oscillator::set_config(rapidjson::Value const& json) -> bool
{
    QLOG_TOPIC("Oscillator::set_config");

    sz::Oscillator::Config sz;
    autojsoncxx::error::ErrorStack result;
    if (!autojsoncxx::from_value(sz, json, result))
    {
        std::ostringstream ss;
        ss << result;
        QLOGE("Cannot deserialize Oscillator config data: {}", ss.str());
        return false;
    }

    *m_config = sz;

    m_rnd_distribution = std::uniform_real_distribution<float>(-m_config->noise*0.5f, m_config->noise*0.5f);

    return true;
}
auto Oscillator::get_config() const -> rapidjson::Document
{
    rapidjson::Document json;
    autojsoncxx::to_document(*m_config, json);
    return std::move(json);
}

auto Oscillator::get_init_params() const -> rapidjson::Document const&
{
    return m_init_paramsj;
}

auto Oscillator::send_message(rapidjson::Value const& /*json*/) -> rapidjson::Document
{
    return rapidjson::Document();
}


}
}
