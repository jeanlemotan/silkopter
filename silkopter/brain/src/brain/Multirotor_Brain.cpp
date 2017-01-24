#include "BrainStdAfx.h"
#include "Multirotor_Brain.h"
#include "physics/constants.h"

#include "hal.def.h"
//#include "sz_PID.hpp"
//#include "sz_Multirotor_Brain.hpp"

namespace silk
{
namespace node
{

Multirotor_Brain::Multirotor_Brain(HAL& hal)
    : m_hal(hal)
    , m_descriptor(new hal::Multirotor_Brain_Descriptor())
    , m_config(new hal::Multirotor_Brain_Config())
{
    m_state_output_stream = std::make_shared<State_Output_Stream>();
    m_rate_output_stream = std::make_shared<Rate_Output_Stream>();
    m_thrust_output_stream = std::make_shared<Thrust_Output_Stream>();
}

ts::Result<void> Multirotor_Brain::init(hal::INode_Descriptor const& descriptor)
{
    QLOG_TOPIC("Multirotor_Brain::init");

    auto specialized = dynamic_cast<hal::Multirotor_Brain_Descriptor const*>(&descriptor);
    if (!specialized)
    {
        return make_error("Wrong descriptor type");
    }
    *m_descriptor = *specialized;

    return init();
}

ts::Result<void> Multirotor_Brain::init()
{
    m_multirotor_properties = m_hal.get_specialized_uav_properties<IMultirotor_Properties>();
    if (!m_multirotor_properties)
    {
        return make_error("No multi properties found");
    }

    auto result = m_battery.init(m_descriptor->get_rate());
    if (result != ts::success)
    {
        return make_error("Cannot initialize the battery: {}", result.error().what());
    }
    m_state_output_stream->set_rate(m_descriptor->get_state_rate());
    m_rate_output_stream->set_rate(m_descriptor->get_rate());
    m_thrust_output_stream->set_rate(m_descriptor->get_rate());

    m_dts = std::chrono::duration<float>(m_thrust_output_stream->get_dt()).count();

    return ts::success;
}

ts::Result<void> Multirotor_Brain::start(q::Clock::time_point tp)
{
    m_state_output_stream->set_tp(tp);
    m_rate_output_stream->set_tp(tp);
    m_thrust_output_stream->set_tp(tp);
    return ts::success;
}

auto Multirotor_Brain::get_inputs() const -> std::vector<Input>
{
    std::vector<Input> inputs =
    {{
         { stream::IMultirotor_Commands::TYPE,      m_descriptor->get_commands_rate(), "commands", m_commands_accumulator.get_stream_path(0) },
         { stream::IFrame::TYPE,                    m_descriptor->get_rate(), "local_frame", m_sensor_accumulator.get_stream_path(0) },
         { stream::IECEF_Position::TYPE,            m_descriptor->get_rate(), "position", m_sensor_accumulator.get_stream_path(1) },
         { stream::IECEF_Velocity::TYPE,            m_descriptor->get_rate(), "velocity", m_sensor_accumulator.get_stream_path(2) },
         { stream::IECEF_Linear_Acceleration::TYPE, m_descriptor->get_rate(), "linear_acceleration", m_sensor_accumulator.get_stream_path(3) },
         { stream::IProximity::TYPE,                m_descriptor->get_rate(), "proximity", m_sensor_accumulator.get_stream_path(4) },
         { stream::IVoltage::TYPE,                  m_descriptor->get_rate(), "voltage", m_sensor_accumulator.get_stream_path(5) },
         { stream::ICurrent::TYPE,                  m_descriptor->get_rate(), "current", m_sensor_accumulator.get_stream_path(6) },
     }};
    return inputs;
}
auto Multirotor_Brain::get_outputs() const -> std::vector<Output>
{
    std::vector<Output> outputs =
    {{
         { "state",        m_state_output_stream },
         { "rate",         m_rate_output_stream },
         { "thrust",       m_thrust_output_stream },
     }};
    return outputs;
}

void Multirotor_Brain::set_mode(Mode mode)
{
    m_mode = mode;

    if (mode == Mode::IDLE)
    {
        QLOGI("Reacquiring Home");
        m_home.is_acquired = false;
        m_home.position_history.clear();
    }
}

void Multirotor_Brain::set_vertical_mode(Vertical_Mode mode)
{
    m_vertical_mode = mode;
}

void Multirotor_Brain::set_horizontal_mode(Horizontal_Mode mode)
{
    m_horizontal_mode = mode;
}

void Multirotor_Brain::set_yaw_mode(Yaw_Mode mode)
{
    m_yaw_mode = mode;
}

ts::Result<void> Multirotor_Brain::check_pre_flight_conditions() const
{
    if (!m_home.is_acquired)
    {
        return make_error("Home is not acquired!");
    }

    auto now = q::Clock::now();

    if (now - m_inputs.commands.last_valid_tp > std::chrono::milliseconds(100))
    {
        return make_error("Command stream is failing!");
    }
    if (now - m_inputs.frame.last_valid_tp > std::chrono::milliseconds(100))
    {
        return make_error("Frame stream is failing!");
    }
    if (now - m_inputs.linear_acceleration.last_valid_tp > std::chrono::milliseconds(100))
    {
        return make_error("Linear acceleration stream is failing!");
    }
    if (now - m_inputs.position.last_valid_tp > std::chrono::milliseconds(100))
    {
        return make_error("Proximity stream is failing!");
    }
    if (now - m_inputs.velocity.last_valid_tp > std::chrono::milliseconds(100))
    {
        return make_error("Velocity stream is failing!");
    }
    if (now - m_inputs.position.last_valid_tp > std::chrono::milliseconds(100))
    {
        return make_error("Position stream is failing!");
    }

    if (math::length(m_enu_velocity) > 1.f)
    {
        return make_error("The UAV seems to be moving!");
    }

    return ts::success;
}

void Multirotor_Brain::process_idle_mode()
{
    acquire_home_position();

    m_enu_position = math::vec3f::zero;
    m_enu_velocity = math::vec3f::zero;

//    stream::IMultirotor_Commands::Value& prev_commands = m_inputs.local_commands.previous_sample.value;
//    stream::IMultirotor_Commands::Value& commands = m_inputs.local_commands.sample.value;
//    QASSERT(commands.mode.get() == Mode::IDLE);

//    if (prev_commands.mode.get() != Mode::IDLE)
//    {
//    }

//    commands.vertical.thrust_rate.set(0);
//    commands.vertical.thrust.set(0);
//    commands.vertical.altitude.set(0);
//    commands.horizontal.angle_rate.set(math::vec2f::zero);
//    commands.horizontal.angle.set(math::vec2f::zero);
//    commands.horizontal.position.set(math::vec2f::zero);
//    commands.yaw.angle_rate.set(0);
//    commands.yaw.angle.set(0);

    m_rate_output_stream->push_sample(stream::IAngular_Velocity::Value(), true);
    m_thrust_output_stream->push_sample(stream::IFloat::Value(), true);

    if (m_inputs.commands.sample.value.mode == Mode::FLY)
    {
        ts::Result<void> result = check_pre_flight_conditions();
        if (result == ts::success)
        {
            set_mode(Mode::FLY);
            return;
        }
        else
        {
            QLOGW("Cannot set FLY mode, preflight checks failed: {}", result.error().what());
        }
    }
}

float Multirotor_Brain::compute_ff_thrust(float target_altitude)
{
    float mass = m_multirotor_properties->get_mass();


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

    float max_speed = math::radians(m_config->get_horizontal().get_max_rate_deg());

    float rx = m_horizontal_mode_data.rate_x_pid.process(-diff_x, 0.f);
    rx = math::clamp(rx, -max_speed, max_speed);
    float ry = m_horizontal_mode_data.rate_y_pid.process(-diff_y, 0.f);
    ry = math::clamp(ry, -max_speed, max_speed);

    return math::vec2f(rx, ry);
}

void Multirotor_Brain::process_return_home_mode()
{
    const stream::IMultirotor_Commands::Value& prev_commands = m_inputs.commands.previous_sample.value;
    stream::IMultirotor_Commands::Value& commands = m_inputs.commands.sample.value;
    QASSERT(m_home.is_acquired);

    set_vertical_mode(Vertical_Mode::ALTITUDE);
    float distance_2d = math::length(math::vec2f(m_enu_position));
    if (distance_2d < 10.f)
    {
        m_vertical_mode_data.target_altitude = 5.f;
    }

    set_horizontal_mode(Horizontal_Mode::POSITION);
    m_horizontal_mode_data.target_enu_position = math::vec2f::zero;
}

void Multirotor_Brain::process_fly_mode()
{
    const stream::IMultirotor_Commands::Value& prev_commands = m_inputs.commands.previous_sample.value;
    stream::IMultirotor_Commands::Value& commands = m_inputs.commands.sample.value;
    QASSERT(m_home.is_acquired);

    auto now = q::Clock::now();
    if (now - m_inputs.commands.last_valid_tp > std::chrono::seconds(2))
    {
        QLOGW("No input received for {}. Heading home", now - m_inputs.commands.last_valid_tp);
        set_mode(Mode::RETURN_HOME);
        return;
    }
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

void Multirotor_Brain::process_flight_modes()
{
    const stream::IMultirotor_Commands::Value& prev_commands = m_inputs.commands.previous_sample.value;
    stream::IMultirotor_Commands::Value& commands = m_inputs.commands.sample.value;

    m_enu_position = math::vec3f(math::transform(m_home.ecef_to_enu_transform, m_inputs.position.sample.value));
    m_enu_velocity = math::vec3f(math::rotate(m_home.ecef_to_enu_transform, math::vec3d(m_inputs.velocity.sample.value)));

    if (m_mode == Mode::FLY)
    {
        process_fly_mode();
    }
    else if (m_mode == Mode::RETURN_HOME)
    {
        process_return_home_mode();
    }

    QASSERT(m_mode == Mode::FLY ||
            m_mode == Mode::RETURN_HOME ||
            m_mode == Mode::LAND ||
            m_mode == Mode::TAKE_OFF);

    QASSERT(m_home.is_acquired);

//    if (!m_home.is_acquired)
//    {
//        QLOGW("Trying to arm but Home is not acquired yet. Ignoring request");
//        commands.mode.set(Mode::IDLE);
//        return;
//    }

    stream::IFloat::Value thrust = m_thrust_output_stream->get_last_sample().value;
    stream::IAngular_Velocity::Value rate = m_rate_output_stream->get_last_sample().value;

    //////////////////////////////////////////////////////////////
    // Verticals

    if (m_vertical_mode == stream::IMultirotor_Commands::Vertical_Mode::THRUST)
    {
//        if (prev_commands.vertical_mode != commands.vertical.mode)
//        {
//            commands.vertical.thrust.set(thrust);
//            QLOGI("Vertical mode changed to THRUST. Initializing thrust to {}N", thrust);
//        }

        thrust = commands.sticks.throttle * m_config->get_max_thrust();
    }
    else if (m_vertical_mode == stream::IMultirotor_Commands::Vertical_Mode::ALTITUDE)
    {
//        if (prev_commands.vertical_mode != commands.vertical_mode)
//        {
//            commands.vertical.altitude.set(m_enu_position.z);
//            QLOGI("Vertical mode changed to ALTITUDE. Initializing altitude to {}m", m_enu_position.z);
//        }

        {
            float output = compute_ff_thrust(commands.sticks.throttle * m_config->get_vertical().get_max_speed() * 2.f);
            thrust = output;
        }

        {
            float target_alt = m_vertical_mode_data.target_altitude;
            float crt_alt = m_enu_position.z;

            //cascaded PIDS: position P(ID) -> speed PI(D)
            float speed_output = m_vertical_mode_data.altitude_p.process(crt_alt, target_alt);
            speed_output = math::clamp(speed_output, -m_config->get_vertical().get_max_speed(), m_config->get_vertical().get_max_speed());

            float output = m_vertical_mode_data.speed_pi.process(m_enu_velocity.z, speed_output);


//            float output = m_vertical_altitude_data.pid.process_ex(crt_alt, target_alt, m_enu_velocity.z);
            output = math::clamp(output, -1.f, 1.f);
            m_vertical_mode_data.altitude_dsp.process(output);

            float hover_thrust = m_multirotor_properties->get_mass() * physics::constants::g;
            float max_thrust_range = math::max(hover_thrust, m_config->get_max_thrust() - hover_thrust);

            thrust = output * max_thrust_range + hover_thrust;
        }
    }

    //clamp thrust
    thrust = math::clamp(thrust, m_config->get_min_thrust(), m_config->get_max_thrust());

    ////////////////////////////////////////////////////////////
    // Horizontals

    if (m_horizontal_mode == stream::IMultirotor_Commands::Horizontal_Mode::ANGLE_RATE)
    {
        float max_speed = math::radians(m_config->get_horizontal().get_max_rate_deg());
        rate.x = commands.sticks.pitch * max_speed * 2.f;
        rate.y = commands.sticks.roll * max_speed * 2.f;
    }
    else if (m_horizontal_mode == stream::IMultirotor_Commands::Horizontal_Mode::ANGLE)
    {
        math::vec2f const& max_angle = math::radians(m_config->get_horizontal().get_max_angle_deg());
        math::vec2f hrate = compute_horizontal_rate_for_angle(math::vec2f(commands.sticks.pitch * max_angle.x * 2.f, commands.sticks.roll * max_angle.y * 2.f));
        rate.x = hrate.x;
        rate.y = hrate.y;
    }
    else if (m_horizontal_mode == stream::IMultirotor_Commands::Horizontal_Mode::POSITION)
    {
//        if (prev_commands.horizontal_mode != commands.horizontal_mode)
//        {
//            commands.horizontal.position.set(math::vec2f(m_enu_position));
//            QLOGI("Horizontal mode changed to POSITION. Initializing position to {}", math::vec2f(m_enu_position));
//        }

        {
            math::vec2f target_pos = m_horizontal_mode_data.target_enu_position;
            math::vec2f crt_pos(m_enu_position);

            //cascaded PIDS: position P(ID) -> speed PI(D)
            math::vec2f velocity_output = m_horizontal_mode_data.position_p.process(crt_pos, target_pos);
            if (math::length(velocity_output) > m_config->get_horizontal().get_max_speed())
            {
                velocity_output.set_length(m_config->get_horizontal().get_max_speed());
            }

            math::vec2f output = m_horizontal_mode_data.velocity_pi.process(math::vec2f(m_enu_velocity.x, m_enu_velocity.y), velocity_output);

            output = math::clamp(output, -math::vec2f(math::radians(20.f)), math::vec2f(math::radians(20.f)));
            m_horizontal_mode_data.position_dsp.process(output);

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

    if (m_yaw_mode == stream::IMultirotor_Commands::Yaw_Mode::ANGLE_RATE)
    {
        float max_speed = math::radians(m_config->get_yaw().get_max_rate_deg());
        rate.z = commands.sticks.yaw * max_speed * 2.f;
    }
    else if (m_yaw_mode == stream::IMultirotor_Commands::Yaw_Mode::ANGLE)
    {
        float fx, fy, fz;
        m_inputs.frame.sample.value.get_as_euler_zxy(fx, fy, fz);

//        if (prev_commands.yaw_mode != commands.yaw_mode)
//        {
//            commands.yaw.angle.set(fz);
//            QLOGI("Yaw mode changed to ANGLE. Initializing angle to {}m", fz);
//        }

        math::quatf target;
//        target.set_from_euler_zxy(fx, fy, commands.yaw.angle);
        math::quatf diff = math::inverse(m_inputs.frame.sample.value) * target;
        float _, diff_z;
        diff.get_as_euler_zxy(_, _, diff_z);

        float max_speed = math::radians(m_config->get_yaw().get_max_rate_deg());

        float z = m_yaw_mode_data.rate_pid.process(-diff_z, 0.f);
        z = math::clamp(z, -max_speed, max_speed);

        rate.z = z;
    }

    ///////////////////////////////////////////////////////////

    m_rate_output_stream->push_sample(rate, true);
    m_thrust_output_stream->push_sample(thrust, true);
}


void Multirotor_Brain::process_mode()
{
    if (m_mode == Mode::IDLE)
    {
        process_idle_mode();
    }
    else
    {
        process_flight_modes();
    }



//    //increment version of overriden commands
//    //Increment_Version func;
//    //stream::IMultirotor_Commands::apply(func, m_inputs.local_commands.previous_sample.value, m_inputs.local_commands.sample.value);

//    //back up the current commands
//    m_inputs.local_commands.previous_sample = m_inputs.local_commands.sample;
}

void Multirotor_Brain::acquire_home_position()
{
    QASSERT(m_mode == Mode::IDLE);

    if (m_home.is_acquired && math::distance(m_home.position, m_inputs.position.sample.value) > 10.f)
    {
        m_home.is_acquired = false;
    }

    bool is_stable = true;

    std::deque<util::coordinates::ECEF>& history = m_home.position_history;

    //check if the position is stable
    for (size_t i = 1; i < history.size(); i++)
    {
        if (math::distance(history[0], history[1]) > m_config->get_max_allowed_position_variation())
        {
            if (m_home.is_acquired)
            {
                QLOGI("Home un-acquired. Too instable!!!");
                m_home.is_acquired = false;
            }

            is_stable = false;
            break;
        }
    }

    //clamp the history size
    size_t per_second = static_cast<size_t>(1.f / m_dts);
    while (history.size() > 5 * per_second + 1)
    {
        if (is_stable && !m_home.is_acquired)
        {
            QLOGI("Home acquired!!!");
            m_home.is_acquired = true;
        }

        history.pop_front();
    }

    //compute the average
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

void Multirotor_Brain::refresh_inputs(stream::IFrame::Sample const& frame,
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

    m_state_output_stream->clear();
    m_rate_output_stream->clear();
    m_thrust_output_stream->clear();

    m_commands_accumulator.process([this](stream::IMultirotor_Commands::Sample const& i_commands)
    {
        m_inputs.commands.sample = i_commands;
        m_inputs.commands.last_valid_tp = i_commands.is_healthy ? q::Clock::now() : m_inputs.commands.last_valid_tp;
    });

//    Merge_Commands func;
//    stream::IMultirotor_Commands::apply(func, m_inputs.remote_commands.previous_sample.value, m_inputs.remote_commands.sample.value, m_inputs.local_commands.sample.value);
//    m_inputs.remote_commands.previous_sample.value = m_inputs.remote_commands.sample.value;

    m_sensor_accumulator.process([this](stream::IFrame::Sample const& i_frame,
                                      stream::IECEF_Position::Sample const& i_position,
                                      stream::IECEF_Velocity::Sample const& i_velocity,
                                      stream::IECEF_Linear_Acceleration::Sample const& i_linear_acceleration,
                                      stream::IProximity::Sample const& i_proximity,
                                      stream::IVoltage::Sample const& i_voltage,
                                      stream::ICurrent::Sample const& i_current)
    {
        m_battery_state_sample = m_battery.process(i_voltage, i_current);

        refresh_inputs(i_frame, i_position, i_velocity, i_linear_acceleration, i_proximity);
        process_mode();
    });

    size_t samples_needed = m_state_output_stream->compute_samples_needed();
    if (samples_needed > 0)
    {
        stream::IMultirotor_State::Value state;
        state.ecef_position = m_inputs.position.sample.value;
        state.enu_velocity = m_enu_velocity;
        state.battery_state.average_current = m_battery_state_sample.value.average_current;
        state.battery_state.average_voltage = m_battery_state_sample.value.average_voltage;
        state.battery_state.capacity_left = m_battery_state_sample.value.capacity_left;
        state.battery_state.charge_used = m_battery_state_sample.value.charge_used;
        state.local_frame = m_inputs.frame.sample.value;
        state.mode = m_mode;

        for (size_t i = 0; i < samples_needed; i++)
        {
            m_state_output_stream->push_sample(state, true);
        }
    }
}

ts::Result<void> Multirotor_Brain::set_input_stream_path(size_t idx, std::string const& path)
{
    if (idx == 0)
    {
        return m_commands_accumulator.set_stream_path(0, path, m_descriptor->get_commands_rate(), m_hal);
    }
    else if (idx >= 1)
    {
        return m_sensor_accumulator.set_stream_path(idx - 1, path, m_descriptor->get_rate(), m_hal);
    }
    return ts::success;
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

ts::Result<void> Multirotor_Brain::set_config(hal::INode_Config const& config)
{
    QLOG_TOPIC("Multirotor_Brain::set_config");

    auto specialized = dynamic_cast<hal::Multirotor_Brain_Config const*>(&config);
    if (!specialized)
    {
        return make_error("Wrong config type");
    }
    *m_config = *specialized;

    uint32_t output_rate = m_rate_output_stream->get_rate();

    m_config->set_min_thrust(math::clamp(m_config->get_min_thrust(), 0.f, m_multirotor_properties->get_motor_thrust() * m_multirotor_properties->get_motors().size() * 0.5f));
    m_config->set_max_thrust(math::clamp(m_config->get_max_thrust(), m_config->get_min_thrust(), m_multirotor_properties->get_motor_thrust() * m_multirotor_properties->get_motors().size()));

    {
        hal::Multirotor_Brain_Config::Horizontal const& descriptor = m_config->get_horizontal();
        PID::Params rate_x_params, rate_y_params;
        if (auto combined_pids = boost::get<hal::Multirotor_Brain_Config::Horizontal::Combined_Rate_PIDs>(&descriptor.get_rate_pids()))
        {
            fill_pid_params(rate_x_params, *combined_pids, output_rate);
            fill_pid_params(rate_y_params, *combined_pids, output_rate);
        }
        else
        {
            auto separate_pids = boost::get<hal::Multirotor_Brain_Config::Horizontal::Separate_Rate_PIDs>(&descriptor.get_rate_pids());
            fill_pid_params(rate_x_params, separate_pids->get_x_pid(), output_rate);
            fill_pid_params(rate_y_params, separate_pids->get_y_pid(), output_rate);
        }

        if (!m_horizontal_mode_data.rate_x_pid.set_params(rate_x_params) ||
            !m_horizontal_mode_data.rate_y_pid.set_params(rate_y_params))
        {
            return make_error("Bad horizontal rate PID params");
        }
    }

    {
        PID::Params params;
        fill_pid_params(params, m_config->get_yaw().get_rate_pid(), output_rate);
        if (!m_yaw_mode_data.rate_pid.set_params(params))
        {
            return make_error("Bad yaw rate PID params");
        }
    }

    //altitude
    {
        PID::Params speed_pi_params, altitude_p_params;
        fill_pi_params(speed_pi_params, m_config->get_vertical().get_speed_pid(), output_rate);
        fill_p_params(altitude_p_params, m_config->get_vertical().get_altitude_pid(), output_rate);
        if (!m_vertical_mode_data.speed_pi.set_params(speed_pi_params))
        {
            return make_error("Bad vertical speed PID params");
        }
        if (!m_vertical_mode_data.altitude_p.set_params(altitude_p_params))
        {
            return make_error("Bad vertical altitude PID params");
        }
    }

    {
        hal::LPF_Config& lpf_config = m_config->get_vertical().get_altitude_lpf();
        lpf_config.set_cutoff_frequency(math::clamp(lpf_config.get_cutoff_frequency(), 0.1f, output_rate / 2.f));
        if (!m_vertical_mode_data.altitude_dsp.setup(lpf_config.get_poles(), output_rate, lpf_config.get_cutoff_frequency()))
        {
            return make_error("Cannot setup vertical altitude dsp filter.");
        }
        m_vertical_mode_data.altitude_dsp.reset();
    }

    //horizontal
    {
        PID2::Params velocity_pi_params, position_p_params;
        fill_pi_params(velocity_pi_params, m_config->get_horizontal().get_velocity_pid(), output_rate);
        fill_p_params(position_p_params, m_config->get_horizontal().get_position_pid(), output_rate);
        if (!m_horizontal_mode_data.velocity_pi.set_params(velocity_pi_params))
        {
            return make_error("Bad horizontal velocity PID params");
        }
        if (!m_horizontal_mode_data.position_p.set_params(position_p_params))
        {
            return make_error("Bad horizontal position PID params");
        }
    }

    {
        hal::LPF_Config& lpf_config = m_config->get_horizontal().get_position_lpf();
        lpf_config.set_cutoff_frequency(math::clamp(lpf_config.get_cutoff_frequency(), 0.1f, output_rate / 2.f));
        if (!m_horizontal_mode_data.position_dsp.setup(lpf_config.get_poles(), output_rate, lpf_config.get_cutoff_frequency()))
        {
            return make_error("Cannot setup position dsp filter.");
        }
        m_horizontal_mode_data.position_dsp.reset();
    }

    return ts::success;
}
auto Multirotor_Brain::get_config() const -> std::shared_ptr<const hal::INode_Config>
{
    return m_config;
}

auto Multirotor_Brain::get_descriptor() const -> std::shared_ptr<const hal::INode_Descriptor>
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
