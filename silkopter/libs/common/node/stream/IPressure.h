#pragma once

#include "Stream_Base.h"

namespace silk
{
namespace node
{
namespace stream
{

class IPressure : public IScalar_Stream<Type::PRESSURE>
{
public:
    typedef float                   Value; //??
    typedef stream::Sample<Value>     Sample;
    virtual auto get_samples() const -> std::vector<Sample> const& = 0;
};
DECLARE_CLASS_PTR(IPressure);


}
}
}
