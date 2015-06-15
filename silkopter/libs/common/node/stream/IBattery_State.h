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

    typedef void Calibration_Data;
};
DECLARE_CLASS_PTR(IBattery_State);


}
}
}

