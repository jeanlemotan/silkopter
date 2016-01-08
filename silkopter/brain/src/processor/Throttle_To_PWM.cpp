#include "BrainStdAfx.h"
#include "Throttle_To_PWM.h"

#include "sz_math.hpp"
#include "sz_Throttle_To_PWM.hpp"

namespace silk
{
namespace node
{

Throttle_To_PWM::Throttle_To_PWM(HAL& hal)
    : m_hal(hal)
    , m_init_params(new sz::Throttle_To_PWM::Init_Params())
    , m_config(new sz::Throttle_To_PWM::Config())
{
}

auto Throttle_To_PWM::init(rapidjson::Value const& init_params) -> bool
{
    QLOG_TOPIC("Throttle_To_PWM::init");

    sz::Throttle_To_PWM::Init_Params sz;
    autojsoncxx::error::ErrorStack result;
    if (!autojsoncxx::from_value(sz, init_params, result))
    {
        std::ostringstream ss;
        ss << result;
        QLOGE("Cannot deserialize Throttle_To_PWM data: {}", ss.str());
        return false;
    }
    *m_init_params = sz;
    return init();
}
auto Throttle_To_PWM::init() -> bool
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
    m_output_streams.resize(m_init_params->channels);

    for (auto& o: m_output_streams)
    {
        o.reset(new Output_Stream);
        o->set_rate(m_init_params->rate);
    }
    return true;
}

auto Throttle_To_PWM::start(q::Clock::time_point tp) -> bool
{
    for (auto& o: m_output_streams)
    {
        o->set_tp(tp);
    }
    return true;
}

auto Throttle_To_PWM::get_inputs() const -> std::vector<Input>
{
    std::vector<Input> inputs(m_init_params->channels);
    for (size_t i = 0; i < m_init_params->channels; i++)
    {
        inputs[i] = { stream::IThrottle::TYPE, m_init_params->rate, q::util::format2<std::string>("Throttle {}", i), m_accumulators[i].get_stream_path(0) };
    }
    return inputs;
}
auto Throttle_To_PWM::get_outputs() const -> std::vector<Output>
{
    std::vector<Output> outputs(m_init_params->channels);
    for (size_t i = 0; i < m_init_params->channels; i++)
    {
        outputs[i] = { q::util::format2<std::string>("PWM {}", i), m_output_streams[i] };
    }
    return outputs;
}

void Throttle_To_PWM::process()
{
    QLOG_TOPIC("Throttle_To_PWM::process");

    for (size_t i = 0; i < m_init_params->channels; i++)
    {
        m_output_streams[i]->clear();

        m_accumulators[i].process([this, i](stream::IThrottle::Sample const& i_sample)
        {
            if (i_sample.is_healthy)
            {
                m_output_streams[i]->push_sample(i_sample.value, true);
            }
            else
            {
                m_output_streams[i]->push_last_sample(false);
            }
        });
    }
}

void Throttle_To_PWM::set_input_stream_path(size_t idx, q::Path const& path)
{
    m_accumulators[idx].set_stream_path(0, path, m_output_streams[0]->get_rate(), m_hal);
}

auto Throttle_To_PWM::set_config(rapidjson::Value const& json) -> bool
{
    QLOG_TOPIC("Throttle_To_PWM::set_config");

    sz::Throttle_To_PWM::Config sz;
    autojsoncxx::error::ErrorStack result;
    if (!autojsoncxx::from_value(sz, json, result))
    {
        std::ostringstream ss;
        ss << result;
        QLOGE("Cannot deserialize Throttle_To_PWM config data: {}", ss.str());
        return false;
    }

    *m_config = sz;

    return true;
}
auto Throttle_To_PWM::get_config() const -> rapidjson::Document
{
    rapidjson::Document json;
    autojsoncxx::to_document(*m_config, json);
    return std::move(json);
}

auto Throttle_To_PWM::get_init_params() const -> rapidjson::Document
{
    rapidjson::Document json;
    autojsoncxx::to_document(*m_init_params, json);
    return std::move(json);
}
auto Throttle_To_PWM::send_message(rapidjson::Value const& /*json*/) -> rapidjson::Document
{
    return rapidjson::Document();
}


}
}
