#pragma once

#include "IStream.h"

namespace silk
{
namespace node
{
namespace stream
{

class ICurrent : public IStream
{
    DEFINE_RTTI_CLASS(ICurrent, IStream);
public:
    typedef float             Value; //amperes
    typedef stream::Sample<Value>     Sample;

    virtual ~ICurrent() {}

    virtual auto get_samples() const -> std::vector<Sample> const& = 0;
};
DECLARE_CLASS_PTR(ICurrent);


}
}
}
