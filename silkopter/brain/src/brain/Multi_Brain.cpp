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

    m_dts = std::chrono::duration<double>(m_thrust_output_stream->get_dt()).count();

    m_config->altitude.lpf_cutoff_frequency = 1;

    return true;
}

auto Multi_Brain::get_inputs() const -> std::vector<Input>
{
    std::vector<Input> inputs =
    {{
         { stream::IMulti_Input::TYPE,       m_init_params->rate, "Input", m_accumulator.get_stream_path(0) },
         { stream::IFrame::TYPE,             m_init_params->rate, "Frame", m_accumulator.get_stream_path(1) },
         { stream::IECEF_Position::TYPE,     m_init_params->rate, "Position (ecef)", m_accumulator.get_stream_path(2) },
         { stream::IECEF_Velocity::TYPE,     m_init_params->rate, "Velocity (ecef)", m_accumulator.get_stream_path(3) },
         { stream::IECEF_Acceleration::TYPE, m_init_params->rate, "Acceleration (ecef)", m_accumulator.get_stream_path(4) },
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

    if (m_state.input.last_value.mode.value != stream::IMulti_Input::Mode::IDLE)
    {
        QLOGI("Reacquiring Home");
        m_home.is_acquired = false;
        m_home.ecef_position_history.clear();
    }

    if (input.vertical.thrust_rate.value != 0)
    {
        input.vertical.thrust_rate.set(0);
    }
    if (input.vertical.thrust_offset.value != 0)
    {
        input.vertical.thrust_rate.set(0);
    }
    if (input.vertical.velocity.value != 0)
    {
        input.vertical.velocity.set(0);
    }

    if (!math::is_zero(input.horizontal.angle_rate.value))
    {
        input.horizontal.angle_rate.set(math::vec2d::zero);
    }
    if (!math::is_zero(input.horizontal.angle.value))
    {
        input.horizontal.angle.set(math::vec2d::zero);
    }
    if (!math::is_zero(input.horizontal.velocity.value))
    {
        input.horizontal.velocity.set(math::vec2d::zero);
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

    if (!m_home.is_acquired)
    {
        QLOGW("Trying to arm but Home is not acquired yet. Ignoring request");
        input.mode.set(stream::IMulti_Input::Mode::IDLE);
        return;
    }

    stream::IForce::Value thrust = m_thrust_output_stream->get_last_sample().value;
    stream::IAngular_Velocity::Value rate = m_rate_output_stream->get_last_sample().value;

    //////////////////////////////////////////////////////////////
    // Verticals

    math::vec3d enu_position = math::transform(m_home.ecef_to_enu_transform, m_state.position.last_value);

    if (input.vertical.mode.value == stream::IMulti_Input::Vertical::Mode::THRUST_RATE)
    {
        thrust.z += input.vertical.thrust_rate.value * m_dts;
    }
    else if (input.vertical.mode.value == stream::IMulti_Input::Vertical::Mode::THRUST_OFFSET)
    {
        thrust.z = m_reference_thrust + input.vertical.thrust_offset.value;
    }
    else if (input.vertical.mode.value == stream::IMulti_Input::Vertical::Mode::VELOCITY)
    {
        double input_velocity = input.vertical.velocity.value;
        m_altitude_data.reference_altitude += input_velocity * m_dts;

        double target_alt = m_altitude_data.reference_altitude;

        double crt_alt = enu_position.z;
        double target_vel = m_altitude_data.velocity_pd.process(crt_alt, target_alt);

        double crt_vel = math::transform(m_home.ecef_to_enu_rotation, m_state.velocity.last_value).z;
        double target_acc = m_altitude_data.velocity_pd.process(crt_vel, target_vel);

        double crt_acc = math::transform(m_home.ecef_to_enu_rotation, m_state.acceleration.last_value).z;

        double half_thrust = (m_config->max_thrust + m_config->min_thrust) * 0.5;
        double t =  half_thrust + m_altitude_data.acceleration_pid.process(crt_acc, target_acc);
        m_altitude_data.dsp.process(t);
        thrust.z = t;
    }
    thrust.z = math::clamp(thrust.z, m_config->min_thrust, m_config->max_thrust);

    //refresh vertical references
    if (input.vertical.mode.value != stream::IMulti_Input::Vertical::Mode::THRUST_OFFSET)
    {
        m_reference_thrust = thrust.z;
    }
    if (input.vertical.mode.value != stream::IMulti_Input::Vertical::Mode::VELOCITY)
    {
        m_altitude_data.reference_altitude = enu_position.z;
    }

    ////////////////////////////////////////////////////////////
    // Horizontals

    if (input.horizontal.mode.value == stream::IMulti_Input::Horizontal::Mode::ANGLE_RATE)
    {
        rate = input.horizontal.angle_rate.value;
    }
    else if (input.horizontal.mode.value == stream::IMulti_Input::Horizontal::Mode::ANGLE)
    {
        double fx, fy, fz;
        m_state.frame.value.get_as_euler_xyz(fx, fy, fz);

        math::quatd target;
        target.set_from_euler_zxy(input.horizontal.angle.value.x, input.horizontal.angle.value.y, fz);
        math::quatd diff = math::inverse(m_state.frame.value) * target;
        double diff_x, diff_y, _z;
        diff.get_as_euler_zxy(diff_x, diff_y, _z);

        double max_speed = math::radians(m_config->horizontal_angle.max_speed_deg);

        double x = m_horizontal_angle_data.x_pid.process(-diff_x, 0.0);
        x = math::clamp(x, -max_speed, max_speed);
        double y = m_horizontal_angle_data.y_pid.process(-diff_y, 0.0);
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
{
    auto multi_config = m_hal.get_multi_config();
    if (!multi_config)
    {
        return;
    }
    if (m_state.input.value.mode.value == stream::IMulti_Input::Mode::IDLE)
    {
        auto& history = m_home.ecef_position_history;
        auto per_second = static_cast<size_t>(1.0 / m_dts);
        history.push_back(m_state.position.value);
#ifdef NDEBUG
        while (history.size() > 10 * per_second + 1)
#else
        while (history.size() > 3 * per_second + 1)
#endif
        {
            if (!m_home.is_acquired)
            {
                QLOGI("Home acquired!!!");
                m_home.is_acquired = true;
            }

            history.pop_front();
        }
        util::coordinates::ECEF avg;
        double mul = 1.0 / double(history.size());
        for (auto const& h: history)
        {
            avg += h * mul;
        }
        auto avg_ = std::accumulate(history.begin(), history.end(), util::coordinates::ECEF(0));
        avg_ /= double(history.size());

        m_home.ecef_position = avg;
        m_home.lla_position = util::coordinates::ecef_to_lla(m_home.ecef_position);
        m_home.enu_to_ecef_transform = util::coordinates::enu_to_ecef_transform(m_home.lla_position);
        m_home.ecef_to_enu_transform = math::inverse(m_home.enu_to_ecef_transform);
        m_home.enu_to_ecef_rotation = util::coordinates::enu_to_ecef_rotation(m_home.lla_position);
        m_home.ecef_to_enu_rotation = math::inverse(m_home.enu_to_ecef_rotation);
    }
}

void Multi_Brain::refresh_state(stream::IMulti_Input::Sample const& input,
                                stream::IFrame::Sample const& frame,
                                stream::IECEF_Position::Sample const& position,
                                stream::IECEF_Velocity::Sample const& velocity,
                                stream::IECEF_Acceleration::Sample const& acceleration)
{
    auto now = q::Clock::now();

    if (input.is_healthy)
    {
        m_state.input.last_value = m_state.input.value;
        m_state.input.value = input.value;
        m_state.input.last_updated_tp = now;
    }
    if (frame.is_healthy)
    {
        m_state.frame.last_value = m_state.frame.value;
        m_state.frame.value = frame.value;
        m_state.frame.last_updated_tp = now;
    }
    if (position.is_healthy)
    {
        m_state.position.last_value = m_state.position.value;
        m_state.position.value = position.value;
        m_state.position.last_updated_tp = now;
    }
    if (velocity.is_healthy)
    {
        m_state.velocity.last_value = m_state.velocity.value;
        m_state.velocity.value = velocity.value;
        m_state.velocity.last_updated_tp = now;
    }
    if (acceleration.is_healthy)
    {
        m_state.acceleration.last_value = m_state.acceleration.value;
        m_state.acceleration.value = acceleration.value;
        m_state.acceleration.last_updated_tp = now;
    }
}

void Multi_Brain::process()
{
    QLOG_TOPIC("multi_Brain::process");

    {
        static q::Clock::time_point last_timestamp = q::Clock::now();
        auto now = q::Clock::now();
        auto dt = now - last_timestamp;
        last_timestamp = now;
        static q::Clock::duration min_dt, max_dt, avg_dt;
        static int xxx = 0;
        if (xxx == 0)
        {
            QLOGI("min {}, max {}, avg {}", min_dt, max_dt, avg_dt);
            min_dt = dt;
            max_dt = dt;
            avg_dt = std::chrono::milliseconds(0);
        }
        min_dt = std::min(min_dt, dt);
        max_dt = std::max(max_dt, dt);
        avg_dt += dt;
        xxx++;
        if (xxx == 1000)
        {
            avg_dt = avg_dt / xxx;
            xxx = 0;
        }
    }

    m_state_output_stream->clear();
    m_rate_output_stream->clear();
    m_thrust_output_stream->clear();

    m_accumulator.process([this](
                          size_t,
                          stream::IMulti_Input::Sample const& i_input,
                          stream::IFrame::Sample const& i_frame,
                          stream::IECEF_Position::Sample const& i_position,
                          stream::IECEF_Velocity::Sample const& i_velocity,
                          stream::IECEF_Acceleration::Sample const& i_acceleration
                          //                          stream::IBattery_State::Sample const& i_battery_state,
                          //                          stream::IECEF_Linear_Acceleration::Sample const& i_linear_acceleration,
                          //                          stream::IECEF_Velocity::Sample const& i_velocity,
                          //                          stream::IProximity::Sample const& i_proximity,
                          )
    {
        refresh_state(i_input, i_frame, i_position, i_velocity, i_acceleration);
        acquire_home_position();
        process_state();
    });


    {
        stream::IMulti_State::Value state;
        state.ecef_position = m_state.position.value;
        state.ecef_home_position = m_home.ecef_position;
        state.frame = m_state.frame.value;
        state.last_input = m_state.input.value;

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

    auto output_rate = m_rate_output_stream->get_rate();

    m_config->min_thrust = math::clamp(m_config->min_thrust, 0.0, m_state.config.motor_thrust * m_state.config.motors.size() * 0.5);
    m_config->max_thrust = math::clamp(m_config->max_thrust, m_config->min_thrust, m_state.config.motor_thrust * m_state.config.motors.size());


    m_config->horizontal_angle.max_speed_deg = math::clamp(m_config->horizontal_angle.max_speed_deg, 0.0, 180.0);

    auto fill_pid_params = [output_rate](PID::Params& dst, sz::PID const& src)
    {
        dst.kp = src.kp;
        dst.ki = src.ki;
        dst.kd = src.kd;
        dst.max_i = src.max_i;
        dst.d_filter = src.d_filter;
        dst.rate = output_rate;
    };
    auto fill_pd_params = [output_rate](PID::Params& dst, sz::PD const& src)
    {
        dst.kp = src.kp;
        dst.kd = src.kd;
        dst.d_filter = src.d_filter;
        dst.rate = output_rate;
    };
    auto fill_p_params = [output_rate](PID::Params& dst, sz::P const& src)
    {
        dst.kp = src.kp;
        dst.rate = output_rate;
    };

    {
        PID::Params x_params, y_params;
        if (m_config->horizontal_angle.combined_pids)
        {
            fill_pid_params(x_params, m_config->horizontal_angle.pids);
            fill_pid_params(y_params, m_config->horizontal_angle.pids);
        }
        else
        {
            fill_pid_params(x_params, m_config->horizontal_angle.x_pid);
            fill_pid_params(y_params, m_config->horizontal_angle.y_pid);
        }

        if (!m_horizontal_angle_data.x_pid.set_params(x_params) ||
                !m_horizontal_angle_data.y_pid.set_params(y_params))
        {
            QLOGE("Bad horizontal PID params");
            return false;
        }
    }

    m_config->altitude.max_speed = math::clamp(m_config->altitude.max_speed, 0.0, 10.0);
    {
        PID::Params params;
        fill_pid_params(params, m_config->altitude.acceleration_pid);
        fill_pd_params(params, m_config->altitude.velocity_pd);
        fill_p_params(params, m_config->altitude.altitude_p);
        if (!m_altitude_data.acceleration_pid.set_params(params) ||
            !m_altitude_data.velocity_pd.set_params(params) ||
            !m_altitude_data.altitude_p.set_params(params))
        {
            QLOGE("Bad climb rate PID params");
            return false;
        }
    }

    m_config->altitude.lpf_cutoff_frequency = math::clamp(m_config->altitude.lpf_cutoff_frequency, 0.1, output_rate / 2.0);
    m_config->altitude.lpf_poles = math::max<uint32_t>(m_config->altitude.lpf_poles, 1);
    if (!m_altitude_data.dsp.setup(m_config->altitude.lpf_poles, output_rate, m_config->altitude.lpf_cutoff_frequency))
    {
        QLOGE("Cannot setup dsp filter.");
        return false;
    }
    m_altitude_data.dsp.reset();

    return true;
}
auto Multi_Brain::get_config() const -> rapidjson::Document
{
    rapidjson::Document json;
    autojsoncxx::to_document(*m_config, json);

    if (m_config->horizontal_angle.combined_pids)
    {
        jsonutil::remove_value(json, q::Path("Horizontal Angle/X PID"));
        jsonutil::remove_value(json, q::Path("Horizontal Angle/Y PID"));
    }
    else
    {
        jsonutil::remove_value(json, q::Path("Horizontal Angle/PIDs"));
    }

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
