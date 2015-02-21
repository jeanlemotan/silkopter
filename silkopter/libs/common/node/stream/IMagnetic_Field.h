#pragma once

#include "IStream.h"

namespace silk
{
namespace node
{
namespace stream
{

class IMagnetic_Field : public IStream
{
public:
    typedef math::vec3f       Value; //??
    typedef stream::Sample<Value>     Sample;

    virtual ~IMagnetic_Field() {}

    virtual auto get_samples() const -> std::vector<Sample> const& = 0;

    //filter helpers
    static const size_t FILTER_CHANNELS = 3;
    static auto get_channels_from_value(std::array<double, FILTER_CHANNELS>& channels, Value const& value) -> bool
    {
        channels[0] = value.x;
        channels[1] = value.y;
        channels[2] = value.z;
        return true;
    }
    static void get_value_from_channels(Value& value, std::array<double, FILTER_CHANNELS> const& channels)
    {
        value.x = channels[0];
        value.y = channels[1];
        value.z = channels[2];
    }
};


}
}
}
