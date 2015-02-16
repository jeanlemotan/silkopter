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
        float ah_used = 0;
        float voltage = 0;
        float capacity_left = 0;
        boost::optional<uint8_t> cells;
    };
    typedef stream::Sample<Value>     Sample;

    virtual ~IBattery_State() {}

    virtual auto get_samples() const -> std::vector<Sample> const& = 0;
};


}
}
}

