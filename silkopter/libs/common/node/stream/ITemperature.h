#pragma once

#include "IStream.h"

namespace silk
{
namespace node
{
namespace stream
{

class ITemperature : public IScalar_Stream<Type::TEMPERATURE>
{
public:
    typedef float                   Value; //degrees celsius
    typedef stream::Sample<Value>     Sample;
    virtual auto get_samples() const -> std::vector<Sample> const& = 0;
};
DECLARE_CLASS_PTR(ITemperature);


}
}
}