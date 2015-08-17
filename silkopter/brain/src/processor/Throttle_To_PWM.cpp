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
    m_output_stream = std::make_shared<Output_Stream>();
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
    m_output_stream->set_rate(m_init_params->rate);
    m_output_stream->set_tp(q::Clock::now());
    return true;
}

auto Throttle_To_PWM::get_inputs() const -> std::vector<Input>
{
    std::vector<Input> inputs =
    {{
        { stream::IThrottle::TYPE, m_init_params->rate, "Throttle", m_accumulator.get_stream_path(0) }
    }};
    return inputs;
}
auto Throttle_To_PWM::get_outputs() const -> std::vector<Output>
{
    std::vector<Output> outputs(1);
    outputs[0].name = "PWM";
    outputs[0].stream = m_output_stream;
    return outputs;
}

void Throttle_To_PWM::process()
{
    QLOG_TOPIC("Throttle_To_PWM::process");

    m_output_stream->clear();

    m_accumulator.process([this](
                          size_t idx,
                          stream::IThrottle::Sample const& i_sample)
    {
        m_output_stream->push_sample(i_sample.value, i_sample.is_healthy);
    });
}

void Throttle_To_PWM::set_input_stream_path(size_t idx, q::Path const& path)
{
    m_accumulator.set_stream_path(idx, path, m_output_stream->get_rate(), m_hal);
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
