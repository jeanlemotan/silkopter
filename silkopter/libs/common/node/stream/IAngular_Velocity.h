#pragma once

#include "IStream.h"

namespace silk
{
namespace node
{
namespace stream
{

class IAngular_Velocity : public IStream
{
public:
    typedef math::vec3f       Value; //radians per second
    typedef stream::Sample<Value>     Sample;

    virtual ~IAngular_Velocity() {}

    virtual auto get_samples() const -> std::vector<Sample> const& = 0;

    //filter helpers
    static const size_t FILTER_CHANNELS = 3;
    typedef float FILTER_CHANNEL_TYPE;
    static void setup_channels(FILTER_CHANNEL_TYPE* channels[FILTER_CHANNELS], Value& value)
    {
        channels[0] = &value.x;
        channels[1] = &value.y;
        channels[2] = &value.z;
    }
    static void lerp(Value& out, Value& a, Value& b, float mu)
    {
        out = math::lerp(a, b, mu);
    }
};


}
}
}
