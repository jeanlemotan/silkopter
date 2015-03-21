#pragma once

#include "IStream.h"

namespace silk
{
namespace node
{

class ITemperature_Stream : public IScalar_Stream<IStream::Type::TEMPERATURE>
{
public:
    typedef float                   Value; //degrees celsius
    typedef node::Sample<Value>     Sample;
    virtual auto get_samples() const -> std::vector<Sample> const& = 0;
};
DECLARE_CLASS_PTR(ITemperature_Stream);


}
}
