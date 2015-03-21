#pragma once

#include "IStream.h"

namespace silk
{
namespace node
{

class ICurrent_Stream : public IScalar_Stream<IStream::Type::CURRENT>
{
public:
    typedef float             Value; //amperes
    typedef node::Sample<Value>     Sample;
    virtual auto get_samples() const -> std::vector<Sample> const& = 0;
};
DECLARE_CLASS_PTR(ICurrent_Stream);


}
}
