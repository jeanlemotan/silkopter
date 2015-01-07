#pragma once

namespace silk
{
namespace comms
{

enum class Telemetry_Message : uint8_t
{
    //------------------------
    //sensors
    SENSORS,
    RAW_SENSORS,

    //------------------------
    //UAV
    UAV_AHRS_ROTATION_L2W,
    UAV_LINEAR_ACCELERATION_W,
    UAV_VELOCITY_W,
    UAV_POSITION_W,
    UAV_BATTERY_CAPACITY_USED,
    UAV_BATTERY_CURRENT,
    UAV_BATTERY_VOLTAGE,
};

enum class Sensor : uint16_t
{
    ACCELEROMETER   = 1 << 0,
    GYROSCOPE       = 1 << 1,
    COMPASS         = 1 << 2,
    BAROMETER       = 1 << 3,
    THERMOMETER     = 1 << 4,
    SONAR           = 1 << 5,
    GPS             = 1 << 6,
    VOLTAGE         = 1 << 7,
    CURRENT         = 1 << 8,
};
typedef q::util::Flag_Set<Sensor, uint16_t> Sensors;


enum class Sim_Message : uint8_t
{
    SENSORS,
    MOTORS,
};


enum class Setup_Message : uint8_t
{
    CAMERA_PARAMS,

    //------------------------
    //sensors
    REQUEST_RAW_SENSORS,

    //------------------------
    //calibration

    CALIBRATION_ACCELEROMETER,
    CALIBRATION_GYROSCOPE,
    CALIBRATION_COMPASS,

    //------------------------
    //pids

    YAW_RATE_PID_PARAMS,
    PITCH_RATE_PID_PARAMS,
    ROLL_RATE_PID_PARAMS,
    ALTITUDE_RATE_PID_PARAMS,

    YAW_PID_PARAMS,
    PITCH_PID_PARAMS,
    ROLL_PID_PARAMS,
    ALTITUDE_PID_PARAMS,

    //------------------------
    //uav misc params
    ASSIST_PARAMS,
};

struct Camera_Params
{
    struct Toggles
    {
        uint8_t record : 1;
    };

    enum class Stream_Quality : uint8_t
    {
        HIGH,
        MEDIUM,
        LOW
    };

    ///////////////////////////////
    /// Data

    q::Clock::duration shutter_speed{0}; //zero means auto
    uint32_t iso = 0; //zero means auto

    Stream_Quality stream_quality = Stream_Quality::LOW;
    Toggles toggles;
};


enum class Input_Message : uint8_t
{
    UAV_INPUT,
    CAMERA_MOUNT_INPUT
};


struct UAV_Input
{
    struct Toggles
    {
        union
        {
            struct
            {
                uint8_t armed       : 1;
                uint8_t take_off    : 1;
                uint8_t land        : 1;
                uint8_t lights_on   : 1;
            };
            uint8_t _all = 0;
        };
    };

    enum class Throttle_Mode : uint8_t
    {
        RATE,       //throttle rate of change, per second. 1 means full throttle in one second
        OFFSET,     //-1 .. 1 are offsets from current throttle. Zero means keep same throttle
        ASSISTED,   //climb speed, meters per second
    };

    enum class Pitch_Roll_Mode : uint8_t
    {
        RATE,       //angle rate of change - radians per second
        HORIZONTAL, //angle from horizontal. zero means horizontal
        ASSISTED,   //speed, meters per second
    };

    enum class Yaw_Mode : uint8_t
    {
        RATE,
    };

    //the reference frame for the user controls
    enum class Reference_Frame : uint8_t
    {
        LOCAL, 	//normal mode - back means back of uav
        USER,	//simple mode - back means towards the user, front away from her.
    };

    struct Assists
    {
        union
        {
            struct
            {
                uint8_t stay_in_radio_range     : 1; //avoid out of radio range situations. Configured by Params::max_radio_distance
                uint8_t stay_in_battery_range   : 1; //avoid going too far considering current battery.
                uint8_t stay_in_perimeter       : 1; //stay in a configured perimeter.
                uint8_t avoid_altitude_drop     : 1; //avoid dropping too much altitude too fast. Controlled by Params::min_time_to_ground parameter.
                uint8_t avoid_the_user          : 1; //avoid being too low around the home position. Needs sonar and GPS. Controlled by the Params::home_radius
                uint8_t avoid_the_ground        : 1; //maintains a min distance from the ground. Needs sonar. Configured by Params::min_ground_distance parameter
            };
            uint8_t _all = 0;
        };
    };

    struct Sticks
    {
        float throttle = 0;
        float yaw = 0;
        float pitch = 0;
        float roll = 0;
    };

    ///////////////////////////////
    /// Data

    Toggles toggles;
    Sticks sticks;
    Throttle_Mode throttle_mode = Throttle_Mode::OFFSET;
    Pitch_Roll_Mode pitch_roll_mode = Pitch_Roll_Mode::HORIZONTAL;
    Yaw_Mode yaw_mode = Yaw_Mode::RATE;
    Reference_Frame reference_frame = Reference_Frame::LOCAL;
    Assists assists;
};

struct Camera_Mount_Input
{
    math::vec3f rotation;
};



}
}
