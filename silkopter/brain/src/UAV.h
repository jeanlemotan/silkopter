#pragma once

#include "common/Comm_Data.h"
#include "common/Manual_Clock.h"
#include "common/sensors/Sensor_Samples.h"
#include "AHRS.h"
#include "Battery.h"
#include "Motor_Mixer.h"
#include "IHAL.h"
#include "utils/PID.h"

namespace silk
{

class Butterworth //10hz
{
public:
    static constexpr size_t NZEROS  = 2;
    static constexpr size_t NPOLES  = 2;
    static constexpr float GAIN    = 1.058546241e+03;
    math::vec3f xv[NZEROS+1], yv[NPOLES+1];
    math::vec3f process(math::vec3f const& t)
    {
//        xv[0] = xv[1]; xv[1] = xv[2];
//        xv[2] = t / GAIN;
//        yv[0] = yv[1]; yv[1] = yv[2];
//        yv[2] =   (xv[0] + xv[2]) + 2.f * xv[1]
//                   + ( -0.9149758348f * yv[0]) + (  1.9111970674f * yv[1]);
//        return yv[2];
        return t;
    }
};


class UAV
{
public:
    UAV(IHAL& hal);


    void process();

    void set_mode(comms::Mode mode);
    auto get_operation_mode() const -> comms::Mode;

    // inputs

    void set_uav_input(comms::UAV_Input const& input);
    auto get_uav_input() const -> comms::UAV_Input const&;

    void set_camera_mount_input(comms::Camera_Mount_Input const& input);
    auto get_camera_mount_input() const -> comms::Camera_Mount_Input const&;

    void set_motor_test_input(comms::Motor_Test_Input const& input);
    auto get_motor_test_input() const -> comms::Motor_Test_Input const&;

    // data

    auto get_ahrs() -> AHRS const&;
    auto get_battery() -> Battery const&;
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
    void process_battery_sensor_data();
    void process_imu_sensor_data();
    void process_motors();
    void process_dead_reckoning();
    void process_input();
    void process_camera_mount();

    void process_rate_pids(sensors::Gyroscope_Sample const& sample);

    struct IMU
    {
        Butterworth gyroscope_filter;
        sensors::Gyroscope_Sample last_gyroscope_sample;
        Manual_Clock::time_point gyroscope_time_point;

        Butterworth accelerometer_filter;
        sensors::Accelerometer_Sample last_accelerometer_sample;
        Manual_Clock::time_point accelerometer_time_point;

        Butterworth compass_filter;
        sensors::Compass_Sample last_compass_sample;
        Manual_Clock::time_point compass_time_point;

        Manual_Clock clock;
    } m_imu;

    IHAL& m_hal;
    AHRS m_ahrs;
    Battery m_battery;
    Motor_Mixer m_motor_mixer;

    bool do_disarm_check() const;
    bool do_arm_check() const;

    enum class State
    {
        IDLE,
        ARMED,
        TAKE_OFF,
        AIRBORNE,
        LANDING,
        CRASHED
    } m_state;

    struct PIDs
    {
        Yaw_Rate_PID yaw_rate;
        Pitch_Rate_PID pitch_rate;
        Roll_Rate_PID roll_rate;
        Altitude_Rate_PID altitude_rate;

        Yaw_PID yaw;
        Pitch_PID pitch;
        Roll_PID roll;
        Altitude_PID altitude;
    } m_pids;

    comms::UAV_Input m_uav_input;
    comms::Camera_Mount_Input m_camera_mount_input;
    comms::Motor_Test_Input m_motor_test_input;

    struct Input_State
    {
        q::Clock::time_point last_process_timestamp{std::chrono::seconds(0)};
    } m_input_state;

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

    comms::Mode m_mode;

    struct Settings
    {
        float max_pitch_rate = math::anglef::pi;
        float max_roll_rate = math::anglef::pi;
        float max_yaw_rate = math::anglef::pi;

        float max_pitch_angle = math::radians(30.f);
        float max_roll_angle = math::radians(30.f);
    } m_settings;

    auto load_settings() -> bool;
    void save_settings();
};

}
