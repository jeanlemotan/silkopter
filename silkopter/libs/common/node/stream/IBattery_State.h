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

class IBattery_State : public IScalar_Stream<Type::BATTERY_STATE>
{
public:
    struct Value
    {
        float charge_used = 0; //Amperes-Hour (Ah)
        IVoltage::Value average_voltage = 0; //Volts, averaged over one second
        ICurrent::Value average_current = 0; //Amperes, averaged over one second
        float capacity_left = 0; //0 is Empty, 1 is Full
    };
    typedef stream::Sample<Value>     Sample;
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
    return math::equals(a.charge_used, b.charge_used) &&
           math::equals(a.average_voltage, b.average_voltage) &&
           math::equals(a.average_current, b.average_current) &&
           math::equals(a.capacity_left, b.capacity_left);
}
template<> inline void apply_coefficients(silk::node::stream::IBattery_State::Value& x,
                                   silk::node::stream::IBattery_State::Value& w0,
                                   silk::node::stream::IBattery_State::Value& w1,
                                   silk::node::stream::IBattery_State::Value& w2, float d1, float d2, float A)
{
}

}
}
