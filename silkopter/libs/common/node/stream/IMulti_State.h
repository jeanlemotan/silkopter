#pragma once

#include "Stream_Base.h"

#include "IFrame.h"
#include "ILinear_Acceleration.h"
#include "IMagnetic_Field.h"
#include "IBattery_State.h"
#include "IBattery_State.h"
#include "IPosition.h"
#include "IVelocity.h"
#include "IProximity.h"

#include "IMulti_Input.h"

namespace silk
{
namespace node
{
namespace stream
{

class IMulti_State : public IScalar_Stream<Type::MULTI_STATE>
{
public:
    typedef std::false_type can_be_filtered_t;

    struct Value
    {
        IFrame::Value frame;
        IBattery_State::Value battery_state;
        IMagnetic_Field::Value magnetic_field;
        IECEF_Linear_Acceleration::Value ecef_linear_acceleration;
        IECEF_Position::Value ecef_position;
        IECEF_Velocity::Value ecef_velocity;
        IProximity::Value proximity;

        IMulti_Input::Toggles toggles;
        IMulti_Input::Mode mode = IMulti_Input::Mode::IDLE;
        IMulti_Input::Throttle_Mode throttle_mode = IMulti_Input::Throttle_Mode::OFFSET;
        IMulti_Input::Pitch_Roll_Mode pitch_roll_mode = IMulti_Input::Pitch_Roll_Mode::HORIZONTAL;
        IMulti_Input::Yaw_Mode yaw_mode = IMulti_Input::Yaw_Mode::RATE;
        IMulti_Input::Reference_Frame reference_frame = IMulti_Input::Reference_Frame::LOCAL;
        IMulti_Input::Assists assists;
    };

    typedef stream::Sample<Value>     Sample;

    virtual ~IMulti_State() {}

    virtual auto get_samples() const -> std::vector<Sample> const& = 0;
};
DECLARE_CLASS_PTR(IMulti_State);


}
}
}

