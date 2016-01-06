#pragma once

#include "Stream_Base.h"

#include "IFrame.h"
#include "ILinear_Acceleration.h"
#include "IBattery_State.h"
#include "IBattery_State.h"
#include "IPosition.h"
#include "IVelocity.h"
#include "IProximity.h"
#include "IFloat.h"
#include "IVideo.h"

#include "IMulti_Input.h"

namespace silk
{
namespace stream
{

class IMulti_State : public IScalar_Stream<Type::MULTI_STATE>
{
public:
    typedef std::false_type can_be_filtered_t;

    struct Value
    {
        IBattery_State::Sample battery_state;
        IFrame::Sample frame;
        IECEF_Linear_Acceleration::Sample linear_acceleration;
        IECEF_Position::Sample home_position;
        IECEF_Position::Sample position;
        IECEF_Velocity::Sample velocity;
        IProximity::Sample proximity;
        IFloat::Sample thrust;
        IVideo::Sample video;

        IMulti_Input::Sample input;
    };

    typedef stream::Sample<Value>     Sample;

    virtual ~IMulti_State() {}

    virtual auto get_samples() const -> std::vector<Sample> const& = 0;
};
DECLARE_CLASS_PTR(IMulti_State);


}
}



namespace util
{
namespace serialization
{

template<> inline void serialize(Buffer_t& buffer, silk::stream::IMulti_State::Value const& value, size_t& off)
{
    serialize(buffer, value.battery_state, off);
    serialize(buffer, value.frame, off);
    serialize(buffer, value.linear_acceleration, off);
    serialize(buffer, value.position, off);
    serialize(buffer, value.home_position, off);
    serialize(buffer, value.velocity, off);
    serialize(buffer, value.proximity, off);
    serialize(buffer, value.thrust, off);
    serialize(buffer, value.video, off);
    serialize(buffer, value.input, off);
}

template<> inline auto deserialize(Buffer_t const& buffer, silk::stream::IMulti_State::Value& value, size_t& off) -> bool
{
    return deserialize(buffer, value.battery_state, off) &&
            deserialize(buffer, value.frame, off) &&
            deserialize(buffer, value.linear_acceleration, off) &&
            deserialize(buffer, value.position, off) &&
            deserialize(buffer, value.home_position, off) &&
            deserialize(buffer, value.velocity, off) &&
            deserialize(buffer, value.proximity, off) &&
            deserialize(buffer, value.thrust, off) &&
            deserialize(buffer, value.video, off) &&
            deserialize(buffer, value.input, off);
}


}
}


