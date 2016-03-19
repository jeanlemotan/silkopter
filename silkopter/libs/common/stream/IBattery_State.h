#pragma once

#include "Stream_Base.h"
#include "ICurrent.h"
#include "IVoltage.h"

namespace silk
{
namespace stream
{

class IBattery_State : public IScalar_Stream<Semantic::BATTERY_STATE>
{
public:
    typedef std::false_type can_be_filtered_t;

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


}
}


namespace util
{
namespace serialization
{

template<> inline void serialize(Buffer_t& buffer, silk::stream::IBattery_State::Value const& value, size_t& off)
{
    serialize(buffer, value.charge_used, off);
    serialize(buffer, value.average_voltage, off);
    serialize(buffer, value.average_current, off);
    serialize(buffer, value.capacity_left, off);
}

template<> inline auto deserialize(Buffer_t const& buffer, silk::stream::IBattery_State::Value& value, size_t& off) -> bool
{
    return deserialize(buffer, value.charge_used, off) &&
            deserialize(buffer, value.average_voltage, off) &&
            deserialize(buffer, value.average_current, off) &&
            deserialize(buffer, value.capacity_left, off);
}


}
}
