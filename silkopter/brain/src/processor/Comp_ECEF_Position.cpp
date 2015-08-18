#include "BrainStdAfx.h"
#include "Comp_ECEF_Position.h"

#include "sz_math.hpp"
#include "sz_Comp_ECEF_Position.hpp"

namespace silk
{
namespace node
{

Comp_ECEF_Position::Comp_ECEF_Position(HAL& hal)
    : m_hal(hal)
    , m_init_params(new sz::Comp_ECEF_Position::Init_Params())
    , m_config(new sz::Comp_ECEF_Position::Config())
{
    m_position_output_stream = std::make_shared<ECEF_Position_Stream>();
//    m_enu_frame_output_stream = std::make_shared<ENU_Frame_Stream>();
}

auto Comp_ECEF_Position::init(rapidjson::Value const& init_params) -> bool
{
    QLOG_TOPIC("comp_position::init");

    sz::Comp_ECEF_Position::Init_Params sz;
    autojsoncxx::error::ErrorStack result;
    if (!autojsoncxx::from_value(sz, init_params, result))
    {
        std::ostringstream ss;
        ss << result;
        QLOGE("Cannot deserialize Comp_Position data: {}", ss.str());
        return false;
    }
    *m_init_params = sz;
    return init();
}
auto Comp_ECEF_Position::init() -> bool
{
    if (m_init_params->rate == 0)
    {
        QLOGE("Bad rate: {}Hz", m_init_params->rate);
        return false;
    }
    m_position_output_stream->rate = m_init_params->rate;
    m_dt = std::chrono::microseconds(1000000 / m_position_output_stream->rate);
    return true;
}

auto Comp_ECEF_Position::get_inputs() const -> std::vector<Input>
{
    std::vector<Input> inputs =
    {{
        { stream::IECEF_Position::TYPE, m_init_params->rate, "Position", m_accumulator.get_stream_path(0) },
        { stream::ILinear_Acceleration::TYPE, m_init_params->rate, "Linear Acceleration (ecef)", m_accumulator.get_stream_path(1) },
        { stream::IPressure::TYPE, m_init_params->rate, "Pressure", m_accumulator.get_stream_path(2) }
    }};
    return inputs;
}
auto Comp_ECEF_Position::get_outputs() const -> std::vector<Output>
{
    std::vector<Output> outputs =
    {{
        { "Position", m_position_output_stream },
       // { "ENU Frame", m_enu_frame_output_stream },
    }};
    return outputs;
}

void Comp_ECEF_Position::process()
{
    QLOG_TOPIC("comp_position::process");


    m_position_output_stream->samples.clear();

    m_accumulator.process([this](
                          size_t idx,
                          stream::IECEF_Position::Sample const& pos_sample,
                          stream::ILinear_Acceleration::Sample const& la_sample,
                          stream::IPressure::Sample const& p_sample)
    {
        auto& sample = m_position_output_stream->last_sample;
        sample.dt = m_dt;
        sample.value = pos_sample.value;

        m_position_output_stream->samples.push_back(sample);
    });
}

void Comp_ECEF_Position::set_input_stream_path(size_t idx, q::Path const& path)
{
    m_accumulator.set_stream_path(idx, path, m_init_params->rate, m_hal);
}

auto Comp_ECEF_Position::set_config(rapidjson::Value const& json) -> bool
{
    QLOG_TOPIC("comp_position::set_config");

    sz::Comp_ECEF_Position::Config sz;
    autojsoncxx::error::ErrorStack result;
    if (!autojsoncxx::from_value(sz, json, result))
    {
        std::ostringstream ss;
        ss << result;
        QLOGE("Cannot deserialize Comp_Position config data: {}", ss.str());
        return false;
    }

    *m_config = sz;

    return true;
}
auto Comp_ECEF_Position::get_config() const -> rapidjson::Document
{
    rapidjson::Document json;
    autojsoncxx::to_document(*m_config, json);
    return std::move(json);
}

auto Comp_ECEF_Position::get_init_params() const -> rapidjson::Document
{
    rapidjson::Document json;
    autojsoncxx::to_document(*m_init_params, json);
    return std::move(json);
}

auto Comp_ECEF_Position::send_message(rapidjson::Value const& /*json*/) -> rapidjson::Document
{
    return rapidjson::Document();
}

}
}
