#include "BrainStdAfx.h"
#include "Multi_Pilot.h"

#include "sz_math.hpp"
#include "sz_Multi_Pilot.hpp"

namespace silk
{
namespace node
{

Multi_Pilot::Multi_Pilot(HAL& hal)
    : m_hal(hal)
    , m_init_params(new sz::Multi_Pilot::Init_Params())
    , m_config(new sz::Multi_Pilot::Config())
{
    autojsoncxx::to_document(*m_init_params, m_init_paramsj);
}

auto Multi_Pilot::init(rapidjson::Value const& init_params) -> bool
{
    QLOG_TOPIC("multi_pilot::init");

    sz::Multi_Pilot::Init_Params sz;
    autojsoncxx::error::ErrorStack result;
    if (!autojsoncxx::from_value(sz, init_params, result))
    {
        std::ostringstream ss;
        ss << result;
        QLOGE("Cannot deserialize Multi_Pilot data: {}", ss.str());
        return false;
    }
    jsonutil::clone_value(m_init_paramsj, init_params, m_init_paramsj.GetAllocator());
    *m_init_params = sz;
    return init();
}

auto Multi_Pilot::init() -> bool
{
    m_output_stream = std::make_shared<Stream>();
    if (m_init_params->rate == 0)
    {
        QLOGE("Bad rate: {}Hz", m_init_params->rate);
        return false;
    }
    m_output_stream->rate = m_init_params->rate;
    m_dt = std::chrono::microseconds(1000000 / m_output_stream->rate);
    return true;
}

auto Multi_Pilot::get_stream_inputs() const -> std::vector<Stream_Input>
{
    std::vector<Stream_Input> inputs =
    {{
        { stream::IAngular_Velocity::TYPE, m_init_params->rate, "Input" },
        { stream::IBattery_State::TYPE, m_init_params->rate, "Battery State" },
        { stream::ICommands::TYPE, m_init_params->rate, "Commands" }
    }};
    return inputs;
}
auto Multi_Pilot::get_stream_outputs() const -> std::vector<Stream_Output>
{
    std::vector<Stream_Output> outputs(0);
    return outputs;
}

void Multi_Pilot::process()
{
    QLOG_TOPIC("multi_pilot::process");
}

auto Multi_Pilot::set_config(rapidjson::Value const& json) -> bool
{
    QLOG_TOPIC("multi_pilot::set_config");

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

    auto angular_velocity_stream = m_hal.get_streams().find_by_name<stream::IAngular_Velocity>(sz.input_streams.angular_velocity);
    auto battery_state_stream = m_hal.get_streams().find_by_name<stream::IBattery_State>(sz.input_streams.battery_state);
    auto commands_stream = m_hal.get_streams().find_by_name<stream::ICommands>(sz.input_streams.commands);

    auto rate = angular_velocity_stream ? angular_velocity_stream->get_rate() : 0u;
    if (rate != m_output_stream->rate)
    {
        QLOGW("Bad input stream '{}'. Expected rate {}Hz, got {}Hz", sz.input_streams.angular_velocity, m_output_stream->rate, rate);
        m_config->input_streams.angular_velocity.clear();
        m_angular_velocity_stream.reset();
    }
    else
    {
        m_angular_velocity_stream = angular_velocity_stream;
    }

//    rate = location_stream ? location_stream->get_rate() : 0u;
//    if (rate != m_output_stream->rate)
//    {
//        m_config->input_streams.location.clear();
//        QLOGW("Bad input stream '{}'. Expected rate {}Hz, got {}Hz", sz.input_streams.location, m_output_stream->rate, rate);
//        m_location_stream.reset();
//    }
//    else
//    {
//        m_location_stream = location_stream;
//    }

    rate = battery_state_stream ? battery_state_stream->get_rate() : 0u;
    if (rate != m_output_stream->rate)
    {
        m_config->input_streams.battery_state.clear();
        QLOGW("Bad input stream '{}'. Expected rate {}Hz, got {}Hz", sz.input_streams.battery_state, m_output_stream->rate, rate);
        m_battery_state_stream.reset();
    }
    else
    {
        m_battery_state_stream = battery_state_stream;
    }

    rate = commands_stream ? commands_stream->get_rate() : 0u;
    if (rate != m_output_stream->rate)
    {
        m_config->input_streams.commands.clear();
        QLOGW("Bad input stream '{}'. Expected rate {}Hz, got {}Hz", sz.input_streams.commands, m_output_stream->rate, rate);
        m_commands_stream.reset();
    }
    else
    {
        m_commands_stream = commands_stream;
    }

    return true;
}
auto Multi_Pilot::get_config() const -> rapidjson::Document
{
    rapidjson::Document json;
    autojsoncxx::to_document(*m_config, json);
    return std::move(json);
}

auto Multi_Pilot::get_init_params() const -> rapidjson::Document const&
{
    return m_init_paramsj;
}

auto Multi_Pilot::send_message(rapidjson::Value const& /*json*/) -> rapidjson::Document
{
    return rapidjson::Document();
}

}
}
