#pragma once

#include "IStream.h"

namespace silk
{
namespace node
{
namespace stream
{

class IPWM_Value : public IStream
{
public:
    typedef float             Value; //0 .. 1 representing duty cycle
    typedef Sample<Value>     Sample;

    virtual ~IPWM_Value() {}

    virtual auto get_samples() const -> std::vector<Sample> const& = 0;

    //filter helpers
    static const size_t FILTER_CHANNELS = 1;
    static void get_channels(double channels[FILTER_CHANNELS], Value const& value)
    {
        channels[0] = value;
    }
    static void get_value(Value& value, double const channels[FILTER_CHANNELS])
    {
        value = channels[0];
    }
};


}
}
}
