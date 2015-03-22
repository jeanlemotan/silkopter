#pragma once

#include "IStream.h"

namespace silk
{
namespace node
{
namespace stream
{

class IFactor : public IScalar_Stream<Type::FACTOR>
{
public:
    typedef float                       Value;
    typedef stream::Sample<Value>       Sample;
    virtual auto get_samples() const -> std::vector<Sample> const& = 0;
};
DECLARE_CLASS_PTR(IFactor);


}
}
}
