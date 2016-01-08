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
    m_output_stream = std::make_shared<Output_Stream>();
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
    *m_init_params = sz;
    return init();
}
auto Oscillator::init() -> bool
{
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

    m_config->components.resize(m_init_params->component_count);

    return true;
}

auto Oscillator::start(q::Clock::time_point tp) -> bool
{
    m_output_stream->set_tp(tp);
    return true;
}

auto Oscillator::get_inputs() const -> std::vector<Input>
{
    return std::vector<Input>(0);
}

auto Oscillator::get_outputs() const -> std::vector<Output>
{
    std::vector<Output> outputs =
    {{
         { "Output", m_output_stream }
    }};
    return outputs;
}

void Oscillator::process()
{
    QLOG_TOPIC("Oscillator::process");

    m_output_stream->clear();

    float amplitude = m_config->amplitude;

    constexpr size_t MAX_SAMPLES_NEEDED = 10000;

    size_t samples_needed = m_output_stream->compute_samples_needed();
    if (samples_needed > MAX_SAMPLES_NEEDED)
    {
        QLOGW("Too many samples needed: {}. Clamping to an arbitrary MAX: {}", samples_needed, MAX_SAMPLES_NEEDED);
        samples_needed = MAX_SAMPLES_NEEDED;
    }

    float period = std::chrono::duration<float>(m_output_stream->get_dt()).count();

    while (samples_needed > 0)
    {
       float value = 0;

       m_period += period;

       float a = m_period * math::anglef::_2pi;
       for (auto& c: m_config->components)
       {
           value += math::sin(a * c.frequency) * 0.5f * c.amplitude;
       }
       if (m_config->square)
       {
           value = value < 0.f ? -0.5f : 0.5f;
       }
       if (m_has_noise)
       {
           value += m_rnd_distribution(m_rnd_engine);
       }

       value *= amplitude;

       m_output_stream->push_sample(value, true);

       samples_needed--;
    }
}

void Oscillator::set_input_stream_path(size_t idx, q::Path const& path)
{
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
    m_has_noise = !math::is_zero(m_config->noise, math::epsilon<float>());

    return true;
}
auto Oscillator::get_config() const -> rapidjson::Document
{
    rapidjson::Document json;
    autojsoncxx::to_document(*m_config, json);
    return std::move(json);
}

auto Oscillator::get_init_params() const -> rapidjson::Document
{
    rapidjson::Document json;
    autojsoncxx::to_document(*m_init_params, json);
    return std::move(json);
}

auto Oscillator::send_message(rapidjson::Value const& /*json*/) -> rapidjson::Document
{
    return rapidjson::Document();
}


}
}
