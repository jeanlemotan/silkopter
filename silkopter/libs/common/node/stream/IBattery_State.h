#pragma once

#include "IStream.h"
#include "ICurrent.h"
#include "IVoltage.h"
#include "utils/Butterworth.h"

namespace silk
{
namespace node
{
namespace stream
{

class IBattery_State : public IStream
{
    DEFINE_RTTI_CLASS(IBattery_State, IStream);
public:
    struct Value
    {
        float charge_used = 0; //Amperes-Hour (Ah)
        IVoltage::Value average_voltage = 0; //Volts, averaged over one second
        ICurrent::Value average_current = 0; //Amperes, averaged over one second
        float capacity_left = 0; //0 is Empty, 1 is Full
    };
    typedef stream::Sample<Value>     Sample;

    virtual ~IBattery_State() {}

    virtual auto get_samples() const -> std::vector<Sample> const& = 0;
};
DECLARE_CLASS_PTR(IBattery_State);


}
}
}

namespace util
{
namespace dsp
{
template<> inline bool equals(silk::node::stream::IBattery_State::Value const& a, silk::node::stream::IBattery_State::Value const& b)
{
    return math::equals(a.charge_used, b.charge_used) ||
           math::equals(a.average_voltage, b.average_voltage) ||
           math::equals(a.average_current, b.average_current) ||
           math::equals(a.capacity_left, b.capacity_left);
}
template<> inline silk::node::stream::IBattery_State::Value add(silk::node::stream::IBattery_State::Value const& a, silk::node::stream::IBattery_State::Value const& b)
{
    silk::node::stream::IBattery_State::Value r = a;
    r.charge_used += b.charge_used;
    r.average_voltage += b.average_voltage;
    r.average_current += b.average_current;
    r.capacity_left += b.capacity_left;
    return r;
}
template<> inline silk::node::stream::IBattery_State::Value scale(silk::node::stream::IBattery_State::Value const& a, double scale)
{
    silk::node::stream::IBattery_State::Value r = a;
    r.charge_used *= scale;
    r.average_voltage *= scale;
    r.average_current *= scale;
    r.capacity_left *= scale;
    return r;
}
}
}
