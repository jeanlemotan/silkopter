#pragma once

#include "IStream.h"
#include "Sample.h"

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
    typedef Sample<Value>     Sample;

    virtual ~ITemperature() {}

    virtual auto get_samples() const -> std::vector<Sample> const& = 0;
};

DECLARE_CLASS_PTR(ITemperature);

}
}
}
