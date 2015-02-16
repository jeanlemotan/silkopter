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
    typedef stream::Sample<Value>     Sample;

    virtual ~IPWM_Value() {}

    virtual auto get_samples() const -> std::vector<Sample> const& = 0;

    //filter helpers
    static const size_t FILTER_CHANNELS = 1;
    typedef float FILTER_CHANNEL_TYPE;
    static void setup_channels(FILTER_CHANNEL_TYPE* channels[FILTER_CHANNELS], Value& value)
    {
        channels[0] = &value;
    }
};


}
}
}
