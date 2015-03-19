#pragma once

#include "IStream.h"

namespace silk
{
namespace node
{
namespace stream
{

class IForce : public IStream
{
    DEFINE_RTTI_CLASS(IForce, IStream);
public:
    typedef float             Value; //N
    typedef stream::Sample<Value>     Sample;

    virtual ~IForce() {}

    virtual auto get_samples() const -> std::vector<Sample> const& = 0;
};
DECLARE_CLASS_PTR(IForce);


}
}
}
