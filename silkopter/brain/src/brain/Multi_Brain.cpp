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
        { stream::IFrame::TYPE,             m_init_params->rate, "Frame", m_accumulator.get_stream_path(1) },
        { stream::IECEF_Position::TYPE,     m_init_params->rate, "Position (ecef)", m_accumulator.get_stream_path(2) },
//        { stream::IBattery_State::TYPE,     m_init_params->rate, "Battery State", m_accumulator.get_stream_path(2) },
//        { stream::IECEF_Linear_Acceleration::TYPE, m_init_params->rate, "Linear Acceleration (ecef)", m_accumulator.get_stream_path(4) },
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

void Multi_Brain::process_input_mode_idle(stream::IMulti_Input::Value& new_input, silk::config::Multi const& multi_config)
{
    auto& crt_input = m_state.last_input;
    QASSERT(crt_input.mode.value == stream::IMulti_Input::Mode::IDLE);

    if (crt_input.vertical.thrust_rate.value != 0)
    {
        crt_input.vertical.thrust_rate.set(0);
    }
    if (crt_input.vertical.thrust_offset.value != 0)
    {
        crt_input.vertical.thrust_rate.set(0);
    }
    if (crt_input.vertical.climb_rate.value != 0)
    {
        crt_input.vertical.climb_rate.set(0);
    }

    if (!math::is_zero(crt_input.horizontal.angle_rate.value))
    {
        crt_input.horizontal.angle_rate.set(math::vec2f::zero);
    }
    if (!math::is_zero(crt_input.horizontal.angle.value))
    {
        crt_input.horizontal.angle.set(math::vec2f::zero);
    }
    if (!math::is_zero(crt_input.horizontal.velocity.value))
    {
        crt_input.horizontal.velocity.set(math::vec2f::zero);
    }

    if (crt_input.yaw.angle_rate.value != 0)
    {
        crt_input.yaw.angle_rate.set(0);
    }

    auto new_mode = new_input.mode;
    new_input = crt_input;
    new_input.mode = new_mode;

    m_rate_output_stream->push_sample(stream::IAngular_Velocity::Value(), true);
    m_thrust_output_stream->push_sample(stream::IForce::Value(), true);
}

void Multi_Brain::process_input_mode_armed(stream::IMulti_Input::Value& new_input, silk::config::Multi const& multi_config)
{
    auto& crt_input = m_state.last_input;
    QASSERT(crt_input.mode.value == stream::IMulti_Input::Mode::ARMED);

    stream::IForce::Value thrust = m_thrust_output_stream->get_last_sample().value;
    stream::IAngular_Velocity::Value rate = m_rate_output_stream->get_last_sample().value;

    if (crt_input.vertical.mode.value == stream::IMulti_Input::Vertical::Mode::THRUST_RATE)
    {
        thrust.z += crt_input.vertical.thrust_rate.value * q::Seconds(m_thrust_output_stream->get_dt()).count();
        thrust.z = math::clamp(thrust.z, 0.f, multi_config.motor_thrust * multi_config.motors.size());

        m_reference_thrust = thrust.z;
    }
    else if (crt_input.vertical.mode.value == stream::IMulti_Input::Vertical::Mode::THRUST_OFFSET)
    {
        thrust.z = m_reference_thrust + crt_input.vertical.thrust_offset.value;
        thrust.z = math::clamp(thrust.z, 0.f, multi_config.motor_thrust * multi_config.motors.size());
        //QLOGI("Thrust: {}", thrust.z);
    }

    if (crt_input.horizontal.mode.value == stream::IMulti_Input::Horizontal::Mode::ANGLE_RATE)
    {
        rate = crt_input.horizontal.angle_rate.value;
    }

    if (crt_input.yaw.mode.value == stream::IMulti_Input::Yaw::Mode::ANGLE_RATE)
    {
        rate.z = crt_input.yaw.angle_rate.value;
    }

    m_rate_output_stream->push_sample(rate, true);
    m_thrust_output_stream->push_sample(thrust, true);
}

void Multi_Brain::process_input(stream::IMulti_Input::Value const& new_input, silk::config::Multi const& multi_config)
{
    stream::IMulti_Input::Value new_input_copy = new_input;

    if (m_state.last_input.mode.value == stream::IMulti_Input::Mode::IDLE)
    {
        process_input_mode_idle(new_input_copy, multi_config);
    }
    else if (m_state.last_input.mode.value == stream::IMulti_Input::Mode::ARMED)
    {
        process_input_mode_armed(new_input_copy, multi_config);
    }

    m_state.last_input = new_input_copy;
}

void Multi_Brain::acquire_home_position(stream::IECEF_Position::Sample const& sample)
{
    if (m_state.last_input.mode.value == stream::IMulti_Input::Mode::IDLE)
    {
        m_home.ecef_position = sample.value;
        m_home.lla_position = util::coordinates::ecef_to_lla(m_home.ecef_position);
        m_home.enu_to_ecef_trans = util::coordinates::enu_to_ecef_transform(m_home.lla_position);
        m_home.ecef_to_enu_trans = math::inverse(m_home.enu_to_ecef_trans);
    }
}

void Multi_Brain::process()
{
    QLOG_TOPIC("multi_Brain::process");

    auto multi_config = m_hal.get_multi_config();
    if (!multi_config)
    {
        return;
    }

    m_state_output_stream->clear();
    m_rate_output_stream->clear();
    m_thrust_output_stream->clear();

    m_accumulator.process([this, &multi_config](
                          size_t idx,
                          stream::IMulti_Input::Sample const& i_input,
                          stream::IFrame::Sample const& i_frame,
                          stream::IECEF_Position::Sample const& i_position
//                          stream::IBattery_State::Sample const& i_battery_state,
//                          stream::IECEF_Linear_Acceleration::Sample const& i_linear_acceleration,
//                          stream::IECEF_Velocity::Sample const& i_velocity,
//                          stream::IProximity::Sample const& i_proximity,
                          )
    {
        acquire_home_position(i_position);
        process_input(i_input.value, *multi_config);

        m_state.ecef_position = i_position.value;
        m_state.ecef_home_position = m_home.ecef_position;
        m_state.frame = i_frame.value;
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
