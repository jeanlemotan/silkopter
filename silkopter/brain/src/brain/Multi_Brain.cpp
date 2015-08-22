#include "BrainStdAfx.h"
#include "Multi_Brain.h"

#include "sz_math.hpp"
#include "sz_PID.hpp"
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
    auto multi_config = m_hal.get_multi_config();
    if (!multi_config)
    {
        QLOGE("No multi config found");
        return false;
    }
    m_state.config = *multi_config;

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

void Multi_Brain::process_state_mode_idle()
{
    auto& input = m_state.input.value;
    QASSERT(input.mode.value == stream::IMulti_Input::Mode::IDLE);

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

void Multi_Brain::process_state_mode_armed()
{
    auto& input = m_state.input.value;
    QASSERT(input.mode.value == stream::IMulti_Input::Mode::ARMED);

    stream::IForce::Value thrust = m_thrust_output_stream->get_last_sample().value;
    stream::IAngular_Velocity::Value rate = m_rate_output_stream->get_last_sample().value;

    if (input.vertical.mode.value == stream::IMulti_Input::Vertical::Mode::THRUST_RATE)
    {
        thrust.z += input.vertical.thrust_rate.value * q::Seconds(m_thrust_output_stream->get_dt()).count();
        thrust.z = math::clamp(thrust.z, 0.f, m_state.config.motor_thrust * m_state.config.motors.size());

        m_reference_thrust = thrust.z;
    }
    else if (input.vertical.mode.value == stream::IMulti_Input::Vertical::Mode::THRUST_OFFSET)
    {
        thrust.z = m_reference_thrust + input.vertical.thrust_offset.value;
        thrust.z = math::clamp(thrust.z, 0.f, m_state.config.motor_thrust * m_state.config.motors.size());
        //QLOGI("Thrust: {}", thrust.z);
    }

    if (input.horizontal.mode.value == stream::IMulti_Input::Horizontal::Mode::ANGLE_RATE)
    {
        rate = input.horizontal.angle_rate.value;
    }
    else if (input.horizontal.mode.value == stream::IMulti_Input::Horizontal::Mode::ANGLE)
    {
        float fx, fy, fz;
        m_state.frame.value.get_as_euler_xyz(fx, fy, fz);

        math::quatf target;
        target.set_from_euler_zxy(input.horizontal.angle.value.x, input.horizontal.angle.value.y, fz);
        math::quatf diff = math::inverse(m_state.frame.value) * target;
        float diff_x, diff_y, _z;
        diff.get_as_euler_zxy(diff_x, diff_y, _z);

        float max_speed = math::radians(m_config->horizontal_angle.max_speed_deg);

        float x = m_horizontal_angle_data.x_pid.process(-diff_x, 0.f);
        x = math::clamp(x, -max_speed, max_speed);
        float y = m_horizontal_angle_data.y_pid.process(-diff_y, 0.f);
        y = math::clamp(y, -max_speed, max_speed);

        rate.set(x, y, 0);
    }


    if (input.yaw.mode.value == stream::IMulti_Input::Yaw::Mode::ANGLE_RATE)
    {
        rate.z = input.yaw.angle_rate.value;
    }

    m_rate_output_stream->push_sample(rate, true);
    m_thrust_output_stream->push_sample(thrust, true);
}

void Multi_Brain::process_state()
{
    if (m_state.input.value.mode.value == stream::IMulti_Input::Mode::IDLE)
    {
        process_state_mode_idle();
    }
    else if (m_state.input.value.mode.value == stream::IMulti_Input::Mode::ARMED)
    {
        process_state_mode_armed();
    }
}

void Multi_Brain::acquire_home_position()
{    auto multi_config = m_hal.get_multi_config();
     if (!multi_config)
     {
         return;
     }
    if (m_state.input.value.mode.value == stream::IMulti_Input::Mode::IDLE)
    {
        m_home.ecef_position = m_state.position.value;
        m_home.lla_position = util::coordinates::ecef_to_lla(m_home.ecef_position);
        m_home.enu_to_ecef_trans = util::coordinates::enu_to_ecef_transform(m_home.lla_position);
        m_home.ecef_to_enu_trans = math::inverse(m_home.enu_to_ecef_trans);
    }
}

void Multi_Brain::refresh_state(stream::IMulti_Input::Sample const& input,
                                     stream::IFrame::Sample const& frame,
                                     stream::IECEF_Position::Sample const& position)
{
    auto now = q::Clock::now();

    if (input.is_healthy)
    {
        m_state.input.value = input.value;
        m_state.input.last_updated_tp = now;
    }
    if (frame.is_healthy)
    {
        m_state.frame.value = frame.value;
        m_state.frame.last_updated_tp = now;
    }
    if (position.is_healthy)
    {
        m_state.position.value = position.value;
        m_state.position.last_updated_tp = now;
    }
}

void Multi_Brain::process()
{
    QLOG_TOPIC("multi_Brain::process");

    m_state_output_stream->clear();
    m_rate_output_stream->clear();
    m_thrust_output_stream->clear();

    m_accumulator.process([this](
                          size_t,
                          stream::IMulti_Input::Sample const& i_input,
                          stream::IFrame::Sample const& i_frame,
                          stream::IECEF_Position::Sample const& i_position
//                          stream::IBattery_State::Sample const& i_battery_state,
//                          stream::IECEF_Linear_Acceleration::Sample const& i_linear_acceleration,
//                          stream::IECEF_Velocity::Sample const& i_velocity,
//                          stream::IProximity::Sample const& i_proximity,
                          )
    {
        refresh_state(i_input, i_frame, i_position);
        acquire_home_position();
        process_state();
    });


    {
        stream::IMulti_State::Value state;
        state.ecef_position = m_state.position.value;
        state.ecef_home_position = m_home.ecef_position;
        state.frame = m_state.frame.value;

        size_t samples_needed = m_state_output_stream->compute_samples_needed();
        for (size_t i = 0; i < samples_needed; i++)
        {
            m_state_output_stream->push_sample(state, true);
        }
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

    m_config->horizontal_angle.max_speed_deg = math::clamp(m_config->horizontal_angle.max_speed_deg, 0.f, 180.f);

    auto fill_params = [this](PID::Params& dst, sz::PID const& src)
    {
        dst.kp = src.kp;
        dst.ki = src.ki;
        dst.kd = src.kd;
        dst.max_i = src.max_i;
        dst.d_filter = src.d_filter;
        dst.rate = m_rate_output_stream->get_rate();
    };

    {
        PID::Params x_params, y_params;
        if (m_config->horizontal_angle.combined_pids)
        {
            fill_params(x_params, m_config->horizontal_angle.pids);
            fill_params(y_params, m_config->horizontal_angle.pids);
        }
        else
        {
            fill_params(x_params, m_config->horizontal_angle.x_pid);
            fill_params(y_params, m_config->horizontal_angle.y_pid);
        }

        if (!m_horizontal_angle_data.x_pid.set_params(x_params) ||
                !m_horizontal_angle_data.y_pid.set_params(y_params))
        {
            QLOGE("Bad PID params");
            return false;
        }
    }


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
