#pragma once

#include "IStream.h"

namespace silk
{
namespace node
{
namespace stream
{

class ITemperature : public IStream
{
public:
    typedef float             Value; //degrees celsius
    typedef stream::Sample<Value>     Sample;

    virtual ~ITemperature() {}

    virtual auto get_samples() const -> std::vector<Sample> const& = 0;

    //filter helpers
    static const size_t FILTER_CHANNELS = 1;
    typedef float FILTER_CHANNEL_TYPE;
    static void setup_channels(FILTER_CHANNEL_TYPE* channels[FILTER_CHANNELS], Value& value)
    {
        channels[0] = &value;
    }
    static void lerp(Value& out, Value& a, Value& b, float mu)
    {
        out = math::lerp(a, b, mu);
    }
};


}
}
}
