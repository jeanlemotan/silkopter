#pragma once

namespace silk
{
namespace detail
{

    enum class Comm_Message : uint16_t
    {
        PING,
        PONG,
        
        //------------------------
        CAMERA_INPUT,
        UAV_INPUT,

        //------------------------
        //sensors
        SENSORS,
        RAW_SENSORS,

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

        //------------------------
        //UAV
        UAV_ROTATION_L2W,
        UAV_LINEAR_ACCELERATION_W,
        UAV_VELOCITY_W,
        UAV_POSITION_W
    };

    enum class Comm_Message_Sensor : uint16_t
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
    typedef q::util::Flag_Set<Comm_Message_Sensor, uint16_t> Comm_Message_Sensors;

}
}
