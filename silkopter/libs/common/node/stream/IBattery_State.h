#pragma once

#include "IStream.h"
#include "ICurrent.h"
#include "IVoltage.h"

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

    virtual auto get_type() const -> std::string const&
    {
        return q::rtti::get_class_name<IBattery_State>();
    }

    virtual auto get_samples() const -> std::vector<Sample> const& = 0;
};


}
}
}

