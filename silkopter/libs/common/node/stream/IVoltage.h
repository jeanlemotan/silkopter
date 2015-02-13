#pragma once

#include "IStream.h"
#include "Sample.h"

namespace silk
{
namespace node
{
namespace stream
{

class IVoltage : public IStream
{
public:
    typedef float             Value; //volts
    typedef Sample<Value>     Sample;

    virtual ~IVoltage() {}

    virtual auto get_samples() const -> std::vector<Sample> const& = 0;
};

DECLARE_CLASS_PTR(IVoltage);

}
}
}
