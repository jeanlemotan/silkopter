#pragma once

namespace silk
{
namespace uav_input
{

    enum class Input : uint8_t
    {
        THROTTLE_MODE,
        PITCH_ROLL_MODE,
        YAW_MODE,
        STICKS,
        ARM,                //arm the UAV. This is a request and the UAV will arm if it's possible
        DISARM,             //disarm the UAV. This is a request and the UAV will disarm if it's possible
        TAKE_OFF,           //take off
        LAND,               //land
        LIGHTS_ON,          //turn on the lights
        ASSISTS,
    };

    struct Sticks
    {
        float throttle = 0;
        float yaw = 0;
        float pitch = 0;
        float roll = 0;
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
        USER,	//simple mode - back means towards the user, front away from her. Needs GPS
    };

    enum class Assist : uint32_t
    {
        STAY_IN_RADIO_RANGE 	= 1 << 0,	//avoid out of radio range situations. Needs GPS. Configured by Params::max_radio_distance
        STAY_IN_BATTERY_RANGE	= 1 << 1, 	//avoid going too far considering current battery. Needs GPS
        STAY_IN_PERIMETER		= 1 << 2,	//stay in a configured perimeter. Needs GPS
        AVOID_ALTITUDE_DROP 	= 1 << 3,	//avoid dropping too much altitude too fast. Needs GPS. Controlled by Params::min_time_to_ground parameter.
        AVOID_THE_USER			= 1 << 4, 	//avoid being too low around the home position. Needs sonar and GPS. Controlled by the Params::home_radius
        AVOID_THE_GROUND 		= 1 << 5,	//maintains a min distance from the ground. Needs sonar. Configured by Params::min_ground_distance parameter
    };
    typedef q::util::Flag_Set<Assist, uint32_t> Assists;

}
}
