#pragma once

#include "IStream.h"
#include "utils/Butterworth.h"

namespace silk
{
namespace node
{
namespace stream
{

class IReference_Frame : public IStream
{
    DEFINE_RTTI_CLASS(IReference_Frame, IStream);
public:
    struct Value
    {
        IAngular_Velocity::Value angular_velocity;
        ILinear_Acceleration linear_acceleration;
        IBattery_State::Value battery_state;
        std::vector<IDistance::Value> distances;
        ILocation location;
        IReference_Frame reference_frame;
    };
    typedef stream::Sample<Value>     Sample;

    virtual ~IReference_Frame() {}

    virtual auto get_samples() const -> std::vector<Sample> const& = 0;
};
DECLARE_CLASS_PTR(IReference_Frame);


}
}
}
