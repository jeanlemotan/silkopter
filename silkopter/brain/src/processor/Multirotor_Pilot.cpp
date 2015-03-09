#include "BrainStdAfx.h"
#include "Multirotor_Pilot.h"

#include "sz_math.hpp"
#include "sz_Multirotor_Pilot.hpp"

namespace silk
{
namespace node
{

Multirotor_Pilot::Multirotor_Pilot(HAL& hal)
    : m_hal(hal)
    , m_init_params(new sz::Multirotor_Pilot::Init_Params())
    , m_config(new sz::Multirotor_Pilot::Config())
{
}

auto Multirotor_Pilot::init(rapidjson::Value const& init_params) -> bool
{
    QLOG_TOPIC("multirotor_pilot::init");

    sz::Multirotor_Pilot::Init_Params sz;
    autojsoncxx::error::ErrorStack result;
    if (!autojsoncxx::from_value(sz, init_params, result))
    {
        std::ostringstream ss;
        ss << result;
        QLOGE("Cannot deserialize Multirotor_Pilot data: {}", ss.str());
        return false;
    }
    *m_init_params = sz;
    return init();
}

auto Multirotor_Pilot::init() -> bool
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

auto Multirotor_Pilot::get_inputs() const -> std::vector<Input>
{
    std::vector<Input> inputs(6);
    inputs[0].class_id = q::rtti::get_class_id<stream::IReference_Frame>();
    inputs[0].rate = m_output_stream ? m_output_stream->rate : 0;
    inputs[0].name = "Reference Frame";
    inputs[1].class_id = q::rtti::get_class_id<stream::IAngular_Velocity>();
    inputs[1].rate = m_output_stream ? m_output_stream->rate : 0;
    inputs[1].name = "Angular Velocity";
    inputs[2].class_id = q::rtti::get_class_id<stream::ICardinal_Points>();
    inputs[2].rate = m_output_stream ? m_output_stream->rate : 0;
    inputs[2].name = "Cardinal Points";
    inputs[3].class_id = q::rtti::get_class_id<stream::ILocation>();
    inputs[3].rate = m_output_stream ? m_output_stream->rate : 0;
    inputs[3].name = "Location";
    inputs[4].class_id = q::rtti::get_class_id<stream::IBattery_State>();
    inputs[4].rate = m_output_stream ? m_output_stream->rate : 0;
    inputs[4].name = "Battery State";
    inputs[5].class_id = q::rtti::get_class_id<stream::IMultirotor_Input>();
    inputs[5].rate = m_output_stream ? m_output_stream->rate : 0;
    inputs[5].name = "Multirotor Input";
    return inputs;
}
auto Multirotor_Pilot::get_outputs() const -> std::vector<Output>
{
    std::vector<Output> outputs(1);
    outputs[0].class_id = q::rtti::get_class_id<stream::IReference_Frame>();
    outputs[0].name = "Reference Frame";
    outputs[0].stream = m_output_stream;
    return outputs;
}

void Multirotor_Pilot::process()
{
}

auto Multirotor_Pilot::set_config(rapidjson::Value const& json) -> bool
{
    sz::Multirotor_Pilot::Config sz;
    autojsoncxx::error::ErrorStack result;
    if (!autojsoncxx::from_value(sz, json, result))
    {
        std::ostringstream ss;
        ss << result;
        QLOGE("Cannot deserialize Multirotor_Pilot config data: {}", ss.str());
        return false;
    }

    *m_config = sz;

    auto reference_frame_stream = m_hal.get_streams().find_by_name<stream::IReference_Frame>(sz.inputs.reference_frame);
    auto angular_velocity_stream = m_hal.get_streams().find_by_name<stream::IAngular_Velocity>(sz.inputs.angular_velocity);
    auto cardinal_points_stream = m_hal.get_streams().find_by_name<stream::ICardinal_Points>(sz.inputs.cardinal_points);
    auto location_stream = m_hal.get_streams().find_by_name<stream::ILocation>(sz.inputs.location);
    auto battery_state_stream = m_hal.get_streams().find_by_name<stream::IBattery_State>(sz.inputs.battery_state);
    auto multirotor_input_stream = m_hal.get_streams().find_by_name<stream::IMultirotor_Input>(sz.inputs.multirotor_input);

    auto rate = reference_frame_stream ? reference_frame_stream->get_rate() : 0u;
    if (rate != m_output_stream->rate)
    {
        m_config->inputs.reference_frame.clear();
        QLOGE("Bad input stream '{}'. Expected rate {}Hz, got {}Hz", sz.inputs.reference_frame, m_output_stream->rate, rate);
        return false;
    }
    m_reference_frame_stream = reference_frame_stream;

    rate = angular_velocity_stream ? angular_velocity_stream->get_rate() : 0u;
    if (rate != m_output_stream->rate)
    {
        m_config->inputs.angular_velocity.clear();
        QLOGE("Bad input stream '{}'. Expected rate {}Hz, got {}Hz", sz.inputs.angular_velocity, m_output_stream->rate, rate);
        return false;
    }
    m_angular_velocity_stream = angular_velocity_stream;

    rate = cardinal_points_stream ? cardinal_points_stream->get_rate() : 0u;
    if (rate != m_output_stream->rate)
    {
        m_config->inputs.cardinal_points.clear();
        QLOGE("Bad input stream '{}'. Expected rate {}Hz, got {}Hz", sz.inputs.cardinal_points, m_output_stream->rate, rate);
        return false;
    }
    m_cardinal_points_stream = cardinal_points_stream;

    rate = location_stream ? location_stream->get_rate() : 0u;
    if (rate != m_output_stream->rate)
    {
        m_config->inputs.location.clear();
        QLOGE("Bad input stream '{}'. Expected rate {}Hz, got {}Hz", sz.inputs.location, m_output_stream->rate, rate);
        return false;
    }
    m_location_stream = location_stream;

    rate = battery_state_stream ? battery_state_stream->get_rate() : 0u;
    if (rate != m_output_stream->rate)
    {
        m_config->inputs.battery_state.clear();
        QLOGE("Bad input stream '{}'. Expected rate {}Hz, got {}Hz", sz.inputs.battery_state, m_output_stream->rate, rate);
        return false;
    }
    m_battery_state_stream = battery_state_stream;

    rate = multirotor_input_stream ? multirotor_input_stream->get_rate() : 0u;
    if (rate != m_output_stream->rate)
    {
        m_config->inputs.multirotor_input.clear();
        QLOGE("Bad input stream '{}'. Expected rate {}Hz, got {}Hz", sz.inputs.multirotor_input, m_output_stream->rate, rate);
        return false;
    }
    m_multirotor_input_stream = multirotor_input_stream;

    return true;
}
auto Multirotor_Pilot::get_config() const -> rapidjson::Document
{
    rapidjson::Document json;
    autojsoncxx::to_document(*m_config, json);
    return std::move(json);
}

auto Multirotor_Pilot::get_init_params() const -> rapidjson::Document
{
    rapidjson::Document json;
    autojsoncxx::to_document(*m_init_params, json);
    return std::move(json);
}

}
}
