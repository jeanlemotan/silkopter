#include "BrainStdAfx.h"
#include "Multirotor_Brain.h"
#include "physics/constants.h"

#include "uav.def.h"
//#include "sz_PID.hpp"
//#include "sz_Multirotor_Brain.hpp"

namespace silk
{
namespace node
{

Multirotor_Brain::Multirotor_Brain(UAV& uav)
    : m_uav(uav)
    , m_descriptor(new uav::Multirotor_Brain_Descriptor())
    , m_config(new uav::Multirotor_Brain_Config())
{
    m_state_output_stream = std::make_shared<State_Output_Stream>();
    m_rate_output_stream = std::make_shared<Rate_Output_Stream>();
    m_thrust_output_stream = std::make_shared<Thrust_Output_Stream>();
}

auto Multirotor_Brain::init(uav::INode_Descriptor const& descriptor) -> bool
{
    QLOG_TOPIC("Multirotor_Brain::init");

    auto specialized = dynamic_cast<uav::Multirotor_Brain_Descriptor const*>(&descriptor);
    if (!specialized)
    {
        QLOGE("Wrong descriptor type");
        return false;
    }
    *m_descriptor = *specialized;

    return init();
}

auto Multirotor_Brain::init() -> bool
{
    m_multirotor_descriptor = m_uav.get_specialized_uav_descriptor<uav::Multirotor_Descriptor>();
    if (!m_multirotor_descriptor)
    {
        QLOGE("No multi descriptor found");
        return false;
    }

    if (!m_battery.init(m_descriptor->get_rate()))
    {
        QLOGE("Cannot initialize the battery");
        return false;
    }
    m_state_output_stream->set_rate(m_descriptor->get_state_rate());
    m_rate_output_stream->set_rate(m_descriptor->get_rate());
    m_thrust_output_stream->set_rate(m_descriptor->get_rate());

    m_dts = std::chrono::duration<float>(m_thrust_output_stream->get_dt()).count();

    return true;
}

auto Multirotor_Brain::start(q::Clock::time_point tp) -> bool
{
    m_state_output_stream->set_tp(tp);
    m_rate_output_stream->set_tp(tp);
    m_thrust_output_stream->set_tp(tp);
    return true;
}

auto Multirotor_Brain::get_inputs() const -> std::vector<Input>
{
    std::vector<Input> inputs =
    {{
         { stream::IMultirotor_Commands::TYPE,    m_descriptor->get_commands_rate(), "Commands", m_commands_accumulator.get_stream_path(0) },
         { stream::IUAV_Frame::TYPE,         m_descriptor->get_rate(), "UAV Frame", m_sensor_accumulator.get_stream_path(0) },
         { stream::IECEF_Position::TYPE,     m_descriptor->get_rate(), "Position (ecef)", m_sensor_accumulator.get_stream_path(1) },
         { stream::IECEF_Velocity::TYPE,     m_descriptor->get_rate(), "Velocity (ecef)", m_sensor_accumulator.get_stream_path(2) },
         { stream::IECEF_Linear_Acceleration::TYPE, m_descriptor->get_rate(), "Linear Acceleration (ecef)", m_sensor_accumulator.get_stream_path(3) },
         { stream::IProximity::TYPE,         m_descriptor->get_rate(), "Proximity", m_sensor_accumulator.get_stream_path(4) },
         { stream::IVoltage::TYPE,           m_descriptor->get_rate(), "Voltage", m_sensor_accumulator.get_stream_path(5) },
         { stream::ICurrent::TYPE,           m_descriptor->get_rate(), "Current", m_sensor_accumulator.get_stream_path(6) },
     }};
    return inputs;
}
auto Multirotor_Brain::get_outputs() const -> std::vector<Output>
{
    std::vector<Output> outputs =
    {{
         { "State",        m_state_output_stream },
         { "Rate",         m_rate_output_stream },
         { "Thrust",       m_thrust_output_stream },
     }};
    return outputs;
}

void Multirotor_Brain::process_state_mode_idle()
{
    stream::IMultirotor_Commands::Value& prev_commands = m_inputs.local_commands.previous_sample.value;
    stream::IMultirotor_Commands::Value& commands = m_inputs.local_commands.sample.value;
    QASSERT(commands.mode.get() == stream::IMultirotor_Commands::Mode::IDLE);

    if (prev_commands.mode.get() != stream::IMultirotor_Commands::Mode::IDLE)
    {
        QLOGI("Reacquiring Home");
        m_home.is_acquired = false;
        m_home.position_history.clear();
    }

    commands.vertical.thrust_rate.set(0);
    commands.vertical.thrust.set(0);
    commands.vertical.altitude.set(0);
    commands.horizontal.angle_rate.set(math::vec2f::zero);
    commands.horizontal.angle.set(math::vec2f::zero);
    commands.horizontal.position.set(math::vec2f::zero);
    commands.yaw.angle_rate.set(0);
    commands.yaw.angle.set(0);

    m_rate_output_stream->push_sample(stream::IAngular_Velocity::Value(), true);
    m_thrust_output_stream->push_sample(stream::IFloat::Value(), true);
}

float Multirotor_Brain::compute_ff_thrust(float target_altitude)
{
    float mass = m_multirotor_descriptor->get_mass();


    float v0 = m_enu_velocity.z;
    float d = target_altitude - m_enu_position.z;

    float a0 = 0; //acceleration phase
    float a1 = 0; //brake phase

    if (d > 0)
    {
        a0 = m_config->get_max_thrust() / mass - physics::constants::g;
        a1 = m_config->get_min_thrust() / mass - physics::constants::g;
    }
    else
    {
        a1 = m_config->get_max_thrust() / mass - physics::constants::g;
        a0 = m_config->get_min_thrust() / mass - physics::constants::g;
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

math::vec2f Multirotor_Brain::compute_horizontal_rate_for_angle(math::vec2f const& angle)
{
    float fx, fy, fz;
    m_inputs.frame.sample.value.get_as_euler_zxy(fx, fy, fz);

    math::quatf target;
    target.set_from_euler_zxy(angle.x, angle.y, fz);
    math::quatf diff = math::inverse(m_inputs.frame.sample.value) * target;
    float diff_x, diff_y, _;
    diff.get_as_euler_zxy(diff_x, diff_y, _);

    float max_speed = math::radians(m_config->get_horizontal_angle().get_max_speed_deg());

    float rx = m_horizontal_angle_data.x_pid.process(-diff_x, 0.f);
    rx = math::clamp(rx, -max_speed, max_speed);
    float ry = m_horizontal_angle_data.y_pid.process(-diff_y, 0.f);
    ry = math::clamp(ry, -max_speed, max_speed);

    return math::vec2f(rx, ry);
}

void Multirotor_Brain::state_mode_armed_apply_commands(const stream::IMultirotor_Commands::Value& prev_commands, stream::IMultirotor_Commands::Value& commands)
{
    std::shared_ptr<const uav::Multirotor_Descriptor> multirotor_descriptor = m_uav.get_specialized_uav_descriptor<uav::Multirotor_Descriptor>();

    QASSERT(commands.mode.get() == stream::IMultirotor_Commands::Mode::ARMED);

    if (!m_home.is_acquired)
    {
        QLOGW("Trying to arm but Home is not acquired yet. Ignoring request");
        commands.mode.set(stream::IMultirotor_Commands::Mode::IDLE);
        return;
    }

    stream::IFloat::Value thrust = m_thrust_output_stream->get_last_sample().value;
    stream::IAngular_Velocity::Value rate = m_rate_output_stream->get_last_sample().value;

    //////////////////////////////////////////////////////////////
    // Verticals

    if (commands.vertical.mode.get() == stream::IMultirotor_Commands::Vertical::Mode::THRUST_RATE)
    {
        thrust += commands.vertical.thrust_rate.get() * m_dts;
    }
    else if (commands.vertical.mode.get() == stream::IMultirotor_Commands::Vertical::Mode::THRUST)
    {
        if (prev_commands.vertical.mode.get() != commands.vertical.mode.get())
        {
            commands.vertical.thrust.set(thrust);
            QLOGI("Vertical mode changed to THRUST. Initializing thrust to {}N", thrust);
        }

        thrust = commands.vertical.thrust.get();
    }
    else if (commands.vertical.mode.get() == stream::IMultirotor_Commands::Vertical::Mode::ALTITUDE)
    {
        if (prev_commands.vertical.mode.get() != commands.vertical.mode.get())
        {
            commands.vertical.altitude.set(m_enu_position.z);
            QLOGI("Vertical mode changed to ALTITUDE. Initializing altitude to {}m", m_enu_position.z);
        }

        {
            //float output = compute_ff_thrust(commands.vertical.altitude.get());
            //thrust = output;
        }

        {
            float target_alt = commands.vertical.altitude.get();
            float crt_alt = m_enu_position.z;

            //cascaded PIDS: position P(ID) -> speed PI(D)
            float speed_output = m_vertical_altitude_data.position_p.process(crt_alt, target_alt);
            speed_output = math::clamp(speed_output, -m_config->get_altitude().get_max_speed(), m_config->get_altitude().get_max_speed());

            float output = m_vertical_altitude_data.speed_pi.process(m_enu_velocity.z, speed_output);


//            float output = m_vertical_altitude_data.pid.process_ex(crt_alt, target_alt, m_enu_velocity.z);
            output = math::clamp(output, -1.f, 1.f);
            m_vertical_altitude_data.dsp.process(output);

            float hover_thrust = multirotor_descriptor->get_mass() * physics::constants::g;
            float max_thrust_range = math::max(hover_thrust, m_config->get_max_thrust() - hover_thrust);

            thrust = output * max_thrust_range + hover_thrust;
        }
    }

    //clamp thrust
    thrust = math::clamp(thrust, m_config->get_min_thrust(), m_config->get_max_thrust());

    ////////////////////////////////////////////////////////////
    // Horizontals

    if (commands.horizontal.mode.get() == stream::IMultirotor_Commands::Horizontal::Mode::ANGLE_RATE)
    {
        rate.x = commands.horizontal.angle_rate.get().x;
        rate.y = commands.horizontal.angle_rate.get().y;
    }
    else if (commands.horizontal.mode.get() == stream::IMultirotor_Commands::Horizontal::Mode::ANGLE)
    {
        math::vec2f hrate = compute_horizontal_rate_for_angle(commands.horizontal.angle.get());
        rate.x = hrate.x;
        rate.y = hrate.y;
    }
    else if (commands.horizontal.mode.get() == stream::IMultirotor_Commands::Horizontal::Mode::POSITION)
    {
        if (prev_commands.horizontal.mode.get() != commands.horizontal.mode.get())
        {
            commands.horizontal.position.set(math::vec2f(m_enu_position));
            QLOGI("Horizontal mode changed to POSITION. Initializing position to {}", math::vec2f(m_enu_position));
        }

        {
            math::vec2f target_pos = commands.horizontal.position.get();
            math::vec2f crt_pos(m_enu_position);

            //cascaded PIDS: position P(ID) -> speed PI(D)
            math::vec2f velocity_output = m_horizontal_position_data.position_p.process(crt_pos, target_pos);
            if (math::length(velocity_output) > m_config->get_horizontal_position().get_max_speed())
            {
                velocity_output.set_length(m_config->get_horizontal_position().get_max_speed());
            }

            math::vec2f output = m_horizontal_position_data.velocity_pi.process(math::vec2f(m_enu_velocity.x, m_enu_velocity.y), velocity_output);

            output = math::clamp(output, -math::vec2f(math::radians(20.f)), math::vec2f(math::radians(20.f)));
            m_horizontal_position_data.dsp.process(output);

            //compute the front/right in enu space
            math::vec3f front_vector = math::rotate(m_inputs.frame.sample.value, physics::constants::uav_front_vector);
            math::vec3f right_vector = math::rotate(m_inputs.frame.sample.value, physics::constants::uav_right_vector);

            //figure out how the delta displacement maps over the axis
            float dx = math::dot(math::vec3f(output, 0.f), right_vector);
            float dy = math::dot(math::vec3f(output, 0.f), front_vector);

            //movement along X axis is obtained by rotation along the Y
            //movement along Y axis is obtained by rotation along the -X
            math::vec2f angle = math::vec2f(-dy, dx);

            math::vec2f hrate = compute_horizontal_rate_for_angle(angle);
            rate.x = hrate.x;
            rate.y = hrate.y;
        }
    }

    ///////////////////////////////////////////////////////////
    // Yaw

    if (commands.yaw.mode.get() == stream::IMultirotor_Commands::Yaw::Mode::ANGLE_RATE)
    {
        rate.z = commands.yaw.angle_rate.get();
    }
    else if (commands.yaw.mode.get() == stream::IMultirotor_Commands::Yaw::Mode::ANGLE)
    {
        float fx, fy, fz;
        m_inputs.frame.sample.value.get_as_euler_zxy(fx, fy, fz);

        if (prev_commands.yaw.mode.get() != commands.yaw.mode.get())
        {
            commands.yaw.angle.set(fz);
            QLOGI("Yaw mode changed to ANGLE. Initializing angle to {}m", fz);
        }

        math::quatf target;
        target.set_from_euler_zxy(fx, fy, commands.yaw.angle.get());
        math::quatf diff = math::inverse(m_inputs.frame.sample.value) * target;
        float _, diff_z;
        diff.get_as_euler_zxy(_, _, diff_z);

        float max_speed = math::radians(m_config->get_yaw_angle().get_max_speed_deg());

        float z = m_yaw_stable_angle_rate_data.pid.process(-diff_z, 0.f);
        z = math::clamp(z, -max_speed, max_speed);

        rate.z = z;
    }

    ///////////////////////////////////////////////////////////

    m_rate_output_stream->push_sample(rate, true);
    m_thrust_output_stream->push_sample(thrust, true);
}

void Multirotor_Brain::process_return_home_toggle(const stream::IMultirotor_Commands::Value& prev_commands, stream::IMultirotor_Commands::Value& commands)
{
    commands.vertical.mode.set(stream::IMultirotor_Commands::Vertical::Mode::ALTITUDE);
    float distance_2d = math::length(math::vec2f(m_enu_position));
    if (distance_2d < 10.f)
    {
        commands.vertical.altitude.set(5.f);
    }

    commands.horizontal.mode.set(stream::IMultirotor_Commands::Horizontal::Mode::POSITION);
    commands.horizontal.position.set(math::vec2f::zero);
}

void Multirotor_Brain::process_state_mode_armed()
{
    const stream::IMultirotor_Commands::Value& prev_commands = m_inputs.local_commands.previous_sample.value;
    stream::IMultirotor_Commands::Value& commands = m_inputs.local_commands.sample.value;
    if (!m_home.is_acquired)
    {
        QLOGW("Trying to arm but Home is not acquired yet. Ignoring request");
        commands.mode.set(stream::IMultirotor_Commands::Mode::IDLE);
        return;
    }

    auto now = q::Clock::now();
    if (commands.toggles.return_home.get() == false && now - m_inputs.remote_commands.last_valid_tp > std::chrono::seconds(2))
    {
        QLOGW("No input received for {}. Heading home", now - m_inputs.remote_commands.last_valid_tp);
        commands.toggles.return_home.set(true);
    }

    if (commands.toggles.return_home.get())
    {
        process_return_home_toggle(prev_commands, commands);
    }

    state_mode_armed_apply_commands(prev_commands, commands);
}

//struct Increment_Version
//{
//    template<class T>
//    bool operator()(const char* name, T const& prev, T& crt)
//    {
//        if (crt.get() != prev.get())
//        {
//            crt.version++;
//        }
//        return true;
//    }
//};
struct Merge_Commands
{
    template<class T>
    bool operator()(const char* name, T const& remote_prev, T const& remote_crt, T& local)
    {
        if (remote_crt.version != remote_prev.version)
        {
            local.set(remote_crt.get());
        }
        return true;
    }
};


void Multirotor_Brain::process_state()
{
    if (m_inputs.local_commands.sample.value.mode.get() == stream::IMultirotor_Commands::Mode::IDLE)
    {
        m_enu_position = math::vec3f::zero;
        m_enu_velocity = math::vec3f::zero;

        process_state_mode_idle();
    }
    else if (m_inputs.local_commands.sample.value.mode.get() == stream::IMultirotor_Commands::Mode::ARMED)
    {
        m_enu_position = math::vec3f(math::transform(m_home.ecef_to_enu_transform, m_inputs.position.sample.value));
        m_enu_velocity = math::vec3f(math::rotate(m_home.ecef_to_enu_transform, math::vec3d(m_inputs.velocity.sample.value)));

        process_state_mode_armed();
    }



    //increment version of overriden commands
    //Increment_Version func;
    //stream::IMultirotor_Commands::apply(func, m_inputs.local_commands.previous_sample.value, m_inputs.local_commands.sample.value);

    //back up the current commands
    m_inputs.local_commands.previous_sample = m_inputs.local_commands.sample;
}

void Multirotor_Brain::acquire_home_position()
{
    if (m_inputs.local_commands.sample.value.mode.get() == stream::IMultirotor_Commands::Mode::IDLE)
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

void Multirotor_Brain::refresh_inputs(stream::IUAV_Frame::Sample const& frame,
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

void Multirotor_Brain::process()
{
    QLOG_TOPIC("Multirotor_Brain::process");

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

    m_commands_accumulator.process([this](stream::IMultirotor_Commands::Sample const& i_commands)
    {
        m_inputs.remote_commands.sample = i_commands;
        m_inputs.remote_commands.last_valid_tp = i_commands.is_healthy ? q::Clock::now() : m_inputs.remote_commands.last_valid_tp;
    });

    Merge_Commands func;
    stream::IMultirotor_Commands::apply(func, m_inputs.remote_commands.previous_sample.value, m_inputs.remote_commands.sample.value, m_inputs.local_commands.sample.value);
    m_inputs.remote_commands.previous_sample.value = m_inputs.remote_commands.sample.value;

    m_sensor_accumulator.process([this](stream::IUAV_Frame::Sample const& i_frame,
                                      stream::IECEF_Position::Sample const& i_position,
                                      stream::IECEF_Velocity::Sample const& i_velocity,
                                      stream::IECEF_Linear_Acceleration::Sample const& i_linear_acceleration,
                                      stream::IProximity::Sample const& i_proximity,
                                      stream::IVoltage::Sample const& i_voltage,
                                      stream::ICurrent::Sample const& i_current)
    {
        m_battery_state_sample = m_battery.process(i_voltage, i_current);

        refresh_inputs(i_frame, i_position, i_velocity, i_linear_acceleration, i_proximity);
        process_state();
    });

    acquire_home_position();

    size_t samples_needed = m_state_output_stream->compute_samples_needed();
    if (samples_needed > 0)
    {
        stream::IMultirotor_State::Value state;
        state.time_point = q::Clock::now();
        state.position = m_inputs.position.sample;
        state.velocity = m_inputs.velocity.sample;
        state.home_position.value = m_home.position;
        state.home_position.is_healthy = m_home.is_acquired;
        state.frame = m_inputs.frame.sample;
        state.battery_state = m_battery_state_sample;

        state.commands = m_inputs.local_commands.sample.value;

        state.proximity = m_inputs.proximity.sample;

        for (size_t i = 0; i < samples_needed; i++)
        {
            m_state_output_stream->push_sample(state, true);
        }
    }
}

void Multirotor_Brain::set_input_stream_path(size_t idx, q::Path const& path)
{
    if (idx == 0)
    {
        m_commands_accumulator.set_stream_path(0, path, m_descriptor->get_commands_rate(), m_uav);
    }
    else if (idx >= 1 && idx <= 7)
    {
        m_sensor_accumulator.set_stream_path(idx - 1, path, m_descriptor->get_rate(), m_uav);
    }
}

template<class T, class PID>
void fill_pid_params(T& dst, PID const& src, size_t rate)
{
    dst.kp = src.get_kp();
    dst.ki = src.get_ki();
    dst.kd = src.get_kd();
    dst.max_i = src.get_max_i();
    dst.d_filter = src.get_d_filter();
    dst.rate = rate;
}
template<class T, class PI>
void fill_pi_params(T& dst, PI const& src, size_t rate)
{
    dst.kp = src.get_kp();
    dst.ki = src.get_ki();
    dst.max_i = decltype(dst.max_i)(src.get_max_i());
    dst.rate = rate;
}
template<class T, class P>
void fill_p_params(T& dst, P const& src, size_t rate)
{
    dst.kp = src.get_kp();
    dst.rate = rate;
}

auto Multirotor_Brain::set_config(uav::INode_Config const& config) -> bool
{
    QLOG_TOPIC("Multirotor_Brain::set_config");

    auto specialized = dynamic_cast<uav::Multirotor_Brain_Config const*>(&config);
    if (!specialized)
    {
        QLOGE("Wrong config type");
        return false;
    }
    *m_config = *specialized;

    uint32_t output_rate = m_rate_output_stream->get_rate();

    m_config->set_min_thrust(math::clamp(m_config->get_min_thrust(), 0.f, m_multirotor_descriptor->get_motor_thrust() * m_multirotor_descriptor->get_motors().size() * 0.5f));
    m_config->set_max_thrust(math::clamp(m_config->get_max_thrust(), m_config->get_min_thrust(), m_multirotor_descriptor->get_motor_thrust() * m_multirotor_descriptor->get_motors().size()));

    {
        uav::Multirotor_Brain_Config::Horizontal_Angle const& descriptor = m_config->get_horizontal_angle();
        PID::Params x_params, y_params;
        if (auto combined_pids = boost::get<uav::Multirotor_Brain_Config::Horizontal_Angle::Combined_XY_PIDs>(&descriptor.get_xy_pids()))
        {
            fill_pid_params(x_params, *combined_pids, output_rate);
            fill_pid_params(y_params, *combined_pids, output_rate);
        }
        else
        {
            auto separate_pids = boost::get<uav::Multirotor_Brain_Config::Horizontal_Angle::Separate_XY_PIDs>(&descriptor.get_xy_pids());
            fill_pid_params(x_params, separate_pids->get_x_pid(), output_rate);
            fill_pid_params(y_params, separate_pids->get_y_pid(), output_rate);
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
        fill_pid_params(params, m_config->get_yaw_angle().get_pid(), output_rate);
        if (!m_yaw_stable_angle_rate_data.pid.set_params(params))
        {
            QLOGE("Bad yaw PID params");
            return false;
        }
    }

    //altitude
    {
        PID::Params speed_pi_params, position_p_params;
        fill_pi_params(speed_pi_params, m_config->get_altitude().get_speed_pi(), output_rate);
        fill_p_params(position_p_params, m_config->get_altitude().get_position_p(), output_rate);
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

    {
        uav::LPF_Config& lpf_config = m_config->get_altitude().get_lpf();
        lpf_config.set_cutoff_frequency(math::clamp(lpf_config.get_cutoff_frequency(), 0.1f, output_rate / 2.f));
        if (!m_vertical_altitude_data.dsp.setup(lpf_config.get_poles(), output_rate, lpf_config.get_cutoff_frequency()))
        {
            QLOGE("Cannot setup dsp filter.");
            return false;
        }
        m_vertical_altitude_data.dsp.reset();
    }

    //horizontal position
    {
        PID2::Params velocity_pi_params, position_p_params;
        fill_pi_params(velocity_pi_params, m_config->get_horizontal_position().get_velocity_pi(), output_rate);
        fill_p_params(position_p_params, m_config->get_horizontal_position().get_position_p(), output_rate);
        if (!m_horizontal_position_data.velocity_pi.set_params(velocity_pi_params))
        {
            QLOGE("Bad horizontal position PID params");
            return false;
        }
        if (!m_horizontal_position_data.position_p.set_params(position_p_params))
        {
            QLOGE("Bad horizontal position PID params");
            return false;
        }
    }

    {
        uav::LPF_Config& lpf_config = m_config->get_horizontal_position().get_lpf();
        lpf_config.set_cutoff_frequency(math::clamp(lpf_config.get_cutoff_frequency(), 0.1f, output_rate / 2.f));
        if (!m_horizontal_position_data.dsp.setup(lpf_config.get_poles(), output_rate, lpf_config.get_cutoff_frequency()))
        {
            QLOGE("Cannot setup dsp filter.");
            return false;
        }
        m_horizontal_position_data.dsp.reset();
    }

    return true;
}
auto Multirotor_Brain::get_config() const -> std::shared_ptr<const uav::INode_Config>
{
    return m_config;
}

auto Multirotor_Brain::get_descriptor() const -> std::shared_ptr<const uav::INode_Descriptor>
{
    return m_descriptor;
}

//auto Multirotor_Brain::send_message(rapidjson::Value const& json) -> rapidjson::Document
//{
//    rapidjson::Document response;

//    auto* messagej = jsonutil::find_value(json, std::string("message"));
//    if (!messagej && messagej->IsString())
//    {
//        jsonutil::add_value(response, std::string("error"), rapidjson::Value("Message not found"), response.GetAllocator());
//        return std::move(response);
//    }

//    std::string message = messagej->GetString();
//    if (message == "reset battery")
//    {
//        m_battery.reset();
//    }
//    else if (message == "battery capacity")
//    {
//        auto* capacityj = jsonutil::find_value(json, std::string("capacity"));
//        if (!capacityj || capacityj->IsNumber())
//        {
//            jsonutil::add_value(response, std::string("error"), rapidjson::Value("Capacity not found"), response.GetAllocator());
//            return std::move(response);
//        }

//        LiPo_Battery::Config config;
//        config.full_charge = capacityj->GetDouble();
//        m_battery.set_config(config);
//    }
//    else
//    {
//        jsonutil::add_value(response, std::string("error"), rapidjson::Value("Unknown command"), response.GetAllocator());
//    }

//    return std::move(response);
//}

}
}
