#pragma once

#include "IStream.h"

namespace silk
{
namespace node
{

class IPressure_Stream : public IScalar_Stream<IStream::Type::PRESSURE>
{
public:
    typedef float                   Value; //??
    typedef node::Sample<Value>     Sample;
    virtual auto get_samples() const -> std::vector<Sample> const& = 0;
};
DECLARE_CLASS_PTR(IPressure_Stream);


}
}
