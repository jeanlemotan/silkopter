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
    typedef Sample<Value>     Sample;

    virtual ~IAngular_Velocity() {}

    virtual auto get_samples() const -> std::vector<Sample> const& = 0;

    //filter helpers
    static const size_t FILTER_CHANNELS = 3;
    static void get_channels(double channels[FILTER_CHANNELS], Value const& value)
    {
        channels[0] = value.x;
        channels[1] = value.y;
        channels[2] = value.z;
    }
    static void get_value(Value& value, double const channels[FILTER_CHANNELS])
    {
        value.x = channels[0];
        value.y = channels[1];
        value.z = channels[2];
    }
};


}
}
}
