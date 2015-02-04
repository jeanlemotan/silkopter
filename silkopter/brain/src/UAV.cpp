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
    //defaults
    m_motor_mixer.add_motor({ math::vec2f(0, 1), false });
    m_motor_mixer.add_motor({ math::vec2f(-1, 0), true });
    m_motor_mixer.add_motor({ math::vec2f(0, -1), false });
    m_motor_mixer.add_motor({ math::vec2f(1, 0), true });

    m_pids.yaw_rate.set_params(Yaw_Rate_PID::Params(0.1f, 0.05f, 0.001f, 0.5f));
    m_pids.pitch_rate.set_params(Pitch_Rate_PID::Params(0.1f, 0.05f, 0.001f, 0.5f));
    m_pids.roll_rate.set_params(Roll_Rate_PID::Params(0.1f, 0.05f, 0.001f, 0.5f));

    m_imu.gyroscope_sample_time_point = q::Clock::time_point(std::chrono::seconds(0));
    m_imu.accelerometer_sample_time_point = q::Clock::time_point(std::chrono::seconds(0));
    m_imu.compass_sample_time_point = q::Clock::time_point(std::chrono::seconds(0));
    m_imu.clock.set_epoch(Manual_Clock::time_point(Manual_Clock::duration(0)));

    load_settings();
    save_settings();
}

auto UAV::load_settings() -> bool
{
    autojsoncxx::ParsingResult result;
    UAV_Config cfg;
    if (!autojsoncxx::from_json_file("uav.cfg", cfg, result))
    {
        QLOGW("Failed to load uav.cfg: {}", result.description());
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

void UAV::set_uav_input(comms::UAV_Input const& input)
{
    m_uav_input = input;
}
auto UAV::get_uav_input() const -> comms::UAV_Input const&
{
    return m_uav_input;
}
void UAV::set_camera_mount_input(comms::Camera_Mount_Input const& input)
{
    m_camera_mount_input = input;
}
auto UAV::get_camera_mount_input() const -> comms::Camera_Mount_Input const&
{
    return m_camera_mount_input;
}
void UAV::set_motor_test_input(comms::Motor_Test_Input const& input)
{
    m_motor_test_input = input;
}
auto UAV::get_motor_test_input() const -> comms::Motor_Test_Input const&
{
    return m_motor_test_input;
}

void UAV::set_mode(comms::Mode new_mode)
{
    switch (m_mode)
    {
        case comms::Mode::IDLE:
        {
            if (new_mode == comms::Mode::ARMED)
            {
                if (do_arm_check())
                {
                    m_mode = new_mode;
                    m_motor_mixer.set_output_range(0.08f, 1.f);
                    m_hal.motors->cut_throttle();
                }
            }
            else
            {
                QLOGW("Cannot change mode to {} while idle.", static_cast<int>(new_mode));
            }
        }
        break;
        case comms::Mode::ARMED:
        {
            if (new_mode == comms::Mode::IDLE)
            {
                if (do_disarm_check())
                {
                    m_mode = new_mode;
                }
            }
            else
            {
                QLOGW("Cannot change mode to {} while armed. Disarm first!", static_cast<int>(new_mode));
            }
        }
        break;
        case comms::Mode::MOTOR_TEST:
        {
            if (new_mode == comms::Mode::IDLE)
            {
                m_mode = new_mode;
            }
            else
            {
                QLOGW("Cannot change mode to {} while testing motors.", static_cast<int>(new_mode));
            }
        }
        break;
        default:
        {
            QLOGE("Unrecognized mode {}.", static_cast<int>(m_mode));
        }
        break;
    }
}

comms::Mode UAV::get_operation_mode() const
{
    return m_mode;
}

bool UAV::do_arm_check() const
{
    if (m_state != State::IDLE)
    {
        QLOGW("Can only arm from the IDLE state. Now in {} state", static_cast<int>(m_state));
        return false;
    }

    auto now = q::Clock::now();

    bool ok = false;
    auto d = now - m_hal.sensors->get_last_accelerometer_sample().time_point;
    if (d > std::chrono::milliseconds(500))
    {
        QLOGW("Unhealthy accelerometer: {}", d);
        ok = false;
    }
    d = now - m_hal.sensors->get_last_gyroscope_sample().time_point;
    if (d > std::chrono::milliseconds(500))
    {
        QLOGW("Unhealthy gyroscope: {}", d);
        ok = false;
    }
    d = now - m_hal.sensors->get_last_compass_sample().time_point;
    if (d > std::chrono::milliseconds(500))
    {
        QLOGW("Unhealthy compass: {}", d);
        ok = false;
    }
    d = now - m_hal.sensors->get_last_barometer_sample().time_point;
    if (d > std::chrono::milliseconds(500))
    {
        QLOGW("Unhealthy barometer: {}", d);
        ok = false;
    }
    d = now - m_hal.sensors->get_last_sonar_sample().time_point;
    if (d > std::chrono::milliseconds(500))
    {
        QLOGW("Unhealthy sonar: {}", d);
        ok = false;
    }
    d = now - m_hal.sensors->get_last_thermometer_sample().time_point;
    if (d > std::chrono::milliseconds(1000))
    {
        QLOGW("Unhealthy thermometer: {}", d);
        ok = false;
    }
    d = now - m_hal.sensors->get_last_voltage_sample().time_point;
    if (d > std::chrono::milliseconds(1000))
    {
        QLOGW("Unhealthy voltage sensor: {}", d);
        ok = false;
    }
    d = now - m_hal.sensors->get_last_current_sample().time_point;
    if (d > std::chrono::milliseconds(500))
    {
        QLOGW("Unhealthy current sensor: {}", d);
        ok = false;
    }
//    d = now - m_hal.sensors->get_last_gps_sample().time_point;
//    if (d > std::chrono::milliseconds(4000))
//    {
//        QLOGW("Unhealthy GPS: {}", d);
//        ok = false;
//    }

    return ok;
}
bool UAV::do_disarm_check() const
{
    bool ok = m_state <= State::ARMED || m_state == State::CRASHED;
    if (!ok)
    {
        QLOGW("Cannot disarm while in state {} while armed. Land first!", static_cast<int>(m_state));
    }
    return ok;
}

auto UAV::get_ahrs() -> AHRS const&
{
    return m_ahrs;
}
auto UAV::get_battery() -> Battery const&
{
    return m_battery;
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

void UAV::process_battery_sensor_data()
{
    m_battery.process(m_hal.sensors->get_current_samples(), m_hal.sensors->get_voltage_samples());
}

void UAV::process_imu_sensor_data()
{
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

    auto g_it = gyroscope_samples.begin();
    auto a_it = accelerometer_samples.begin();
    auto c_it = compass_samples.begin();

    //const auto max_allowed_dt = std::chrono::milliseconds(500);

    //this matches the sensor samples that might come at different rates
    while (g_it != gyroscope_samples.end() || a_it != accelerometer_samples.end() || c_it != compass_samples.end())
    {
        bool has_new_gyroscope_sample = false;
        bool has_new_accelerometer_sample = false;
        //bool has_new_compass_sample = false;
        //update the current smaples

        auto sensor_now = m_imu.clock.now();

        Manual_Clock::duration min_dt{std::numeric_limits<uint32_t>::max()};
        if (g_it != gyroscope_samples.end())
        {
            QASSERT(g_it->dt < std::chrono::seconds(100));
            min_dt = math::min(min_dt, g_it->dt);
            if (m_imu.gyroscope_sample_time_point <= sensor_now)
            {
                m_imu.last_gyroscope_sample = *g_it++;
                m_imu.last_gyroscope_sample.value = m_imu.gyroscope_filter.process(m_imu.last_gyroscope_sample.value);
                m_imu.gyroscope_sample_time_point += m_imu.last_gyroscope_sample.dt;
                has_new_gyroscope_sample = true;
            }
        }
        if (a_it != accelerometer_samples.end())
        {
            QASSERT(a_it->dt < std::chrono::seconds(100));
            min_dt = math::min(min_dt, a_it->dt);
            if (m_imu.accelerometer_sample_time_point <= sensor_now)
            {
                m_imu.last_accelerometer_sample = *a_it++;
                m_imu.last_accelerometer_sample.value = m_imu.accelerometer_filter.process(m_imu.last_accelerometer_sample.value);
                m_imu.accelerometer_sample_time_point += m_imu.last_accelerometer_sample.dt;
                has_new_accelerometer_sample = true;
            }
        }
        if (c_it != compass_samples.end())
        {
            QASSERT(c_it->dt < std::chrono::seconds(100));
            min_dt = math::min(min_dt, c_it->dt);
            if (m_imu.compass_sample_time_point <= sensor_now)
            {
                m_imu.last_compass_sample = *c_it++;
                m_imu.last_compass_sample.value = m_imu.compass_filter.process(m_imu.last_compass_sample.value);
                m_imu.compass_sample_time_point += m_imu.last_compass_sample.dt;
//                has_new_compass_sample = true;
            }
        }
        QASSERT(min_dt.count() < std::numeric_limits<uint32_t>::max());

        //increment the time
        sensor_now += min_dt;
        m_imu.clock.advance(min_dt);

        //-------------------------------------
        //USING THE SAMPLES

        m_ahrs.process(m_imu.last_gyroscope_sample, m_imu.last_accelerometer_sample, m_imu.last_compass_sample);

        if (has_new_gyroscope_sample)
        {
            process_rate_pids(m_imu.last_gyroscope_sample);
        }
        if (has_new_accelerometer_sample)
        {
            process_dead_reckoning();
        }
    }
}

void UAV::process_dead_reckoning()
{
    float sample_dts = q::Seconds(m_imu.last_accelerometer_sample.dt).count();

    //auto old_acceleration = m_linear_motion.acceleration;
    math::vec3f acceleration = m_imu.last_accelerometer_sample.value;
    //math::vec3f gravity = math::transform(m_ahrs.get_e2b_mat(), math::vec3f(0, 0, 1)) * physics::constants::g;
    auto new_acceleration = math::transform(m_ahrs.get_mat_l2w(), acceleration) - math::vec3f(0, 0, physics::constants::g);
    //LOG_INFO("acc {}, l {}", new_acceleration, math::length(new_acceleration));

//            m_linear_motion.position += dts * (m_linear_motion.velocity + dts * old_acceleration * 0.5f);
//            m_linear_motion.velocity += dts * (old_acceleration + new_acceleration) * 0.5f;
//            m_linear_motion.acceleration = new_acceleration;

    m_linear_motion.acceleration = new_acceleration;
    m_linear_motion.velocity += m_linear_motion.acceleration * sample_dts;
    m_linear_motion.position += m_linear_motion.velocity * sample_dts;

    //        m_linear_motion.velocity = math::lerp(m_linear_motion.velocity, math::vec3f::zero, dts * 0.5f);

    //LOG_INFO("{}: {} / {} / {}", dts, new_acceleration, m_linear_motion.velocity, m_linear_motion.position);

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


void UAV::process_input()
{
    constexpr std::chrono::milliseconds PROCESS_PERIOD{10};

    auto now = q::Clock::now();
    auto dt = now - m_input_state.last_process_timestamp;
    if (dt < PROCESS_PERIOD)
    {
        return;
    }
    m_input_state.last_process_timestamp = now;

    if (m_uav_input.throttle_mode == comms::UAV_Input::Throttle_Mode::RATE)
    {
        process_input_throttle_rate(dt);
    }
    else if (m_uav_input.throttle_mode == comms::UAV_Input::Throttle_Mode::OFFSET)
    {
        process_input_throttle_offset(dt);
    }
    else if (m_uav_input.throttle_mode == comms::UAV_Input::Throttle_Mode::ASSISTED)
    {
        process_input_throttle_assisted(dt);
    }


    if (m_uav_input.pitch_roll_mode == comms::UAV_Input::Pitch_Roll_Mode::RATE)
    {
        process_input_pitch_roll_rate(dt);
    }
    else if (m_uav_input.pitch_roll_mode == comms::UAV_Input::Pitch_Roll_Mode::HORIZONTAL)
    {
        process_input_pitch_roll_horizontal(dt);
    }
    else if (m_uav_input.pitch_roll_mode == comms::UAV_Input::Pitch_Roll_Mode::ASSISTED)
    {
        process_input_pitch_roll_assisted(dt);
    }

    if (m_uav_input.yaw_mode == comms::UAV_Input::Yaw_Mode::RATE)
    {
        process_input_yaw_rate(dt);
    }
}

void UAV::process_input_throttle_rate(q::Clock::duration dt)
{
    float dts = q::Seconds(dt).count();

    m_throttle.reference = math::clamp(m_throttle.reference + m_uav_input.sticks.throttle * dts, 0.f, 1.f);
    m_throttle.current = m_throttle.reference;
}
void UAV::process_input_throttle_offset(q::Clock::duration dt)
{
    QUNUSED(dt);
    m_throttle.current = math::clamp(m_throttle.reference + m_uav_input.sticks.throttle, 0.f, 1.f);
}
void UAV::process_input_throttle_assisted(q::Clock::duration dt)
{
    QUNUSED(dt);
    QLOGW("assisted throttle not implemented");
}
void UAV::process_input_pitch_roll_rate(q::Clock::duration dt)
{
    QUNUSED(dt);
    m_pids.pitch_rate.set_target(m_uav_input.sticks.pitch * m_settings.max_pitch_rate);
    m_pids.roll_rate.set_target(m_uav_input.sticks.roll * m_settings.max_roll_rate);
}
void UAV::process_input_pitch_roll_horizontal(q::Clock::duration dt)
{
    QUNUSED(dt);

    math::quatf target;
    target.set_from_euler_xyz(-m_uav_input.sticks.pitch*m_settings.max_pitch_angle, //pitch - X
                              m_uav_input.sticks.roll*m_settings.max_roll_angle,    //roll - Y
                              0);                                               //yaw - Z. Empty cause it's always controlled as rate


    //target.set_from_euler_xyz(-rs.y*3.1415f, rs.x*3.1415f, -ls.x*3.1415f);


    auto& crt = m_ahrs.get_quat_l2w();
    auto diff = math::quatf::from_a_to_b(crt, target);

    math::vec3f diff_euler;
    diff.get_as_euler_xyz(diff_euler);

    auto v = math::clamp(diff.x / m_settings.max_pitch_angle, -1.f, 1.f);
    m_pids.pitch.set_input(v);

    v = math::clamp(diff.y / m_settings.max_roll_angle, -1.f, 1.f);
    m_pids.roll.set_input(v);

    m_pids.pitch.process(dt);
    m_pids.roll.process(dt);

    m_pids.pitch_rate.set_target(-m_pids.pitch.get_output() * m_settings.max_pitch_rate);
    m_pids.roll_rate.set_target(-m_pids.roll.get_output() * m_settings.max_roll_rate);
}
void UAV::process_input_pitch_roll_assisted(q::Clock::duration dt)
{
    QUNUSED(dt);
    QLOGW("assisted pitch/roll not implemented");
}
void UAV::process_input_yaw_rate(q::Clock::duration dt)
{
    QUNUSED(dt);
    m_pids.yaw_rate.set_target(m_uav_input.sticks.yaw * m_settings.max_yaw_rate);
}

void UAV::process_rate_pids(sensors::Gyroscope_Sample const& sample)
{
    auto input = sample.value;

    m_pids.pitch_rate.set_input(input.x);
    m_pids.pitch_rate.process(sample.dt);

    m_pids.roll_rate.set_input(input.y);
    m_pids.roll_rate.process(sample.dt);

    m_pids.yaw_rate.set_input(input.z);
    m_pids.yaw_rate.process(sample.dt);
}

void UAV::process_motors()
{
    if (m_mode == comms::Mode::ARMED)
    {
        m_motor_mixer.set_data(m_uav_input.sticks.throttle,
                               0,//m_pids.yaw_rate.get_output(),
                               0,//m_pids.pitch_rate.get_output(),
                               0);//-m_pids.roll_rate.get_output());

        //LOG_INFO("{} / {}, {}", m_angular_velocity, m_pids.pitch_rate.get_output(), m_pids.roll_rate.get_output());

        std::array<float, 4> throttles;
        for (size_t i = 0; i < m_motor_mixer.get_motor_count(); i++)
        {
            throttles[i] = math::sqrt(m_motor_mixer.get_motor_output(i));
        }

        //QLOGI("{.2}", throttles);

        m_hal.motors->set_throttles(throttles.data(), m_motor_mixer.get_motor_count());
    }
    else if (m_mode == comms::Mode::MOTOR_TEST)
    {
        m_hal.motors->set_throttles(m_motor_test_input.throttles.data(), m_motor_mixer.get_motor_count());
    }
    else
    {
        m_hal.motors->cut_throttle();
    }
}

void UAV::process_camera_mount()
{
    if (m_mode == comms::Mode::IDLE ||
        m_mode == comms::Mode::ARMED)
    {
        m_hal.camera_mount->set_rotation(m_camera_mount_input.rotation);
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

void UAV::process()
{
    auto now = q::Clock::now();

    {
        static q::Clock::time_point last_timestamp = q::Clock::now();
        auto dt = now - last_timestamp;
        last_timestamp = now;
        static q::Clock::duration min_dt, max_dt, avg_dt;
        static int xxx = 0;
        if (xxx == 0 && max_dt > std::chrono::milliseconds(10))
        {
            QLOGI("{}: min {}, max {}, avg {}", now, min_dt, max_dt, avg_dt);
            min_dt = dt;
            max_dt = dt;
            avg_dt = std::chrono::milliseconds(0);
        }
        min_dt = std::min(min_dt, dt);
        max_dt = std::max(max_dt, dt);
        avg_dt += dt;
        xxx++;
        if (xxx == 50)
        {
            avg_dt = avg_dt / xxx;
            xxx = 0;
        }
    }

    //LOG_INFO("Sensor lag: {}", m_sensor_clock.now_rt() - m_sensor_clock.now());

    //process samples at real-time - as they come
    process_imu_sensor_data();
    process_battery_sensor_data();

    process_input();
    process_motors();
    process_camera_mount();
}
