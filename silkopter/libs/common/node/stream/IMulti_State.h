#pragma once

#include "Stream_Base.h"

#include "IFrame.h"
#include "ILinear_Acceleration.h"
#include "IBattery_State.h"
#include "IBattery_State.h"
#include "IPosition.h"
#include "IVelocity.h"
#include "IProximity.h"
#include "IForce.h"

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
        IBattery_State::Value battery_state;
        IFrame::Value frame;
        IECEF_Linear_Acceleration::Value ecef_linear_acceleration;
        IECEF_Position::Value ecef_home_position;
        IECEF_Position::Value ecef_position;
        IECEF_Velocity::Value ecef_velocity;
        IProximity::Value proximity;
        IForce::Value thrust;

        IMulti_Input::Value last_input;
    };

    typedef stream::Sample<Value>     Sample;

    virtual ~IMulti_State() {}

    virtual auto get_samples() const -> std::vector<Sample> const& = 0;
};
DECLARE_CLASS_PTR(IMulti_State);


}
}
}



namespace util
{
namespace serialization
{

template<> inline void serialize(Buffer_t& buffer, silk::node::stream::IMulti_State::Value const& value, size_t& off)
{
    serialize(buffer, value.battery_state, off);
    serialize(buffer, value.frame, off);
    serialize(buffer, value.ecef_linear_acceleration, off);
    serialize(buffer, value.ecef_position, off);
    serialize(buffer, value.ecef_home_position, off);
    serialize(buffer, value.ecef_velocity, off);
    serialize(buffer, value.proximity, off);
    serialize(buffer, value.thrust, off);
    serialize(buffer, value.last_input, off);
}

template<> inline auto deserialize(Buffer_t const& buffer, silk::node::stream::IMulti_State::Value& value, size_t& off) -> bool
{
    return deserialize(buffer, value.battery_state, off) &&
            deserialize(buffer, value.frame, off) &&
            deserialize(buffer, value.ecef_linear_acceleration, off) &&
            deserialize(buffer, value.ecef_position, off) &&
            deserialize(buffer, value.ecef_home_position, off) &&
            deserialize(buffer, value.ecef_velocity, off) &&
            deserialize(buffer, value.proximity, off) &&
            deserialize(buffer, value.thrust, off) &&
            deserialize(buffer, value.last_input, off);
}


}
}


