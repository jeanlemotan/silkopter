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
    DEFINE_RTTI_CLASS(IPWM_Value, IStream);
public:
    typedef float             Value; //0 .. 1 representing duty cycle
    typedef stream::Sample<Value>     Sample;

    virtual ~IPWM_Value() {}

    virtual auto get_type() const -> std::string const&
    {
        return q::rtti::get_class_name<IPWM_Value>();
    }

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


}
}
}
