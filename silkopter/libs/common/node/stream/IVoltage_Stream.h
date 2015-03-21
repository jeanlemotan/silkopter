#pragma once

#include "IStream.h"

namespace silk
{
namespace node
{

class IVoltage_Stream : public IScalar_Stream<IStream::Type::VOLTAGE>
{
public:
    typedef float                   Value; //volts
    typedef node::Sample<Value>     Sample;
    virtual auto get_samples() const -> std::vector<Sample> const& = 0;
};
DECLARE_CLASS_PTR(IVoltage_Stream);


}
}
