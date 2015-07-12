#pragma once

#include "Stream_Base.h"

namespace silk
{
namespace node
{
namespace stream
{

class IMulti_Input : public IScalar_Stream<Type::MULTI_INPUT>
{
public:
    typedef std::false_type can_be_filtered_t;

    struct Toggles
    {
        union
        {
            struct
            {
                uint8_t take_off    : 1;
                uint8_t land        : 1;
                uint8_t lights_on   : 1;
            };
            uint8_t _all = 0;
        };
    };

    enum class Mode : uint8_t
    {
        IDLE,
        ARMED,
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


    struct Value
    {
        Toggles toggles;
        Sticks sticks;
        Mode mode = Mode::IDLE;
        Throttle_Mode throttle_mode = Throttle_Mode::OFFSET;
        Pitch_Roll_Mode pitch_roll_mode = Pitch_Roll_Mode::HORIZONTAL;
        Yaw_Mode yaw_mode = Yaw_Mode::RATE;
        Reference_Frame reference_frame = Reference_Frame::LOCAL;
        Assists assists;
    };

    typedef stream::Sample<Value>     Sample;

    virtual ~IMulti_Input() {}

    virtual auto get_samples() const -> std::vector<Sample> const& = 0;
};
DECLARE_CLASS_PTR(IMulti_Input);


}
}
}


