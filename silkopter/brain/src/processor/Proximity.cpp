#include "BrainStdAfx.h"
#include "Proximity.h"

#include "sz_math.hpp"
#include "sz_Proximity.hpp"

namespace silk
{
namespace node
{

Proximity::Proximity(HAL& hal)
    : m_hal(hal)
    , m_init_params(new sz::Proximity::Init_Params())
    , m_config(new sz::Proximity::Config())
{
    m_output_stream = std::make_shared<Output_Stream>();
}

auto Proximity::init(rapidjson::Value const& init_params) -> bool
{
    QLOG_TOPIC("Proximity::init");

    sz::Proximity::Init_Params sz;
    autojsoncxx::error::ErrorStack result;
    if (!autojsoncxx::from_value(sz, init_params, result))
    {
        std::ostringstream ss;
        ss << result;
        QLOGE("Cannot deserialize Proximity data: {}", ss.str());
        return false;
    }
    *m_init_params = sz;
    return init();
}
auto Proximity::init() -> bool
{
    if (m_init_params->rate == 0)
    {
        QLOGE("Bad rate: {}Hz", m_init_params->rate);
        return false;
    }
    if (m_init_params->channels == 0)
    {
        QLOGE("Need to have at least one channel");
        return false;
    }

    m_accumulators.resize(m_init_params->channels);
    m_output_stream->set_rate(m_init_params->rate);

    return true;
}

auto Proximity::start(q::Clock::time_point tp) -> bool
{
    m_output_stream->set_tp(tp);
    return true;
}

auto Proximity::get_inputs() const -> std::vector<Input>
{
    std::vector<Input> inputs(m_init_params->channels);
    for (size_t i = 0; i < m_init_params->channels; i++)
    {
        inputs[i] = { stream::IDistance::TYPE, m_init_params->rate, q::util::format2<std::string>("Distance {}", i), m_accumulators[i].get_stream_path(0) };
    }
    return inputs;
}
auto Proximity::get_outputs() const -> std::vector<Output>
{
    std::vector<Output> outputs =
    {{
         { "Proximity", m_output_stream }
    }};
    return outputs;
}

void Proximity::process()
{
    QLOG_TOPIC("Proximity::process");

    m_output_stream->clear();
    m_output_value.distances.clear();

    for (size_t i = 0; i < m_init_params->channels; i++)
    {
        m_accumulators[i].process([this, i](stream::IDistance::Sample const& i_sample)
        {
            if (i_sample.is_healthy)
            {
                m_output_value.distances.push_back(i_sample.value);
            }
        });
    }

    size_t samples_needed = m_output_stream->compute_samples_needed();
    if (samples_needed > 0)
    {
        for (size_t i = 0; i < samples_needed; i++)
        {
            m_output_stream->push_sample(m_output_value, !m_output_value.distances.empty());
        }
    }
}

void Proximity::set_input_stream_path(size_t idx, q::Path const& path)
{
    m_accumulators[idx].set_stream_path(0, path, m_output_stream->get_rate(), m_hal);
}

auto Proximity::set_config(rapidjson::Value const& json) -> bool
{
    QLOG_TOPIC("Proximity::set_config");

    sz::Proximity::Config sz;
    autojsoncxx::error::ErrorStack result;
    if (!autojsoncxx::from_value(sz, json, result))
    {
        std::ostringstream ss;
        ss << result;
        QLOGE("Cannot deserialize Proximity config data: {}", ss.str());
        return false;
    }

    *m_config = sz;

    return true;
}
auto Proximity::get_config() const -> rapidjson::Document
{
    rapidjson::Document json;
    autojsoncxx::to_document(*m_config, json);
    return std::move(json);
}

auto Proximity::get_init_params() const -> rapidjson::Document
{
    rapidjson::Document json;
    autojsoncxx::to_document(*m_init_params, json);
    return std::move(json);
}
auto Proximity::send_message(rapidjson::Value const& /*json*/) -> rapidjson::Document
{
    return rapidjson::Document();
}


}
}
