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
                uint8_t return_home : 1;
            };
            uint8_t _all = 0;
        };
    };

    enum class Mode : uint8_t
    {
        IDLE,
        ARMED,
    };

    enum class Vertical_Mode : uint8_t
    {
        THRUST_RATE,
        THRUST_OFFSET,
        CLIMB_RATE,
    };
    struct Vertical_Mode_Params
    {
        float thrust_rate = 0;      //thrust change rate in N/s. 1 means full thrust in one second
        float thrust_offset = 0;    //offset applied to the current thrust
        float climb_rate = 0;       //climb speed, meters per second
    };

    enum class Horizontal_Mode : uint8_t
    {
        ANGLE_RATE,
        ANGLE,
        VELOCITY,
    };
    struct Horizontal_Mode_Params
    {
        math::vec2f angle_rate;   //angle rate of change - radians per second
        math::vec2f angle;        //angle from horizontal. zero means horizontal
        math::vec2f velocity;     //speed, meters per second
    };

    enum class Yaw_Mode : uint8_t
    {
        ANGLE_RATE,
    };

    struct Yaw_Mode_Params
    {
        math::vec2f angle_rate;   //angle rate of change - radians per second
    };

    //the reference frame for the user controls
    enum class Reference_Frame : uint8_t
    {
        LOCAL, 	//normal mode - back means back of uav
        USER,	//simple mode - back means towards the user, front away from her.
    };

    struct Helpers
    {
        union
        {
            struct
            {
                uint8_t stay_in_range           : 1; //avoid out of range situations.
                uint8_t stay_in_battery_range   : 1; //avoid going too far considering current battery.
                uint8_t stay_in_perimeter       : 1; //stay in a configured perimeter.
                uint8_t avoid_altitude_drop     : 1; //avoid dropping too much altitude too fast.
                uint8_t avoid_the_user          : 1; //avoid getting too close to the launch position (the user).
                uint8_t avoid_proximity         : 1; //maintains a min distance from all objects around.
            };
            uint8_t _all = 0;
        };
    };

    ///////////////////////////////
    /// Data


    struct Value
    {
        q::Clock::time_point time_point; //when it was generated. This is written and read by the GS.

        Toggles toggles;
        Mode mode = Mode::IDLE;
        Vertical_Mode vertical_mode = Vertical_Mode::THRUST_OFFSET;
        Horizontal_Mode horizontal_mode = Horizontal_Mode::ANGLE;
        Yaw_Mode yaw_mode = Yaw_Mode::ANGLE_RATE;
        Reference_Frame reference_frame = Reference_Frame::LOCAL;
        Helpers assists;

        Vertical_Mode_Params vertical_mode_params;
        Horizontal_Mode_Params horizontal_mode_params;
        Yaw_Mode_Params yaw_mode_params;
    };

    typedef stream::Sample<Value>     Sample;

    virtual ~IMulti_Input() {}

    virtual auto get_samples() const -> std::vector<Sample> const& = 0;
};
DECLARE_CLASS_PTR(IMulti_Input);


}
}
}


