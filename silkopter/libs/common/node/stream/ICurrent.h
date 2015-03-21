#pragma once

#include "IStream.h"

namespace silk
{
namespace node
{
namespace stream
{

class ICurrent : public IScalar_Stream<Type::CURRENT>
{
public:
    typedef float             Value; //amperes
    typedef stream::Sample<Value>     Sample;
    virtual auto get_samples() const -> std::vector<Sample> const& = 0;
};
DECLARE_CLASS_PTR(ICurrent);


}
}
}
