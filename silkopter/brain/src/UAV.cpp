#include "BrainStdAfx.h"
#include "UAV.h"
#include "physics/constants.h"
#include "utils/Json_Helpers.h"
#include "utils/Timed_Scope.h"

#include "sz_math.hpp"
#include "sz_pid_params.hpp"
#include "sz_motor_info.hpp"
#include "sz_uav_config.hpp"

using namespace silk;

UAV::UAV(HAL& hal)
    : m_hal(hal)
{
    m_last_timestamp = q::Clock::now();

    //defaults
    m_motor_mixer.add_motor({ math::vec2f(0, 1), false });
    m_motor_mixer.add_motor({ math::vec2f(-1, 0), true });
    m_motor_mixer.add_motor({ math::vec2f(0, -1), false });
    m_motor_mixer.add_motor({ math::vec2f(1, 0), true });

    m_pids.yaw_rate.set_params(Yaw_Rate_PID::Params(0.1f, 0.05f, 0.001f, 0.5f));
    m_pids.pitch_rate.set_params(Pitch_Rate_PID::Params(0.1f, 0.05f, 0.001f, 0.5f));
    m_pids.roll_rate.set_params(Roll_Rate_PID::Params(0.1f, 0.05f, 0.001f, 0.5f));

    m_gyroscope_sample_time_point = q::Clock::time_point(std::chrono::seconds(0));
    m_accelerometer_sample_time_point = q::Clock::time_point(std::chrono::seconds(0));
    m_compass_sample_time_point = q::Clock::time_point(std::chrono::seconds(0));
    m_last_sample_time_point = q::Clock::time_point(std::chrono::seconds(0));


    load_settings();
    save_settings();
}

auto UAV::load_settings() -> bool
{
    autojsoncxx::ParsingResult result;
    UAV_Config cfg;
    if (!autojsoncxx::from_json_file("uav.cfg", cfg, result))
    {
        SILK_WARNING("Failed to load uav.cfg: {}", result.description());
        return false;
    }

    m_pids.yaw_rate.set_params(cfg.pids.yaw_rate);
    m_pids.pitch_rate.set_params(cfg.pids.pitch_rate);
    m_pids.roll_rate.set_params(cfg.pids.roll_rate);
    m_pids.altitude_rate.set_params(cfg.pids.altitude_rate);

    m_pids.yaw.set_params(cfg.pids.yaw);
    m_pids.pitch.set_params(cfg.pids.pitch);
    m_pids.roll.set_params(cfg.pids.roll);
    m_pids.altitude.set_params(cfg.pids.altitude);

    if (!cfg.motors.empty())
    {
        m_motor_mixer.remove_all_motors();
        for (auto const& mi: cfg.motors)
        {
            m_motor_mixer.add_motor(mi);
        }
    }

    return true;
}
void UAV::save_settings()
{
    UAV_Config cfg;

    cfg.pids.yaw_rate = m_pids.yaw_rate.get_params();
    cfg.pids.pitch_rate = m_pids.pitch_rate.get_params();
    cfg.pids.roll_rate = m_pids.roll_rate.get_params();
    cfg.pids.altitude_rate = m_pids.altitude_rate.get_params();

    cfg.pids.yaw = m_pids.yaw.get_params();
    cfg.pids.pitch = m_pids.pitch.get_params();
    cfg.pids.roll = m_pids.roll.get_params();
    cfg.pids.altitude = m_pids.altitude.get_params();

    for (size_t i = 0; i < m_motor_mixer.get_motor_count(); i++)
    {
        cfg.motors.push_back(m_motor_mixer.get_motor_info(i));
    }

    autojsoncxx::to_pretty_json_file("uav.cfg", cfg);
}

void UAV::set_throttle_mode(uav_input::Throttle_Mode mode)
{
    m_input.throttle_mode = mode;
}
void UAV::set_pitch_roll_mode(uav_input::Pitch_Roll_Mode mode)
{
    m_input.pitch_roll_mode = mode;
}
void UAV::set_yaw_mode(uav_input::Yaw_Mode mode)
{
    m_input.yaw_mode = mode;
}
void UAV::set_reference_frame(uav_input::Reference_Frame frame)
{
    m_input.reference_frame = frame;
}
void UAV::set_sticks(uav_input::Sticks const& sticks)
{
    m_input.sticks = sticks;
}
void UAV::set_assists(uav_input::Assists assists)
{
    m_input.assists = assists;
}
void UAV::arm()
{
    m_is_armed = true;
    m_motor_mixer.set_output_range(0.05f, 1.f);
}
void UAV::disarm()
{
    m_is_armed = false;
    m_motor_mixer.set_output_range(0, 1);

    m_hal.motors->cut_throttle();
}

auto UAV::get_ahrs() -> AHRS const&
{
    return m_ahrs;
}
auto UAV::get_motor_mixer() -> Motor_Mixer const&
{
    return m_motor_mixer;
}

auto UAV::get_linear_acceleration_w() const -> math::vec3f const&
{
    return m_linear_motion.acceleration;
}
auto UAV::get_velocity_w() const -> math::vec3f const&
{
    return m_linear_motion.velocity;
}
auto UAV::get_position_w() const -> math::vec3f const&
{
    return m_linear_motion.position;
}


void UAV::process()
{
    auto now = q::Clock::now();
    auto dt = now - m_last_timestamp;
    m_last_timestamp = now;
    static q::Clock::duration min_dt, max_dt, avg_dt;
    static int xxx = 0;
    if (xxx == 0)
    {
        //SILK_INFO("min {}, max {}, avg {}", min_dt, max_dt, avg_dt);
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

    process_input(dt);
    process_sensor_data(dt);
    process_motors(dt);
}

void UAV::process_input(q::Clock::duration dt)
{
    if (m_input.throttle_mode == uav_input::Throttle_Mode::RATE)
    {
        process_input_throttle_rate(dt);
    }
    else if (m_input.throttle_mode == uav_input::Throttle_Mode::OFFSET)
    {
        process_input_throttle_offset(dt);
    }
    else if (m_input.throttle_mode == uav_input::Throttle_Mode::ASSISTED)
    {
        process_input_throttle_assisted(dt);
    }


    if (m_input.pitch_roll_mode == uav_input::Pitch_Roll_Mode::RATE)
    {
        process_input_pitch_roll_rate(dt);
    }
    else if (m_input.pitch_roll_mode == uav_input::Pitch_Roll_Mode::HORIZONTAL)
    {
        process_input_pitch_roll_horizontal(dt);
    }
    else if (m_input.pitch_roll_mode == uav_input::Pitch_Roll_Mode::ASSISTED)
    {
        process_input_pitch_roll_assisted(dt);
    }

    if (m_input.yaw_mode == uav_input::Yaw_Mode::RATE)
    {
        process_input_yaw_rate(dt);
    }
}

void UAV::process_input_throttle_rate(q::Clock::duration dt)
{
    float dts = q::Seconds(dt).count();

    m_throttle.reference = math::clamp(m_throttle.reference + m_input.sticks.throttle * dts, 0.f, 1.f);
    m_throttle.current = m_throttle.reference;
}
void UAV::process_input_throttle_offset(q::Clock::duration dt)
{
    QUNUSED(dt);
    m_throttle.current = math::clamp(m_throttle.reference + m_input.sticks.throttle, 0.f, 1.f);
}
void UAV::process_input_throttle_assisted(q::Clock::duration dt)
{
    QUNUSED(dt);
    SILK_WARNING("assisted throttle not implemented");
}
void UAV::process_input_pitch_roll_rate(q::Clock::duration dt)
{
    QUNUSED(dt);
    m_pids.pitch_rate.set_target(m_input.sticks.pitch * m_settings.max_pitch_rate);
    m_pids.roll_rate.set_target(m_input.sticks.roll * m_settings.max_roll_rate);
}
void UAV::process_input_pitch_roll_horizontal(q::Clock::duration dt)
{
    QUNUSED(dt);
}
void UAV::process_input_pitch_roll_assisted(q::Clock::duration dt)
{
    QUNUSED(dt);
    SILK_WARNING("assisted pitch/roll not implemented");
}
void UAV::process_input_yaw_rate(q::Clock::duration dt)
{
    QUNUSED(dt);
    m_pids.yaw_rate.set_target(m_input.sticks.yaw * m_settings.max_yaw_rate);
}

void UAV::process_sensor_data(q::Clock::duration dt)
{
    QUNUSED(dt);
//    static int xxx = 100;
//    xxx--;
//    if (xxx > 0)
//    {
//        return;
//    }

    static q::Clock::time_point xxx = q::Clock::now();
    auto now = q::Clock::now();
    if (now - xxx > std::chrono::seconds(10))
    {
        xxx = now;
        m_linear_motion.acceleration.set(0, 0, 0);
        m_linear_motion.velocity.set(0, 0, 0);
        m_linear_motion.position.set(0, 0, 0);
    }

    //combine accelerometer, gyroscope and compass readings

    auto const& gyroscope_samples = m_hal.sensors->get_gyroscope_samples();
    auto const& accelerometer_samples = m_hal.sensors->get_accelerometer_samples();
    auto const& compass_samples = m_hal.sensors->get_compass_samples();

    //find the start time_point
//    if (!gyroscope_samples.empty())
//    {
//        m_last_sample_time_point = math::min(m_last_sample_time_point, gyroscope_samples.front().time_point);
//    }
//    if (!accelerometer_samples.empty())
//    {
//        m_last_sample_time_point = math::min(m_last_sample_time_point, accelerometer_samples.front().time_point);
//    }
//    if (!compass_samples.empty())
//    {
//        m_last_sample_time_point = math::min(m_last_sample_time_point, compass_samples.front().time_point);
//    }

    auto g_it = gyroscope_samples.begin();
    auto a_it = accelerometer_samples.begin();
    auto c_it = compass_samples.begin();

    //this matches the sensor samples that might come at different rates
    while (g_it != gyroscope_samples.end() || a_it != accelerometer_samples.end() || c_it != compass_samples.end())
    {
        bool has_new_gyroscope_sample = false;
        bool has_new_accelerometer_sample = false;
        bool has_new_compass_sample = false;
        //update the current smaples
        if (g_it != gyroscope_samples.end() && m_gyroscope_sample_time_point <= m_last_sample_time_point)
        {
            m_last_gyroscope_sample = *g_it++;
            m_gyroscope_sample_time_point += m_last_gyroscope_sample.dt;
            has_new_gyroscope_sample = true;
        }
        if (a_it != accelerometer_samples.end() && m_accelerometer_sample_time_point <= m_last_sample_time_point)
        {
            m_last_accelerometer_sample = *a_it++;
            m_accelerometer_sample_time_point += m_last_accelerometer_sample.dt;
            has_new_accelerometer_sample = true;
        }
        if (c_it != compass_samples.end() && m_compass_sample_time_point <= m_last_sample_time_point)
        {
            m_last_compass_sample = *c_it++;
            m_compass_sample_time_point += m_last_compass_sample.dt;
            has_new_compass_sample = true;
        }

        //increment the time
        m_last_sample_time_point += math::min(math::min(m_last_gyroscope_sample.dt, m_last_accelerometer_sample.dt), m_last_compass_sample.dt);

        //-------------------------------------
        //USING THE SAMPLES

        m_ahrs.process(m_last_gyroscope_sample, m_last_accelerometer_sample, m_last_compass_sample);

        if (has_new_gyroscope_sample)
        {
            process_rate_pids();
        }

        if (has_new_accelerometer_sample)
        {
            process_motion();
        }
    }

    process_stability_pids();

//    SILK_INFO("time: {} for {} samples", q::Clock::now() - start, samples.size());


    //m_position = math::lerp(m_position, math::vec3f::zero, 0.01f);
}

void UAV::process_rate_pids()
{
    auto sample_dt = m_last_gyroscope_sample.dt;
    auto const& input = m_last_gyroscope_sample.value;

    m_pids.pitch_rate.set_input(input.x);
    m_pids.pitch_rate.process(sample_dt);

    m_pids.roll_rate.set_input(input.y);
    m_pids.roll_rate.process(sample_dt);

    m_pids.yaw_rate.set_input(input.z);
    m_pids.yaw_rate.process(sample_dt);
}

void UAV::process_stability_pids()
{
    //these are dot products (i.e. -1 .. 1) not angles
    //auto pitch = math::dot(m_ahrs.get_up_vector_l(), )
}

void UAV::process_motion()
{
    float sample_dts = q::Seconds(m_last_accelerometer_sample.dt).count();

    //auto old_acceleration = m_linear_motion.acceleration;
    math::vec3f acceleration = m_last_accelerometer_sample.value;
    //math::vec3f gravity = math::transform(m_ahrs.get_e2b_mat(), math::vec3f(0, 0, 1)) * physics::constants::g;
    auto new_acceleration = math::transform(m_ahrs.get_mat_l2w(), acceleration) - math::vec3f(0, 0, physics::constants::g);
    //SILK_INFO("acc {}, l {}", new_acceleration, math::length(new_acceleration));

//            m_linear_motion.position += dts * (m_linear_motion.velocity + dts * old_acceleration * 0.5f);
//            m_linear_motion.velocity += dts * (old_acceleration + new_acceleration) * 0.5f;
//            m_linear_motion.acceleration = new_acceleration;

    m_linear_motion.acceleration = new_acceleration;
    m_linear_motion.velocity += m_linear_motion.acceleration * sample_dts;
    m_linear_motion.position += m_linear_motion.velocity * sample_dts;

    //        m_linear_motion.velocity = math::lerp(m_linear_motion.velocity, math::vec3f::zero, dts * 0.5f);

    //SILK_INFO("{}: {} / {} / {}", dts, new_acceleration, m_linear_motion.velocity, m_linear_motion.position);

//            m_linear_motion.position.z = math::lerp<float, math::safe>(
//                        m_linear_motion.position.z,
//                        sample.sonar.value,
//                        dts * 0.9f);

    if (m_linear_motion.position.z < 0)
    {
        m_linear_motion.position.z = 0;
        m_linear_motion.velocity.z = std::max(0.f, m_linear_motion.velocity.z);
    }
}

void UAV::process_motors(q::Clock::duration dt)
{
    QUNUSED(dt);

    if (m_is_armed)
    {
        m_motor_mixer.set_data(m_input.sticks.throttle,
                               m_pids.yaw_rate.get_output(),
                               m_pids.pitch_rate.get_output(),
                               m_pids.roll_rate.get_output());

        //SILK_INFO("{} / {}, {}", m_angular_velocity, m_pids.pitch_rate.get_output(), m_pids.roll_rate.get_output());

        std::array<float, MAX_MOTOR_COUNT> throttles;
        for (size_t i = 0; i < m_motor_mixer.get_motor_count(); i++)
        {
            throttles[i] = m_motor_mixer.get_motor_output(i);
        }

        //SILK_INFO("{.2}", throttles);

        m_hal.motors->set_throttles(throttles.data(), m_motor_mixer.get_motor_count());
    }
}


auto UAV::get_yaw_rate_pid_params() const -> Yaw_Rate_PID::Params
{
    return m_pids.yaw_rate.get_params();
}
void UAV::set_yaw_rate_pid_params(Yaw_Rate_PID::Params const& params)
{
    m_pids.yaw_rate.set_params(params);
    save_settings();
}
auto UAV::get_pitch_rate_pid_params() const -> Pitch_Rate_PID::Params
{
    return m_pids.pitch_rate.get_params();
}
void UAV::set_pitch_rate_pid_params(Pitch_Rate_PID::Params const& params)
{
    m_pids.pitch_rate.set_params(params);
    save_settings();
}
auto UAV::get_roll_rate_pid_params() const -> Roll_Rate_PID::Params
{
    return m_pids.roll_rate.get_params();
}
void UAV::set_roll_rate_pid_params(Roll_Rate_PID::Params const& params)
{
    m_pids.roll_rate.set_params(params);
    save_settings();
}
auto UAV::get_altitude_rate_pid_params() const -> Altitude_Rate_PID::Params
{
    return m_pids.altitude_rate.get_params();
}
void UAV::set_altitude_rate_pid_params(Altitude_Rate_PID::Params const& params)
{
    m_pids.altitude_rate.set_params(params);
    save_settings();
}
auto UAV::get_yaw_pid_params() const -> Yaw_PID::Params
{
    return m_pids.yaw.get_params();
}
void UAV::set_yaw_pid_params(Yaw_PID::Params const& params)
{
    m_pids.yaw.set_params(params);
    save_settings();
}
auto UAV::get_pitch_pid_params() const -> Pitch_PID::Params
{
    return m_pids.pitch.get_params();
}
void UAV::set_pitch_pid_params(Pitch_PID::Params const& params)
{
    m_pids.pitch.set_params(params);
    save_settings();
}
auto UAV::get_roll_pid_params() const -> Roll_PID::Params
{
    return m_pids.roll.get_params();
}
void UAV::set_roll_pid_params(Roll_PID::Params const& params)
{
    m_pids.roll.set_params(params);
    save_settings();
}
auto UAV::get_altitude_pid_params() const -> Altitude_PID::Params
{
    return m_pids.altitude.get_params();
}
void UAV::set_altitude_pid_params(Altitude_PID::Params const& params)
{
    m_pids.altitude.set_params(params);
    save_settings();
}

auto UAV::get_assist_params() const -> Assist_Params
{
    return m_assist_params;
}
void UAV::set_assist_params(Assist_Params const& params)
{
    m_assist_params = params;
}
