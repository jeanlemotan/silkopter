#pragma once

#include "common/input/UAV_Input.h"
#include "common/sensors/Sensor_Samples.h"
#include "AHRS.h"
#include "Motor_Mixer.h"
#include "HAL.h"
#include "utils/PID.h"

namespace silk
{

class UAV
{
public:
    UAV(HAL& hal);


    void process();

    void set_throttle_mode(uav_input::Throttle_Mode mode);
    void set_pitch_roll_mode(uav_input::Pitch_Roll_Mode mode);
    void set_yaw_mode(uav_input::Yaw_Mode mode);
    void set_reference_frame(uav_input::Reference_Frame frame);
    void set_sticks(uav_input::Sticks const& sticks);
    void set_assists(uav_input::Assists assists);
    void arm();
    void disarm();

    auto get_ahrs() -> AHRS const&;
    auto get_motor_mixer() -> Motor_Mixer const&;

    auto get_linear_acceleration_w() const -> math::vec3f const&;
    auto get_velocity_w() const -> math::vec3f const&;
    auto get_position_w() const -> math::vec3f const&;

    typedef util::PID<util::Butterworth_500_20_LPF> Yaw_Rate_PID;
    typedef util::PID<util::Butterworth_500_20_LPF> Pitch_Rate_PID;
    typedef util::PID<util::Butterworth_500_20_LPF> Roll_Rate_PID;
    typedef util::PID<util::Butterworth_500_20_LPF> Altitude_Rate_PID;

    typedef util::PID<> Yaw_PID;
    typedef util::PID<> Pitch_PID;
    typedef util::PID<> Roll_PID;
    typedef util::PID<> Altitude_PID;

    auto get_yaw_rate_pid_params() const -> Yaw_Rate_PID::Params;
    void set_yaw_rate_pid_params(Yaw_Rate_PID::Params const& params);

    auto get_pitch_rate_pid_params() const -> Pitch_Rate_PID::Params;
    void set_pitch_rate_pid_params(Pitch_Rate_PID::Params const& params);

    auto get_roll_rate_pid_params() const -> Roll_Rate_PID::Params;
    void set_roll_rate_pid_params(Roll_Rate_PID::Params const& params);

    auto get_altitude_rate_pid_params() const -> Altitude_Rate_PID::Params;
    void set_altitude_rate_pid_params(Altitude_Rate_PID::Params const& params);

    auto get_yaw_pid_params() const -> Yaw_PID::Params;
    void set_yaw_pid_params(Yaw_PID::Params const& params);

    auto get_pitch_pid_params() const -> Pitch_PID::Params;
    void set_pitch_pid_params(Pitch_PID::Params const& params);

    auto get_roll_pid_params() const -> Roll_PID::Params;
    void set_roll_pid_params(Roll_PID::Params const& params);

    auto get_altitude_pid_params() const -> Altitude_PID::Params;
    void set_altitude_pid_params(Altitude_PID::Params const& params);

    struct Assist_Params
    {
        //AVOID_THE_GROUND params
        //The uav will try to keep this minimum distance from the ground.
        float min_ground_distance = 1.5f;

        //AVOID_THE_USER params
        //This feature is meant to protect the user from getting hit by the uav
        //Avoid this radius around the home position
        float home_radius = 3.f;

        //STAY_IN_RADIO_RANGE params
        //The maximum allowed radio range, in meters
        float max_radio_distance = 800.f;

        //AVOID_ALTITUDE_DROP params
        //the minimum time to ground in seconds, calculated from the current altitude and speed with g acceleration
        //A uav hovering at 100m will take ~4.5 seconds to hit the ground at terminal speed of 44.2m/s
        q::Seconds min_time_to_ground = q::Seconds(3.f);

        //STAY_IN_PERIMETER
        //a list of points that define an enclosed perimeter. The last point is connected to the first point
        std::vector<math::vec2f> perimeter_points;
    };

    auto get_assist_params() const -> Assist_Params;
    void set_assist_params(Assist_Params const& params);

private:
    void process_sensor_data();
    void process_motors();
    void process_motion();
    void process_input();

    void process_pids();
    void process_rate_pids(q::Clock::duration dt);
    void process_stability_pids(q::Clock::duration dt);

    Gyroscope_Sample m_last_gyroscope_sample;
    q::Clock::time_point m_gyroscope_sample_time_point;

    Accelerometer_Sample m_last_accelerometer_sample;
    q::Clock::time_point m_accelerometer_sample_time_point;

    Compass_Sample m_last_compass_sample;
    q::Clock::time_point m_compass_sample_time_point;

    q::Clock::time_point m_last_sample_time_point;


    HAL& m_hal;
    AHRS m_ahrs;
    Motor_Mixer m_motor_mixer;

    enum class State
    {
        IDLE,
        PRE_ARM_CHECK,
        ARMED,
        TAKE_OFF,
        AIRBORNE,
        LANDING,
        CRASHED
    } m_state;

    struct PIDs
    {
        q::Clock::time_point last_process_timestamp{std::chrono::seconds(0)};

        math::vec3f angular_velocity;
        size_t angular_velocity_samples = 0;

        Yaw_Rate_PID yaw_rate;
        Pitch_Rate_PID pitch_rate;
        Roll_Rate_PID roll_rate;
        Altitude_Rate_PID altitude_rate;

        Yaw_PID yaw;
        Pitch_PID pitch;
        Roll_PID roll;
        Altitude_PID altitude;
    } m_pids;

    struct Input
    {
        q::Clock::time_point last_process_timestamp{std::chrono::seconds(0)};

        uav_input::Throttle_Mode throttle_mode;
        uav_input::Pitch_Roll_Mode pitch_roll_mode;
        uav_input::Yaw_Mode yaw_mode;
        uav_input::Reference_Frame reference_frame;
        uav_input::Sticks sticks;
        uav_input::Assists assists;
    } m_input;

    void process_input_throttle_rate(q::Clock::duration dt);
    void process_input_throttle_offset(q::Clock::duration dt);
    void process_input_throttle_assisted(q::Clock::duration dt);
    void process_input_pitch_roll_rate(q::Clock::duration dt);
    void process_input_pitch_roll_horizontal(q::Clock::duration dt);
    void process_input_pitch_roll_assisted(q::Clock::duration dt);
    void process_input_yaw_rate(q::Clock::duration dt);

    struct Throttle
    {
        float reference = 0.f;
        float current = 0.f;
    } m_throttle;

    struct Linear_Motion
    {
        math::vec3f old_acceleration;
        math::vec3f acceleration;
        math::vec3f old_velocity;
        math::vec3f velocity;
        math::vec3f position;
    } m_linear_motion;

    Assist_Params m_assist_params;

    bool m_is_armed = false;

    struct Settings
    {
        float max_pitch_rate = math::anglef::pi;
        float max_roll_rate = math::anglef::pi;
        float max_yaw_rate = math::anglef::pi;
    } m_settings;

    auto load_settings() -> bool;
    void save_settings();
};

}
