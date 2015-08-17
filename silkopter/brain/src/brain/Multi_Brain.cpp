#include "BrainStdAfx.h"
#include "Multi_Brain.h"

#include "sz_math.hpp"
#include "sz_Multi_Brain.hpp"

namespace silk
{
namespace node
{

Multi_Brain::Multi_Brain(HAL& hal)
    : m_hal(hal)
    , m_init_params(new sz::Multi_Brain::Init_Params())
    , m_config(new sz::Multi_Brain::Config())
{
    m_state_output_stream = std::make_shared<State_Output_Stream>();
    m_rate_output_stream = std::make_shared<Rate_Output_Stream>();
    m_thrust_output_stream = std::make_shared<Thrust_Output_Stream>();
}

auto Multi_Brain::init(rapidjson::Value const& init_params) -> bool
{
    QLOG_TOPIC("multi_Brain::init");

    sz::Multi_Brain::Init_Params sz;
    autojsoncxx::error::ErrorStack result;
    if (!autojsoncxx::from_value(sz, init_params, result))
    {
        std::ostringstream ss;
        ss << result;
        QLOGE("Cannot deserialize Multi_Brain data: {}", ss.str());
        return false;
    }
    *m_init_params = sz;
    return init();
}

auto Multi_Brain::init() -> bool
{
    if (m_init_params->rate == 0)
    {
        QLOGE("Bad rate: {}Hz", m_init_params->rate);
        return false;
    }
    m_state_output_stream->set_rate(m_init_params->state_rate);
    m_state_output_stream->set_tp(q::Clock::now());

    m_rate_output_stream->set_rate(m_init_params->rate);
    m_rate_output_stream->set_tp(q::Clock::now());

    m_thrust_output_stream->set_rate(m_init_params->rate);
    m_thrust_output_stream->set_tp(q::Clock::now());

    return true;
}

auto Multi_Brain::get_inputs() const -> std::vector<Input>
{
    std::vector<Input> inputs =
    {{
        { stream::IMulti_Input::TYPE,       m_init_params->rate, "Input", m_accumulator.get_stream_path(0) },
        { stream::IAngular_Velocity::TYPE,  m_init_params->rate, "Angular Velocity", m_accumulator.get_stream_path(1) },
        { stream::IFrame::TYPE,             m_init_params->rate, "Frame", m_accumulator.get_stream_path(2) },
        { stream::IMagnetic_Field::TYPE,    m_init_params->rate, "Magnetic Field", m_accumulator.get_stream_path(3) },
//        { stream::IBattery_State::TYPE,     m_init_params->rate, "Battery State", m_accumulator.get_stream_path(2) },
//        { stream::IECEF_Linear_Acceleration::TYPE, m_init_params->rate, "Linear Acceleration (ecef)", m_accumulator.get_stream_path(4) },
//        { stream::IECEF_Position::TYPE,     m_init_params->rate, "Position (ecef)", m_accumulator.get_stream_path(5) },
//        { stream::IECEF_Velocity::TYPE,     m_init_params->rate, "Velocity (ecef)", m_accumulator.get_stream_path(6) },
//        { stream::IProximity::TYPE,         m_init_params->rate, "Proximity", m_accumulator.get_stream_path(7) },
    }};
    return inputs;
}
auto Multi_Brain::get_outputs() const -> std::vector<Output>
{
    std::vector<Output> outputs =
    {{
         { "State",        m_state_output_stream },
         { "Rate",         m_rate_output_stream },
         { "Thrust",       m_thrust_output_stream },
    }};
    return outputs;
}

void Multi_Brain::process_input(stream::IMulti_Input::Value const& req_input)
{
    auto& input = m_state.last_input;
    input = req_input;

    if (input.mode.value == stream::IMulti_Input::Mode::IDLE)
    {
        if (input.vertical.thrust_rate.value != 0)
        {
            input.vertical.thrust_rate.set(0);
        }
        if (input.vertical.thrust_offset.value != 0)
        {
            input.vertical.thrust_rate.set(0);
        }
        if (input.vertical.climb_rate.value != 0)
        {
            input.vertical.climb_rate.set(0);
        }

        if (!math::is_zero(input.horizontal.angle_rate.value))
        {
            input.horizontal.angle_rate.set(math::vec2f::zero);
        }
        if (!math::is_zero(input.horizontal.angle.value))
        {
            input.horizontal.angle.set(math::vec2f::zero);
        }
        if (!math::is_zero(input.horizontal.velocity.value))
        {
            input.horizontal.velocity.set(math::vec2f::zero);
        }

        if (input.yaw.angle_rate.value != 0)
        {
            input.yaw.angle_rate.set(0);
        }

        m_rate_output_stream->push_sample(stream::IAngular_Velocity::Value(), true);
        m_thrust_output_stream->push_sample(stream::IForce::Value(), true);
    }
    else
    {
        stream::IForce::Value thrust = m_thrust_output_stream->get_last_sample().value;
        stream::IAngular_Velocity::Value rate = m_rate_output_stream->get_last_sample().value;


        if (input.vertical.mode.value == stream::IMulti_Input::Vertical::Mode::THRUST_RATE)
        {
            thrust.z += input.vertical.thrust_rate.value * q::Seconds(m_thrust_output_stream->get_dt()).count();
            m_reference_thrust = thrust.z;
        }
        else if (input.vertical.mode.value == stream::IMulti_Input::Vertical::Mode::THRUST_OFFSET)
        {
            thrust.z = m_reference_thrust + input.vertical.thrust_offset.value;
        }

        if (input.horizontal.mode.value == stream::IMulti_Input::Horizontal::Mode::ANGLE_RATE)
        {
            rate = input.horizontal.angle_rate.value;
        }

        if (input.yaw.mode.value == stream::IMulti_Input::Yaw::Mode::ANGLE_RATE)
        {
            rate.z = input.yaw.angle_rate.value;
        }

        m_rate_output_stream->push_sample(rate, true);
        m_thrust_output_stream->push_sample(thrust, true);
    }
}

void Multi_Brain::process()
{
    QLOG_TOPIC("multi_Brain::process");

    m_state_output_stream->clear();
    m_rate_output_stream->clear();
    m_thrust_output_stream->clear();

    m_accumulator.process([this](
                          size_t idx,
                          stream::IMulti_Input::Sample const& i_input,
                          stream::IAngular_Velocity::Sample const& i_angular_velocity,
                          stream::IFrame::Sample const& i_frame,
                          stream::IMagnetic_Field::Sample const& i_magneic_field
//                          stream::IBattery_State::Sample const& i_battery_state,
//                          stream::IECEF_Linear_Acceleration::Sample const& i_linear_acceleration,
//                          stream::IECEF_Position::Sample const& i_position,
//                          stream::IECEF_Velocity::Sample const& i_velocity,
//                          stream::IProximity::Sample const& i_proximity,
                          )
    {
        process_input(i_input.value);
    });


    size_t samples_needed = m_state_output_stream->compute_samples_needed();
    for (size_t i = 0; i < samples_needed; i++)
    {
        m_state_output_stream->push_sample(m_state, true);
    }
}

void Multi_Brain::set_input_stream_path(size_t idx, q::Path const& path)
{
    m_accumulator.set_stream_path(idx, path, m_init_params->rate, m_hal);
}

auto Multi_Brain::set_config(rapidjson::Value const& json) -> bool
{
    QLOG_TOPIC("multi_Brain::set_config");

    sz::Multi_Brain::Config sz;
    autojsoncxx::error::ErrorStack result;
    if (!autojsoncxx::from_value(sz, json, result))
    {
        std::ostringstream ss;
        ss << result;
        QLOGE("Cannot deserialize Multi_Brain config data: {}", ss.str());
        return false;
    }

    *m_config = sz;

    return true;
}
auto Multi_Brain::get_config() const -> rapidjson::Document
{
    rapidjson::Document json;
    autojsoncxx::to_document(*m_config, json);
    return std::move(json);
}

auto Multi_Brain::get_init_params() const -> rapidjson::Document
{
    rapidjson::Document json;
    autojsoncxx::to_document(*m_init_params, json);
    return std::move(json);
}

auto Multi_Brain::send_message(rapidjson::Value const& /*json*/) -> rapidjson::Document
{
    return rapidjson::Document();
}

}
}
