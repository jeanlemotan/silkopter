#pragma once

#include "IStream.h"
#include "ICurrent_Stream.h"
#include "IVoltage_Stream.h"
#include "utils/Butterworth.h"

namespace silk
{
namespace node
{

class IBattery_State_Stream : public IScalar_Stream<IStream::Type::BATTERY_STATE>
{
public:
    struct Value
    {
        float charge_used = 0; //Amperes-Hour (Ah)
        IVoltage_Stream::Value average_voltage = 0; //Volts, averaged over one second
        ICurrent_Stream::Value average_current = 0; //Amperes, averaged over one second
        float capacity_left = 0; //0 is Empty, 1 is Full
    };
    typedef node::Sample<Value>     Sample;
    virtual auto get_samples() const -> std::vector<Sample> const& = 0;
};
DECLARE_CLASS_PTR(IBattery_State_Stream);


}
}


namespace util
{
namespace dsp
{
template<> inline bool equals(silk::node::IBattery_State_Stream::Value const& a, silk::node::IBattery_State_Stream::Value const& b)
{
    return math::equals(a.charge_used, b.charge_used) &&
           math::equals(a.average_voltage, b.average_voltage) &&
           math::equals(a.average_current, b.average_current) &&
           math::equals(a.capacity_left, b.capacity_left);
}
template<> inline silk::node::IBattery_State_Stream::Value add(silk::node::IBattery_State_Stream::Value const& a, silk::node::IBattery_State_Stream::Value const& b)
{
    silk::node::IBattery_State_Stream::Value r = a;
    r.charge_used += b.charge_used;
    r.average_voltage += b.average_voltage;
    r.average_current += b.average_current;
    r.capacity_left += b.capacity_left;
    return r;
}
template<> inline silk::node::IBattery_State_Stream::Value scale(silk::node::IBattery_State_Stream::Value const& a, double scale)
{
    silk::node::IBattery_State_Stream::Value r = a;
    r.charge_used *= scale;
    r.average_voltage *= scale;
    r.average_current *= scale;
    r.capacity_left *= scale;
    return r;
}
}
}
