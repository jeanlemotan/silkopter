#pragma once

#include "IStream.h"

namespace silk
{
namespace node
{
namespace stream
{

class IDistance : public IStream
{
    DEFINE_RTTI_CLASS(IDistance, IStream);
public:
    typedef float             Value; //meters
    typedef stream::Sample<Value>     Sample;

    virtual ~IDistance() {}

    virtual auto get_samples() const -> std::vector<Sample> const& = 0;
};
DECLARE_CLASS_PTR(IDistance);


}
}
}

