#include "BrainStdAfx.h"
#include "Pilot.h"

#include "sz_math.hpp"
#include "sz_Pilot.hpp"

namespace silk
{
namespace node
{

Pilot::Pilot(HAL& hal)
    : m_hal(hal)
    , m_init_params(new sz::Pilot::Init_Params())
    , m_config(new sz::Pilot::Config())
{
    autojsoncxx::to_document(*m_init_params, m_init_paramsj);
}

auto Pilot::init(rapidjson::Value const& init_params) -> bool
{
    QLOG_TOPIC("pilot::init");

    sz::Pilot::Init_Params sz;
    autojsoncxx::error::ErrorStack result;
    if (!autojsoncxx::from_value(sz, init_params, result))
    {
        std::ostringstream ss;
        ss << result;
        QLOGE("Cannot deserialize Pilot data: {}", ss.str());
        return false;
    }
    jsonutil::clone_value(m_init_paramsj, init_params, m_init_paramsj.GetAllocator());
    *m_init_params = sz;
    return init();
}

auto Pilot::init() -> bool
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

auto Pilot::get_inputs() const -> std::vector<Input>
{
    std::vector<Input> inputs(3);
    inputs[0].type = IAngular_Velocity_Stream::TYPE;
    inputs[0].rate = m_output_stream ? m_output_stream->rate : 0;
    inputs[0].name = "Angular Velocity";
    inputs[1].type = IBattery_State_Stream::TYPE;
    inputs[1].rate = m_output_stream ? m_output_stream->rate : 0;
    inputs[1].name = "Battery State";
    inputs[2].type = ICommands_Stream::TYPE;
    inputs[2].rate = m_output_stream ? m_output_stream->rate : 0;
    inputs[2].name = "Commands";
    return inputs;
}
auto Pilot::get_outputs() const -> std::vector<Output>
{
    std::vector<Output> outputs(0);
    return outputs;
}

void Pilot::process()
{
    QLOG_TOPIC("pilot::process");
}

auto Pilot::set_config(rapidjson::Value const& json) -> bool
{
    QLOG_TOPIC("pilot::set_config");

    sz::Pilot::Config sz;
    autojsoncxx::error::ErrorStack result;
    if (!autojsoncxx::from_value(sz, json, result))
    {
        std::ostringstream ss;
        ss << result;
        QLOGE("Cannot deserialize Pilot config data: {}", ss.str());
        return false;
    }

    *m_config = sz;

    auto angular_velocity_stream = m_hal.get_streams().find_by_name<IAngular_Velocity_Stream>(sz.inputs.angular_velocity);
    auto battery_state_stream = m_hal.get_streams().find_by_name<IBattery_State_Stream>(sz.inputs.battery_state);
    auto commands_stream = m_hal.get_streams().find_by_name<ICommands_Stream>(sz.inputs.commands);

    auto rate = angular_velocity_stream ? angular_velocity_stream->get_rate() : 0u;
    if (rate != m_output_stream->rate)
    {
        QLOGE("Bad input stream '{}'. Expected rate {}Hz, got {}Hz", sz.inputs.angular_velocity, m_output_stream->rate, rate);
        m_config->inputs.angular_velocity.clear();
        m_angular_velocity_stream.reset();
    }
    else
    {
        m_angular_velocity_stream = angular_velocity_stream;
    }

//    rate = location_stream ? location_stream->get_rate() : 0u;
//    if (rate != m_output_stream->rate)
//    {
//        m_config->inputs.location.clear();
//        QLOGE("Bad input stream '{}'. Expected rate {}Hz, got {}Hz", sz.inputs.location, m_output_stream->rate, rate);
//        m_location_stream.reset();
//    }
//    else
//    {
//        m_location_stream = location_stream;
//    }

    rate = battery_state_stream ? battery_state_stream->get_rate() : 0u;
    if (rate != m_output_stream->rate)
    {
        m_config->inputs.battery_state.clear();
        QLOGE("Bad input stream '{}'. Expected rate {}Hz, got {}Hz", sz.inputs.battery_state, m_output_stream->rate, rate);
        m_battery_state_stream.reset();
    }
    else
    {
        m_battery_state_stream = battery_state_stream;
    }

    rate = commands_stream ? commands_stream->get_rate() : 0u;
    if (rate != m_output_stream->rate)
    {
        m_config->inputs.commands.clear();
        QLOGE("Bad input stream '{}'. Expected rate {}Hz, got {}Hz", sz.inputs.commands, m_output_stream->rate, rate);
        m_commands_stream.reset();
    }
    else
    {
        m_commands_stream = commands_stream;
    }

    return true;
}
auto Pilot::get_config() const -> rapidjson::Document
{
    rapidjson::Document json;
    autojsoncxx::to_document(*m_config, json);
    return std::move(json);
}

auto Pilot::get_init_params() const -> rapidjson::Document const&
{
    return m_init_paramsj;
}

}
}
