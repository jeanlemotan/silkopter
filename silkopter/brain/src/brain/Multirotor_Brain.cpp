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

//////////////////////////////////////////////////////////////////////////////////////////

Multirotor_Brain::Multirotor_Brain(HAL& hal)
    : m_hal(hal)
    , m_descriptor(new hal::Multirotor_Brain_Descriptor())
    , m_config(new hal::Multirotor_Brain_Config())
{
    m_state_output_stream = std::make_shared<State_Output_Stream>();
    m_rate_output_stream = std::make_shared<Rate_Output_Stream>();
    m_thrust_output_stream = std::make_shared<Thrust_Output_Stream>();
}

//////////////////////////////////////////////////////////////////////////////////////////

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

//////////////////////////////////////////////////////////////////////////////////////////

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

//////////////////////////////////////////////////////////////////////////////////////////

ts::Result<void> Multirotor_Brain::start(Clock::time_point tp)
{
    m_state_output_stream->set_tp(tp);
    m_rate_output_stream->set_tp(tp);
    m_thrust_output_stream->set_tp(tp);
    return ts::success;
}

//////////////////////////////////////////////////////////////////////////////////////////

std::vector<Multirotor_Brain::Input> Multirotor_Brain::get_inputs() const
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

//////////////////////////////////////////////////////////////////////////////////////////

std::vector<Multirotor_Brain::Output> Multirotor_Brain::get_outputs() const
{
    std::vector<Output> outputs =
    {{
         { "state",        m_state_output_stream },
         { "rate",         m_rate_output_stream },
         { "thrust",       m_thrust_output_stream },
     }};
    return outputs;
}

//////////////////////////////////////////////////////////////////////////////////////////

void Multirotor_Brain::set_mode(Mode mode)
{
    m_mode = mode;

    if (mode == Mode::IDLE)
    {
        QLOGI("Reacquiring Home");
        m_home.is_acquired = false;
        m_home.position_history.clear();
    }
    else if (mode == Mode::FLY)
    {
        m_fly_mode_data.state = Fly_Mode_Data::State::NORMAL;

        //in case the uav was going somewhere, reset the targets to the current position
        //this happens when switching form RETURN HOME to FLY
        m_vertical_mode_data.target_altitude = m_enu_position.z;
        m_horizontal_mode_data.target_enu_position = math::vec2f(m_enu_position);
    }
    else if (mode == Mode::RETURN_HOME)
    {
        set_vertical_mode(Vertical_Mode::ALTITUDE);
        set_horizontal_mode(Horizontal_Mode::POSITION);

        //make sure we're not too close to the ground
        //TODO - add a config param for the RTH altitude
        m_vertical_mode_data.target_altitude = math::max(m_vertical_mode_data.target_altitude, 5.f);

        //head straight home
        m_horizontal_mode_data.target_enu_position = math::vec2f::zero;
    }
}

//////////////////////////////////////////////////////////////////////////////////////////

void Multirotor_Brain::set_vertical_mode(Vertical_Mode mode)
{
    if (m_vertical_mode == mode)
    {
        return;
    }
    m_vertical_mode = mode;

    if (mode == Vertical_Mode::ALTITUDE)
    {
        //when switching to altitude hold, keep the current altitude
        m_vertical_mode_data.target_altitude = m_enu_position.z;
        QLOGI("Vertical mode changed to ALTITUDE. Initializing altitude to {}m", m_vertical_mode_data.target_altitude);
    }
}

//////////////////////////////////////////////////////////////////////////////////////////

void Multirotor_Brain::set_horizontal_mode(Horizontal_Mode mode)
{
    if (m_horizontal_mode == mode)
    {
        return;
    }
    m_horizontal_mode = mode;

    if (mode == Horizontal_Mode::POSITION)
    {
        //when switching to position hold, keep the current position
        m_horizontal_mode_data.target_enu_position = math::vec2f(m_enu_position);
        QLOGI("Horizontal mode changed to POSITION. Initializing position to {}", m_horizontal_mode_data.target_enu_position);
    }
}

//////////////////////////////////////////////////////////////////////////////////////////

void Multirotor_Brain::set_yaw_mode(Yaw_Mode mode)
{
    if (m_yaw_mode == mode)
    {
        return;
    }
    m_yaw_mode = mode;

    if (mode == Yaw_Mode::ANGLE)
    {
        //when switching to angle hold, keep the current angle as the reference
        float _, fz;
        m_inputs.frame.sample.value.get_as_euler_zxy(_, _, fz);

        m_yaw_mode_data.target_angle = fz;
        QLOGI("Yaw mode changed to ANGLE. Initializing angle to {}", m_yaw_mode_data.target_angle);
    }
}

//////////////////////////////////////////////////////////////////////////////////////////

ts::Result<void> Multirotor_Brain::check_pre_flight_conditions() const
{
    if (!m_home.is_acquired)
    {
        return make_error("Home is not acquired!");
    }

    //check that all sensors are functional
    auto now = Clock::now();

    if (now - m_last_invalid_commands_tp < std::chrono::seconds(5))
    {
        return make_error("Command stream failed recently!");
    }
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

//////////////////////////////////////////////////////////////////////////////////////////

void Multirotor_Brain::process_idle_mode()
{
    acquire_home_position();

    //we're home
    m_enu_position = math::vec3f::zero;
    m_enu_velocity = math::vec3f::zero;

    //output nothing
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

//////////////////////////////////////////////////////////////////////////////////////////

float Multirotor_Brain::compute_ff_thrust(float target_altitude) const
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

//////////////////////////////////////////////////////////////////////////////////////////

math::vec2f Multirotor_Brain::compute_horizontal_rate_for_angle(math::vec2f const& target_angle)
{
    //current yaw. We'll use it to compine with the new target_angles to get a new quaternion
    float _, fz;
    m_inputs.frame.sample.value.get_as_euler_zxy(_, _, fz);

    //target attitude
    math::quatf target;
    target.set_from_euler_zxy(target_angle.x, target_angle.y, fz);

    //compute the quat difference. This has no issues with singularities or gimbal locks
    math::quatf diff = math::inverse(m_inputs.frame.sample.value) * target;

    //get the difference as angles. We ignore the yaw (Z rotation) here
    float diff_x, diff_y;
    diff.get_as_euler_zxy(diff_x, diff_y, _);

    //run it throught the PID to get rate. The target diff is 0, 0 - the UAV has to match the target angles
    float rx = m_horizontal_mode_data.angle_x_pid.process(-diff_x, 0.f);
    float ry = m_horizontal_mode_data.angle_y_pid.process(-diff_y, 0.f);

    //clamp the rate to our maximums
    float max_speed = math::radians(m_config->get_horizontal().get_max_rate_deg());
    rx = math::clamp(rx, -max_speed, max_speed);
    ry = math::clamp(ry, -max_speed, max_speed);

    return math::vec2f(rx, ry);
}

//////////////////////////////////////////////////////////////////////////////////////////

math::vec2f Multirotor_Brain::compute_horizontal_rate_for_position(math::vec2f const& target_pos)
{
    math::vec2f crt_pos(m_enu_position);
    math::vec2f crt_vel(m_enu_velocity);

    // cascaded PIDS: position P(ID) -> speed PI(D) !!!

    //first run the crt & target pos through the PID to get the velocity
    math::vec2f velocity_output = m_horizontal_mode_data.position_p.process(crt_pos, target_pos);

    //clamp the velocity to the max allowed
    if (math::length(velocity_output) > m_config->get_horizontal().get_max_speed())
    {
        velocity_output.set_length(m_config->get_horizontal().get_max_speed());
    }

    //now run the crt and target velocity through the PID to get the desired angles that will make them match
    math::vec2f output = m_horizontal_mode_data.velocity_pi.process(crt_vel, velocity_output);

    //clamp and filter the angles
    //TODO - add a config param for this clamping
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

    return compute_horizontal_rate_for_angle(angle);
}

//////////////////////////////////////////////////////////////////////////////////////////

float Multirotor_Brain::compute_thrust_for_altitude(float target_alt)
{
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

    return output * max_thrust_range + hover_thrust;
}

//////////////////////////////////////////////////////////////////////////////////////////

float Multirotor_Brain::compute_yaw_rate_for_angle(float target_angle)
{
    float fx, fy, fz;
    m_inputs.frame.sample.value.get_as_euler_zxy(fx, fy, fz);

    math::quatf target;
    target.set_from_euler_zxy(fx, fy, target_angle);
    math::quatf diff = math::inverse(m_inputs.frame.sample.value) * target;
    float _, diff_z;
    diff.get_as_euler_zxy(_, _, diff_z);

    float z = m_yaw_mode_data.angle_pid.process(-diff_z, 0.f);

    float max_speed = math::radians(m_config->get_yaw().get_max_rate_deg());
    z = math::clamp(z, -max_speed, max_speed);

    return z;
}

//////////////////////////////////////////////////////////////////////////////////////////

void Multirotor_Brain::process_return_home_mode()
{
    stream::IMultirotor_Commands::Value const& commands = m_inputs.commands.sample.value;
    QASSERT(m_home.is_acquired);

    auto now = Clock::now();

    if (now - m_last_invalid_commands_tp > std::chrono::seconds(5))
    {
        if (commands.mode == Mode::FLY ||
            commands.mode == Mode::LAND)
        {
            set_mode(commands.mode);
            return;
        }
    }

    float distance_2d = math::length(math::vec2f(m_enu_position));
    if (distance_2d < 10.f)
    {
        m_vertical_mode_data.target_altitude = 5.f;
    }

    m_horizontal_mode_data.target_enu_position = math::vec2f::zero;

    stream::IFloat::Value thrust = m_thrust_output_stream->get_last_sample().value;
    stream::IAngular_Velocity::Value rate = m_rate_output_stream->get_last_sample().value;

    //////////////////////////////////////////////////////////////
    // Verticals

    //apply command
    thrust = compute_thrust_for_altitude(m_vertical_mode_data.target_altitude);

    //clamp thrust
    thrust = math::clamp(thrust, m_config->get_min_thrust(), m_config->get_max_thrust());

    ////////////////////////////////////////////////////////////
    // Horizontals

    //apply command
    rate = compute_horizontal_rate_for_position(m_horizontal_mode_data.target_enu_position);

    ///////////////////////////////////////////////////////////

    m_rate_output_stream->push_sample(rate, true);
    m_thrust_output_stream->push_sample(thrust, true);
}

//////////////////////////////////////////////////////////////////////////////////////////

void Multirotor_Brain::process_fly_mode()
{
    stream::IMultirotor_Commands::Value commands = m_inputs.commands.sample.value;
    QASSERT(m_home.is_acquired);

    auto now = Clock::now();

    if (m_fly_mode_data.state == Fly_Mode_Data::State::NORMAL)
    {
        //check signal loss condition
        //TODO - add a config param for this
        if (now - m_inputs.commands.last_valid_tp > std::chrono::seconds(2))
        {
            m_fly_mode_data.state = Fly_Mode_Data::State::ALERT_HOLD;
            QLOGW("No input received for {}. Holding position", now - m_inputs.commands.last_valid_tp);

            //store the old modes in case we get the signal back and we have to revert back to normal
            m_fly_mode_data.old_vertical_mode = m_vertical_mode;
            m_fly_mode_data.old_horizontal_mode = m_horizontal_mode;
            m_fly_mode_data.old_yaw_mode = m_yaw_mode;

            //set hold mode
            set_vertical_mode(Vertical_Mode::ALTITUDE);
            set_horizontal_mode(Horizontal_Mode::POSITION);
            set_yaw_mode(Yaw_Mode::ANGLE);
        }
        else
        {
            //we have solid signal, check mode transitions
            if (commands.mode != Mode::FLY)
            {
                set_mode(commands.mode);
                return;
            }
            set_vertical_mode(commands.vertical_mode);
            set_horizontal_mode(commands.horizontal_mode);
            set_yaw_mode(commands.yaw_mode);
        }
    }
    if (m_fly_mode_data.state == Fly_Mode_Data::State::ALERT_HOLD)
    {
        //still no signal? switch to RETURN_HOME
        //TODO - add a config param for this
        if (now - m_inputs.commands.last_valid_tp > std::chrono::seconds(20))
        {
            QLOGW("No input received for {}. Returning home", now - m_inputs.commands.last_valid_tp);
            set_mode(Mode::RETURN_HOME);
            return;
        }
        //no glitch lately? go back to normal
        //TODO - add a config param for this
        else if (now - m_last_invalid_commands_tp > std::chrono::milliseconds(500))
        {
            m_fly_mode_data.state = Fly_Mode_Data::State::NORMAL;
            QLOGW("Input received. Returning to normal");
            set_vertical_mode(m_fly_mode_data.old_vertical_mode);
            set_horizontal_mode(m_fly_mode_data.old_horizontal_mode);
            set_yaw_mode(m_fly_mode_data.old_yaw_mode);
        }
        //make sure the commands are centered
        else
        {
            commands.sticks.yaw = 0.5f;
            commands.sticks.pitch = 0.5f;
            commands.sticks.roll = 0.5f;
            commands.sticks.throttle = 0.5f;
        }
    }

    stream::IFloat::Value thrust = m_thrust_output_stream->get_last_sample().value;
    stream::IAngular_Velocity::Value rate = m_rate_output_stream->get_last_sample().value;

    //////////////////////////////////////////////////////////////
    // Verticals

    if (m_vertical_mode == stream::IMultirotor_Commands::Vertical_Mode::THRUST)
    {
        thrust = commands.sticks.throttle * m_config->get_max_thrust();
    }
    else if (m_vertical_mode == stream::IMultirotor_Commands::Vertical_Mode::ALTITUDE)
    {
        {
            //float output = compute_ff_thrust(commands.sticks.throttle * m_config->get_vertical().get_max_speed() * 2.f);
            //thrust = output;
        }

        //apply command
        float speed = m_config->get_vertical().get_max_speed() * (commands.sticks.throttle * 2.f - 1.f);
        m_vertical_mode_data.target_altitude += speed * m_dts;

        thrust = compute_thrust_for_altitude(m_vertical_mode_data.target_altitude);
    }

    //clamp thrust
    thrust = math::clamp(thrust, m_config->get_min_thrust(), m_config->get_max_thrust());

    ////////////////////////////////////////////////////////////
    // Horizontals

    if (m_horizontal_mode == stream::IMultirotor_Commands::Horizontal_Mode::ANGLE_RATE)
    {
        //positive X roations pitch up (X rotates Y over Z).
        //The pitch stick is positive when pushed forward, so we have to invert it
        float pitch_stick = -(commands.sticks.pitch * 2.f - 1.f);

        //positive Y rotations roll clockwise (Y rotates Z over X)
        //The roll stick is positive when pushed right, so no need for inversion
        float roll_stick = (commands.sticks.roll * 2.f - 1.f);

        float max_speed = math::radians(m_config->get_horizontal().get_max_rate_deg());

        rate.x = pitch_stick * max_speed;
        rate.y = roll_stick * max_speed;
    }
    else if (m_horizontal_mode == stream::IMultirotor_Commands::Horizontal_Mode::ANGLE)
    {
        //positive X roations pitch up (X rotates Y over Z).
        //The pitch stick is positive when pushed forward, so we have to invert it
        float pitch_stick = -(commands.sticks.pitch * 2.f - 1.f);

        //positive Y rotations roll clockwise (Y rotates Z over X)
        //The roll stick is positive when pushed right, so no need for inversion
        float roll_stick = (commands.sticks.roll * 2.f - 1.f);

        math::vec2f const& max_angle = math::radians(m_config->get_horizontal().get_max_angle_deg());
        math::vec2f hrate = compute_horizontal_rate_for_angle(math::vec2f(pitch_stick * max_angle.x,
                                                                          roll_stick * max_angle.y));
        rate.x = hrate.x;
        rate.y = hrate.y;
    }
    else if (m_horizontal_mode == stream::IMultirotor_Commands::Horizontal_Mode::POSITION)
    {
        //apply command

        float pitch_stick = (commands.sticks.pitch * 2.f - 1.f);
        float roll_stick = (commands.sticks.roll * 2.f - 1.f);

        math::vec2f velocity_local(roll_stick * m_config->get_horizontal().get_max_speed(),
                                   pitch_stick * m_config->get_horizontal().get_max_speed());

        math::vec2f velocity_enu(math::rotate(m_inputs.frame.sample.value, math::vec3f(velocity_local)));

        m_horizontal_mode_data.target_enu_position += velocity_enu * m_dts;

        rate = compute_horizontal_rate_for_position(m_horizontal_mode_data.target_enu_position);
    }

    ///////////////////////////////////////////////////////////
    // Yaw

    if (m_yaw_mode == stream::IMultirotor_Commands::Yaw_Mode::ANGLE_RATE)
    {
        //positive Z rotations rotate the quad counter-clockwise as viewed from above (Z rotate X over Y)
        //the yaw stick rotates clockwise for positive Z angles so we have to invert it
        float stick = -(commands.sticks.yaw * 2.f - 1.f);

        float max_speed = math::radians(m_config->get_yaw().get_max_rate_deg());
        rate.z = stick * max_speed;
    }
    else if (m_yaw_mode == stream::IMultirotor_Commands::Yaw_Mode::ANGLE)
    {
        float max_speed = math::radians(m_config->get_yaw().get_max_rate_deg());

        //positive Z rotations rotate the quad counter-clockwise as viewed from above (Z rotate X over Y)
        //the yaw stick rotates clockwise for positive Z angles so we have to invert it
        float stick = -(commands.sticks.yaw * 2.f - 1.f);

        float speed = stick * max_speed;
        m_yaw_mode_data.target_angle += speed * m_dts;

        rate.z = compute_yaw_rate_for_angle(m_yaw_mode_data.target_angle);
    }

    ///////////////////////////////////////////////////////////

    m_rate_output_stream->push_sample(rate, true);
    m_thrust_output_stream->push_sample(thrust, true);
}

//////////////////////////////////////////////////////////////////////////////////////////

void Multirotor_Brain::process_flight_modes()
{
    stream::IMultirotor_Commands::Value const& commands = m_inputs.commands.sample.value;

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
}

//////////////////////////////////////////////////////////////////////////////////////////

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

//////////////////////////////////////////////////////////////////////////////////////////

void Multirotor_Brain::acquire_home_position()
{
    QASSERT(m_mode == Mode::IDLE);

    bool is_stable = true;

    std::deque<util::coordinates::ECEF>& history = m_home.position_history;

    //check if the position is stable
    for (size_t i = 1; i < history.size(); i++)
    {
        if (math::distance(history[0], history[i]) > m_config->get_max_allowed_position_variation())
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

//////////////////////////////////////////////////////////////////////////////////////////

void Multirotor_Brain::refresh_inputs(stream::IFrame::Sample const& frame,
                                stream::IECEF_Position::Sample const& position,
                                stream::IECEF_Velocity::Sample const& velocity,
                                stream::IECEF_Linear_Acceleration::Sample const& linear_acceleration,
                                stream::IProximity::Sample const& proximity)
{
    auto now = Clock::now();

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

//////////////////////////////////////////////////////////////////////////////////////////

void Multirotor_Brain::process()
{
    QLOG_TOPIC("Multirotor_Brain::process");

    m_state_output_stream->clear();
    m_rate_output_stream->clear();
    m_thrust_output_stream->clear();

    auto now = Clock::now();

    m_commands_accumulator.process([now, this](stream::IMultirotor_Commands::Sample const& i_commands)
    {
        m_inputs.commands.sample = i_commands;
        m_inputs.commands.last_valid_tp = i_commands.is_healthy ? now : m_inputs.commands.last_valid_tp;
    });

    if (now - m_inputs.commands.last_valid_tp >= std::chrono::milliseconds(500))
    {
        m_last_invalid_commands_tp = now;
    }

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
        state.throttle = m_thrust_output_stream->get_last_sample().value / m_config->get_max_thrust();

        for (size_t i = 0; i < samples_needed; i++)
        {
            m_state_output_stream->push_sample(state, true);
        }
    }
}

//////////////////////////////////////////////////////////////////////////////////////////

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

//////////////////////////////////////////////////////////////////////////////////////////

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

//////////////////////////////////////////////////////////////////////////////////////////

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
        PID::Params angle_x_params, angle_y_params;
        if (auto combined_pids = boost::get<hal::Multirotor_Brain_Config::Horizontal::Combined_Angle_PIDs>(&descriptor.get_angle_pids()))
        {
            fill_pid_params(angle_x_params, *combined_pids, output_rate);
            fill_pid_params(angle_y_params, *combined_pids, output_rate);
        }
        else
        {
            auto separate_pids = boost::get<hal::Multirotor_Brain_Config::Horizontal::Separate_Angle_PIDs>(&descriptor.get_angle_pids());
            fill_pid_params(angle_x_params, separate_pids->get_x_pid(), output_rate);
            fill_pid_params(angle_y_params, separate_pids->get_y_pid(), output_rate);
        }

        if (!m_horizontal_mode_data.angle_x_pid.set_params(angle_x_params) ||
            !m_horizontal_mode_data.angle_y_pid.set_params(angle_y_params))
        {
            return make_error("Bad horizontal angle PID params");
        }
    }

    {
        PID::Params params;
        fill_pid_params(params, m_config->get_yaw().get_angle_pid(), output_rate);
        if (!m_yaw_mode_data.angle_pid.set_params(params))
        {
            return make_error("Bad yaw angle PID params");
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

//////////////////////////////////////////////////////////////////////////////////////////

std::shared_ptr<const hal::INode_Config> Multirotor_Brain::get_config() const
{
    return m_config;
}

//////////////////////////////////////////////////////////////////////////////////////////

std::shared_ptr<const hal::INode_Descriptor> Multirotor_Brain::get_descriptor() const
{
    return m_descriptor;
}

ts::Result<std::shared_ptr<messages::INode_Message>> Multirotor_Brain::send_message(messages::INode_Message const& message)
{
    return make_error("Unknown message");
}

}
}
