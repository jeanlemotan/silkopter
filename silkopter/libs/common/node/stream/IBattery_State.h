#pragma once

#include "IStream.h"

namespace silk
{
namespace node
{
namespace stream
{

class IBattery_State : public IStream
{
public:
    struct Value
    {
        float charge_used = 0; //Amperes-Hour (Ah)
        float voltage = 0; //Volts
        float current = 0; //Amperes
        float capacity_left = 0; //0 is Empty, 1 is Full
    };
    typedef stream::Sample<Value>     Sample;

    virtual ~IBattery_State() {}

    virtual auto get_samples() const -> std::vector<Sample> const& = 0;
};


}
}
}

