#pragma once

#include "IStream.h"

namespace silk
{
namespace node
{
namespace stream
{

class IPressure : public IStream
{
public:
    typedef float             Value; //??
    typedef Sample<Value>     Sample;

    virtual ~IPressure() {}

    virtual auto get_samples() const -> std::vector<Sample> const& = 0;
};


}
}
}
