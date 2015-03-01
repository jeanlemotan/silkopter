#pragma once

#include "IStream.h"

namespace silk
{
namespace node
{
namespace stream
{

class IAcceleration : public IStream
{
    DEFINE_RTTI_CLASS(IAcceleration, IStream);
public:
    typedef math::vec3f       Value; //meters per second^2
    typedef stream::Sample<Value>     Sample;

    virtual ~IAcceleration() {}

    virtual auto get_type() const -> q::rtti::class_id { return q::rtti::get_class_id<IAcceleration>(); }

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
