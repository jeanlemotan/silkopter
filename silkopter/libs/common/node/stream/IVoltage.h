#pragma once

#include "IStream.h"

namespace silk
{
namespace node
{
namespace stream
{

class IVoltage : public IStream
{
    DEFINE_RTTI_CLASS(IVoltage, IStream);
public:
    typedef float             Value; //volts
    typedef stream::Sample<Value>     Sample;

    virtual ~IVoltage() {}

    virtual auto get_samples() const -> std::vector<Sample> const& = 0;
};
DECLARE_CLASS_PTR(IVoltage);


}
}
}
