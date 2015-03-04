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
    DEFINE_RTTI_CLASS(ITemperature, IStream);
public:
    typedef float             Value; //degrees celsius
    typedef stream::Sample<Value>     Sample;

    virtual ~ITemperature() {}

    virtual auto get_type() const -> q::rtti::class_id { return q::rtti::get_class_id<ITemperature>(); }

    virtual auto get_samples() const -> std::vector<Sample> const& = 0;

    //filter helpers
    static const size_t FILTER_CHANNELS = 1;
    static auto get_channels_from_value(std::array<double, FILTER_CHANNELS>& channels, Value const& value) -> bool
    {
        channels[0] = value;
        return true;
    }
    static void get_value_from_channels(Value& value, std::array<double, FILTER_CHANNELS> const& channels)
    {
        value = channels[0];
    }
};
DECLARE_CLASS_PTR(ITemperature);


}
}
}
