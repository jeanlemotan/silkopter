#include "BrainStdAfx.h"
#include "Multi_Brain.h"
#include "physics/constants.h"

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
    QLOG_TOPIC("Multi_Brain::init");

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
    boost::optional<config::Multi> multi_config = m_hal.get_multi_config();
    if (!multi_config)
    {
        QLOGE("No multi config found");
        return false;
    }

    if (m_init_params->rate == 0)
    {
        QLOGE("Bad rate: {}Hz", m_init_params->rate);
        return false;
    }
    m_state_output_stream->set_rate(m_init_params->state_rate);
    m_rate_output_stream->set_rate(m_init_params->rate);
    m_thrust_output_stream->set_rate(m_init_params->rate);

    m_dts = std::chrono::duration<float>(m_thrust_output_stream->get_dt()).count();

    m_config->altitude.lpf_cutoff_frequency = 1;

    return true;
}

auto Multi_Brain::start(q::Clock::time_point tp) -> bool
{
    m_state_output_stream->set_tp(tp);
    m_rate_output_stream->set_tp(tp);
    m_thrust_output_stream->set_tp(tp);
    return true;
}

auto Multi_Brain::get_inputs() const -> std::vector<Input>
{
    std::vector<Input> inputs =
    {{
         { stream::IMulti_Commands::TYPE,    m_init_params->commands_rate, "Commands", m_commands_accumulator.get_stream_path(0) },
         { stream::IFrame::TYPE,             m_init_params->rate, "Frame", m_sensor_accumulator.get_stream_path(0) },
         { stream::IECEF_Position::TYPE,     m_init_params->rate, "Position (ecef)", m_sensor_accumulator.get_stream_path(1) },
         { stream::IECEF_Velocity::TYPE,     m_init_params->rate, "Velocity (ecef)", m_sensor_accumulator.get_stream_path(2) },
         { stream::IECEF_Linear_Acceleration::TYPE, m_init_params->rate, "Linear Acceleration (ecef)", m_sensor_accumulator.get_stream_path(3) },
         { stream::IProximity::TYPE,         m_init_params->rate, "Proximity", m_sensor_accumulator.get_stream_path(4) },
         //        { stream::IBattery_State::TYPE,     m_init_params->rate, "Battery State", m_accumulator.get_stream_path(2) },
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
    stream::IMulti_Commands::Sample::Value& commands = m_inputs.commands.sample.value;
    QASSERT(commands.mode.value == stream::IMulti_Commands::Mode::IDLE);

    if (m_inputs.commands.previous_sample.value.mode.value != stream::IMulti_Commands::Mode::IDLE)
    {
        QLOGI("Reacquiring Home");
        m_home.is_acquired = false;
        m_home.position_history.clear();
    }

    if (commands.vertical.thrust_rate.value != 0)
    {
        commands.vertical.thrust_rate.set(0);
    }
    if (commands.vertical.thrust.value != 0)
    {
        commands.vertical.thrust.set(0);
    }
    if (commands.vertical.altitude.value != 0)
    {
        commands.vertical.altitude.set(0);
    }

    if (!math::is_zero(commands.horizontal.angle_rate.value))
    {
        commands.horizontal.angle_rate.set(math::vec2f::zero);
    }
    if (!math::is_zero(commands.horizontal.angle.value))
    {
        commands.horizontal.angle.set(math::vec2f::zero);
    }
    if (!math::is_zero(commands.horizontal.position.value))
    {
        commands.horizontal.position.set(math::vec2f::zero);
    }
    if (commands.yaw.angle_rate.value != 0)
    {
        commands.yaw.angle_rate.set(0);
    }
    if (commands.yaw.angle.value != 0)
    {
        commands.yaw.angle.set(0);
    }

    m_rate_output_stream->push_sample(stream::IAngular_Velocity::Value(), true);
    m_thrust_output_stream->push_sample(stream::IFloat::Value(), true);
}

float Multi_Brain::compute_ff_thrust(float target_altitude)
{
    boost::optional<config::Multi> multi_config = m_hal.get_multi_config();
    QASSERT(multi_config);
    float mass = multi_config->mass;


    float v0 = m_enu_velocity.z;
    float d = target_altitude - m_enu_position.z;

    float a0 = 0; //acceleration phase
    float a1 = 0; //brake phase

    if (d > 0)
    {
        a0 = m_config->max_thrust / mass - physics::constants::g;
        a1 = m_config->min_thrust / mass - physics::constants::g;
    }
    else
    {
        a1 = m_config->max_thrust / mass - physics::constants::g;
        a0 = m_config->min_thrust / mass - physics::constants::g;
    }

    if (math::sgn(d) != math::sgn(v0))
    {
        float thrust = (a0 + physics::constants::g) * mass;
        return thrust;
    }

    float stopping_d = (v0*v0) / (2.f*math::abs(a1)); //<---- correct formula
    //float stopping_d = 2.f * (v0*v0) / (2.f*math::abs(a1)); //<---- multiplied by 2. For some reason this works very well. Needs investigation
    if (stopping_d < math::abs(d))
    {
        float thrust = (a0 + physics::constants::g) * mass;
        return thrust;
    }
    else
    {
        float thrust = (a1 + physics::constants::g) * mass;
        return thrust;
    }
}

void Multi_Brain::process_state_mode_armed()
{
    boost::optional<config::Multi> multi_config = m_hal.get_multi_config();
    QASSERT(multi_config);

    stream::IMulti_Commands::Sample::Value& prev_commands = m_inputs.commands.previous_sample.value;
    stream::IMulti_Commands::Sample::Value& commands = m_inputs.commands.sample.value;
    QASSERT(commands.mode.value == stream::IMulti_Commands::Mode::ARMED);

    if (!m_home.is_acquired)
    {
        QLOGW("Trying to arm but Home is not acquired yet. Ignoring request");
        commands.mode.set(stream::IMulti_Commands::Mode::IDLE);
        return;
    }

    stream::IFloat::Value thrust = m_thrust_output_stream->get_last_sample().value;
    stream::IAngular_Velocity::Value rate = m_rate_output_stream->get_last_sample().value;

    //////////////////////////////////////////////////////////////
    // Verticals

    if (commands.vertical.mode.value == stream::IMulti_Commands::Vertical::Mode::THRUST_RATE)
    {
        thrust += commands.vertical.thrust_rate.value * m_dts;
    }
    else if (commands.vertical.mode.value == stream::IMulti_Commands::Vertical::Mode::THRUST)
    {
        if (prev_commands.vertical.mode.value != commands.vertical.mode.value)
        {
            commands.vertical.thrust.set(thrust);
            QLOGI("Vertical mode changed to THRUST. Initializing thrust to {}N", thrust);
        }

        thrust = commands.vertical.thrust.value;
    }
    else if (commands.vertical.mode.value == stream::IMulti_Commands::Vertical::Mode::ALTITUDE)
    {
        if (prev_commands.vertical.mode.value != commands.vertical.mode.value)
        {
            commands.vertical.altitude.set(m_enu_position.z);
            QLOGI("Vertical mode changed to ALTITUDE. Initializing altitude to {}m", m_enu_position.z);
        }

        {
            //float output = compute_ff_thrust(commands.vertical.altitude.value);
            //thrust = output;
        }

        {
            float target_alt = commands.vertical.altitude.value;
            float crt_alt = m_enu_position.z;

            //cascaded PIDS: position P(ID) -> speed PI(D)
            float velocity_output = m_vertical_altitude_data.position_p.process(crt_alt, target_alt);
            float output = m_vertical_altitude_data.speed_pi.process(m_enu_velocity.z, velocity_output);


//            float output = m_vertical_altitude_data.pid.process_ex(crt_alt, target_alt, m_enu_velocity.z);
            output = math::clamp(output, -1.f, 1.f);
            m_vertical_altitude_data.dsp.process(output);

            float hover_thrust = multi_config->mass * physics::constants::g;
            float max_thrust_range = math::max(hover_thrust, m_config->max_thrust - hover_thrust);

            thrust = output * max_thrust_range + hover_thrust;
        }
    }

    //clamp thrust
    thrust = math::clamp(thrust, m_config->min_thrust, m_config->max_thrust);

    ////////////////////////////////////////////////////////////
    // Horizontals

    if (commands.horizontal.mode.value == stream::IMulti_Commands::Horizontal::Mode::ANGLE_RATE)
    {
        rate.x = commands.horizontal.angle_rate.value.x;
        rate.y = commands.horizontal.angle_rate.value.y;
    }
    else if (commands.horizontal.mode.value == stream::IMulti_Commands::Horizontal::Mode::ANGLE)
    {
        float fx, fy, fz;
        m_inputs.frame.sample.value.get_as_euler_zxy(fx, fy, fz);

        math::quatf target;
        target.set_from_euler_zxy(commands.horizontal.angle.value.x, commands.horizontal.angle.value.y, fz);
        math::quatf diff = math::inverse(m_inputs.frame.sample.value) * target;
        float diff_x, diff_y, _;
        diff.get_as_euler_zxy(diff_x, diff_y, _);

        float max_speed = math::radians(m_config->horizontal_angle.max_speed_deg);

        float x = m_horizontal_angle_data.x_pid.process(-diff_x, 0.f);
        x = math::clamp(x, -max_speed, max_speed);
        float y = m_horizontal_angle_data.y_pid.process(-diff_y, 0.f);
        y = math::clamp(y, -max_speed, max_speed);

        rate.x = x;
        rate.y = y;
    }

    ///////////////////////////////////////////////////////////
    // Yaw

    if (commands.yaw.mode.value == stream::IMulti_Commands::Yaw::Mode::ANGLE_RATE)
    {
        rate.z = commands.yaw.angle_rate.value;
    }
    else if (commands.yaw.mode.value == stream::IMulti_Commands::Yaw::Mode::ANGLE)
    {
        float fx, fy, fz;
        m_inputs.frame.sample.value.get_as_euler_zxy(fx, fy, fz);

        if (prev_commands.yaw.mode.value != commands.yaw.mode.value)
        {
            commands.yaw.angle.set(fz);
            QLOGI("Yaw mode changed to ANGLE. Initializing angle to {}m", fz);
        }

        math::quatf target;
        target.set_from_euler_zxy(fx, fy, commands.yaw.angle.value);
        math::quatf diff = math::inverse(m_inputs.frame.sample.value) * target;
        float _, diff_z;
        diff.get_as_euler_zxy(_, _, diff_z);

        float max_speed = math::radians(m_config->yaw_angle.max_speed_deg);

        float z = m_yaw_stable_angle_rate_data.pid.process(-diff_z, 0.f);
        z = math::clamp(z, -max_speed, max_speed);

        rate.z = z;
    }

    ///////////////////////////////////////////////////////////

    m_rate_output_stream->push_sample(rate, true);
    m_thrust_output_stream->push_sample(thrust, true);
}

struct Increment_Version
{
    template<class T>
    bool operator()(T const& prev, T& crt)
    {
        if (crt.value != prev.value)
        {
            crt.version++;
        }
        return true;
    }
};

void Multi_Brain::process_state()
{
    if (m_inputs.commands.sample.value.mode.value == stream::IMulti_Commands::Mode::IDLE)
    {
        m_enu_position = math::vec3f::zero;
        m_enu_velocity = math::vec3f::zero;

        process_state_mode_idle();
    }
    else if (m_inputs.commands.sample.value.mode.value == stream::IMulti_Commands::Mode::ARMED)
    {
        m_enu_position = math::vec3f(math::transform(m_home.ecef_to_enu_transform, m_inputs.position.sample.value));
        m_enu_velocity = math::vec3f(math::rotate(m_home.ecef_to_enu_transform, math::vec3d(m_inputs.velocity.sample.value)));

        process_state_mode_armed();
    }

    //increment version of overriden commands
    Increment_Version func;
    stream::IMulti_Commands::apply(func, m_inputs.commands.previous_sample.value, m_inputs.commands.sample.value);
}

void Multi_Brain::acquire_home_position()
{
    if (m_inputs.commands.sample.value.mode.value == stream::IMulti_Commands::Mode::IDLE)
    {
        std::deque<util::coordinates::ECEF>& history = m_home.position_history;
        size_t per_second = static_cast<size_t>(1.f / m_dts);
        while (history.size() > 5 * per_second + 1)
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
        for (util::coordinates::ECEF const& h: history)
        {
            avg += h * mul;
        }
        //auto avg_ = std::accumulate(history.begin(), history.end(), util::coordinates::ECEF(0));
        //avg_ /= double(history.size());

        m_home.position = avg;
        util::coordinates::LLA lla_position = util::coordinates::ecef_to_lla(m_home.position);
        util::coordinates::enu_to_ecef_transform_and_inv(lla_position, m_home.enu_to_ecef_transform, m_home.ecef_to_enu_transform);
        util::coordinates::enu_to_ecef_rotation_and_inv(lla_position, m_home.enu_to_ecef_rotation, m_home.ecef_to_enu_rotation);
    }
}

void Multi_Brain::refresh_inputs(stream::IFrame::Sample const& frame,
                                stream::IECEF_Position::Sample const& position,
                                stream::IECEF_Velocity::Sample const& velocity,
                                stream::IECEF_Linear_Acceleration::Sample const& linear_acceleration,
                                stream::IProximity::Sample const& proximity)
{
    auto now = q::Clock::now();

    m_inputs.frame.previous_sample = m_inputs.frame.sample;
    m_inputs.frame.sample = frame;
    m_inputs.frame.last_valid_tp = frame.is_healthy ? now : m_inputs.frame.last_valid_tp;

    m_home.position_history.push_back(position.value);
    if (position.is_healthy)
    {
        m_inputs.position.previous_sample = m_inputs.position.sample;
        m_inputs.position.sample = position;
        m_inputs.position.last_valid_tp = now;
    }

    m_inputs.velocity.previous_sample = m_inputs.velocity.sample;
    m_inputs.velocity.sample = velocity;
    m_inputs.velocity.last_valid_tp = velocity.is_healthy ? now : m_inputs.velocity.last_valid_tp;

    m_inputs.linear_acceleration.previous_sample = m_inputs.linear_acceleration.sample;
    m_inputs.linear_acceleration.sample = linear_acceleration;
    m_inputs.linear_acceleration.last_valid_tp = linear_acceleration.is_healthy ? now : m_inputs.linear_acceleration.last_valid_tp;

    m_inputs.proximity.previous_sample = m_inputs.proximity.sample;
    m_inputs.proximity.sample = proximity;
    m_inputs.proximity.last_valid_tp = proximity.is_healthy ? now : m_inputs.proximity.last_valid_tp;
}

void Multi_Brain::process()
{
    QLOG_TOPIC("Multi_Brain::process");

    {
        static q::Clock::time_point last_timestamp = q::Clock::now();
        auto now = q::Clock::now();
        auto dt = now - last_timestamp;
        last_timestamp = now;
        static q::Clock::duration min_dt, max_dt, avg_dt;
        static int xxx = 0;
        min_dt = std::min(min_dt, dt);
        max_dt = std::max(max_dt, dt);
        avg_dt += dt;
        xxx++;
        static q::Clock::time_point xxx_timestamp = q::Clock::now();
        if (now - xxx_timestamp >= std::chrono::milliseconds(1000))
        {
            xxx_timestamp = now;

            QLOGI("min {}, max {}, avg {}", min_dt, max_dt, avg_dt/ xxx);
            min_dt = dt;
            max_dt = dt;
            avg_dt = std::chrono::milliseconds(0);

            xxx = 0;
        }
    }

    m_state_output_stream->clear();
    m_rate_output_stream->clear();
    m_thrust_output_stream->clear();

    stream::IMulti_Commands::Assignment_Functor func;
    m_commands_accumulator.process([this, &func](stream::IMulti_Commands::Sample const& i_commands)
    {
        m_inputs.commands.previous_sample = m_inputs.commands.sample;

        m_inputs.commands.sample.is_healthy = i_commands.is_healthy;
        stream::IMulti_Commands::apply(func, m_inputs.commands.sample.value, i_commands.value);

        m_inputs.commands.last_valid_tp = i_commands.is_healthy ? q::Clock::now() : m_inputs.commands.last_valid_tp;
    });

    m_sensor_accumulator.process([this](stream::IFrame::Sample const& i_frame,
                                      stream::IECEF_Position::Sample const& i_position,
                                      stream::IECEF_Velocity::Sample const& i_velocity,
                                      stream::IECEF_Linear_Acceleration::Sample const& i_linear_acceleration,
                                      stream::IProximity::Sample const& i_proximity)
    {
        refresh_inputs(i_frame, i_position, i_velocity, i_linear_acceleration, i_proximity);
        process_state();
    });

    acquire_home_position();

    size_t samples_needed = m_state_output_stream->compute_samples_needed();
    if (samples_needed > 0)
    {
        stream::IMulti_State::Value state;
        state.time_point = q::Clock::now();
        state.position = m_inputs.position.sample;
        state.velocity = m_inputs.velocity.sample;
        state.home_position.value = m_home.position;
        state.home_position.is_healthy = m_home.is_acquired;
        state.frame = m_inputs.frame.sample;

        state.commands = m_inputs.commands.sample.value;

        state.proximity = m_inputs.proximity.sample;

        for (size_t i = 0; i < samples_needed; i++)
        {
            m_state_output_stream->push_sample(state, true);
        }
    }
}

void Multi_Brain::set_input_stream_path(size_t idx, q::Path const& path)
{
//    { stream::IMulti_Commands::TYPE,       m_init_params->input_rate, "Input", m_input_accumulator.get_stream_path(0) },
//    { stream::IFrame::TYPE,             m_init_params->rate, "Frame", m_sensor_accumulator.get_stream_path(0) },
//    { stream::IECEF_Position::TYPE,     m_init_params->rate, "Position (ecef)", m_sensor_accumulator.get_stream_path(1) },
//    { stream::IECEF_Velocity::TYPE,     m_init_params->rate, "Velocity (ecef)", m_sensor_accumulator.get_stream_path(2) },
//    { stream::IECEF_Linear_Acceleration::TYPE, m_init_params->rate, "Linear Acceleration (ecef)", m_sensor_accumulator.get_stream_path(3) },
//    { stream::IProximity::TYPE,         m_init_params->rate, "Proximity", m_sensor_accumulator.get_stream_path(4) },

    if (idx == 0)
    {
        m_commands_accumulator.set_stream_path(0, path, m_init_params->commands_rate, m_hal);
    }
    else if (idx >= 1 && idx <= 5)
    {
        m_sensor_accumulator.set_stream_path(idx - 1, path, m_init_params->rate, m_hal);
    }
}

auto Multi_Brain::set_config(rapidjson::Value const& json) -> bool
{
    QLOG_TOPIC("Multi_Brain::set_config");

    boost::optional<config::Multi> multi_config = m_hal.get_multi_config();
    QASSERT(multi_config);

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

    uint32_t output_rate = m_rate_output_stream->get_rate();

    m_config->min_thrust = math::clamp(m_config->min_thrust, 0.f, multi_config->motor_thrust * multi_config->motors.size() * 0.5f);
    m_config->max_thrust = math::clamp(m_config->max_thrust, m_config->min_thrust, multi_config->motor_thrust * multi_config->motors.size());

    m_config->horizontal_angle.max_speed_deg = math::clamp(m_config->horizontal_angle.max_speed_deg, 10.f, 3000.f);
    m_config->yaw_angle.max_speed_deg = math::clamp(m_config->yaw_angle.max_speed_deg, 10.f, 3000.f);

    auto fill_pid_params = [output_rate](PID::Params& dst, sz::PID const& src)
    {
        dst.kp = src.kp;
        dst.ki = src.ki;
        dst.kd = src.kd;
        dst.max_i = src.max_i;
        dst.d_filter = src.d_filter;
        dst.rate = output_rate;
    };
    auto fill_pi_params = [output_rate](PID::Params& dst, sz::PI const& src)
    {
        dst.kp = src.kp;
        dst.ki = src.ki;
        dst.max_i = src.max_i;
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

    {
        PID::Params params;
        fill_pid_params(params, m_config->yaw_angle.pid);
        if (!m_yaw_stable_angle_rate_data.pid.set_params(params))
        {
            QLOGE("Bad yaw PID params");
            return false;
        }
    }

    m_config->altitude.max_speed = math::clamp(m_config->altitude.max_speed, 0.f, 10.f);
    {
        PID::Params speed_pi_params, position_p_params;
        fill_pi_params(speed_pi_params, m_config->altitude.speed_pi);
        fill_p_params(position_p_params, m_config->altitude.position_p);
        if (!m_vertical_altitude_data.speed_pi.set_params(speed_pi_params))
        {
            QLOGE("Bad altitude PID params");
            return false;
        }
        if (!m_vertical_altitude_data.position_p.set_params(position_p_params))
        {
            QLOGE("Bad altitude PID params");
            return false;
        }
    }

    m_config->altitude.lpf_cutoff_frequency = math::clamp(m_config->altitude.lpf_cutoff_frequency, 0.1f, output_rate / 2.f);
    m_config->altitude.lpf_poles = math::max<uint32_t>(m_config->altitude.lpf_poles, 1);
    if (!m_vertical_altitude_data.dsp.setup(m_config->altitude.lpf_poles, output_rate, m_config->altitude.lpf_cutoff_frequency))
    {
        QLOGE("Cannot setup dsp filter.");
        return false;
    }
    m_vertical_altitude_data.dsp.reset();

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
