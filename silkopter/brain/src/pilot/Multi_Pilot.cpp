#include "BrainStdAfx.h"
#include "Multi_Pilot.h"

#include "sz_math.hpp"
#include "sz_Multi_Pilot.hpp"

namespace silk
{
namespace node
{

Multi_Pilot::Multi_Pilot(HAL& hal, Comms& comms)
    : m_hal(hal)
    , m_comms(comms)
    , m_init_params(new sz::Multi_Pilot::Init_Params())
    , m_config(new sz::Multi_Pilot::Config())
{
    m_output_stream = std::make_shared<Output_Stream>();
}

auto Multi_Pilot::init(rapidjson::Value const& init_params) -> bool
{
    QLOG_TOPIC("Multi_Pilot::init");

    sz::Multi_Pilot::Init_Params sz;
    autojsoncxx::error::ErrorStack result;
    if (!autojsoncxx::from_value(sz, init_params, result))
    {
        std::ostringstream ss;
        ss << result;
        QLOGE("Cannot deserialize Multi_Pilot data: {}", ss.str());
        return false;
    }
    *m_init_params = sz;
    return init();
}

auto Multi_Pilot::init() -> bool
{
    if (m_init_params->rate == 0)
    {
        QLOGE("Bad rate: {}Hz", m_init_params->rate);
        return false;
    }
    m_output_stream->set_rate(m_init_params->rate);
    m_output_stream->set_tp(q::Clock::now());

    return true;
}

auto Multi_Pilot::get_inputs() const -> std::vector<Input>
{
    std::vector<Input> inputs =
    {{
        { stream::IMulti_State::TYPE,  m_init_params->rate, "State", m_accumulator.get_stream_path(0) },
    }};
    return inputs;
}
auto Multi_Pilot::get_outputs() const -> std::vector<Output>
{
    std::vector<Output> outputs =
    {{
         { "Input",        m_output_stream },
    }};
    return outputs;
}

static constexpr std::chrono::milliseconds INPUT_HEALTHY_TIMEOUT(200);

void Multi_Pilot::process()
{
    QLOG_TOPIC("Multi_Pilot::process");

    m_output_stream->clear();

    //process inputs
    {
        auto const& input_values = m_comms.get_multi_input_values();

        auto now = q::Clock::now();

        stream::IMulti_Input::Value& input_value = m_last_input_value;
        if (!input_values.empty())
        {
            input_value = input_values.back();
            m_last_received_input_value_tp = now;
        }

        bool is_healthy = now - m_last_received_input_value_tp < INPUT_HEALTHY_TIMEOUT;

        size_t samples_needed = m_output_stream->compute_samples_needed();
        for (size_t i = 0; i < samples_needed; i++)
        {
            m_output_stream->push_sample(input_value, is_healthy);
        }
    }


    //write back the state
    m_accumulator.process([this](stream::IMulti_State::Sample const& i_state)
    {
        m_comms.add_multi_state_sample(i_state);
    });
}

void Multi_Pilot::set_input_stream_path(size_t idx, q::Path const& path)
{
    m_accumulator.set_stream_path(idx, path, m_init_params->rate, m_hal);
}

auto Multi_Pilot::set_config(rapidjson::Value const& json) -> bool
{
    QLOG_TOPIC("Multi_Pilot::set_config");

    sz::Multi_Pilot::Config sz;
    autojsoncxx::error::ErrorStack result;
    if (!autojsoncxx::from_value(sz, json, result))
    {
        std::ostringstream ss;
        ss << result;
        QLOGE("Cannot deserialize Multi_Pilot config data: {}", ss.str());
        return false;
    }

    *m_config = sz;

    return true;
}
auto Multi_Pilot::get_config() const -> rapidjson::Document
{
    rapidjson::Document json;
    autojsoncxx::to_document(*m_config, json);
    return std::move(json);
}

auto Multi_Pilot::get_init_params() const -> rapidjson::Document
{
    rapidjson::Document json;
    autojsoncxx::to_document(*m_init_params, json);
    return std::move(json);
}

auto Multi_Pilot::send_message(rapidjson::Value const& /*json*/) -> rapidjson::Document
{
    return rapidjson::Document();
}

}
}
