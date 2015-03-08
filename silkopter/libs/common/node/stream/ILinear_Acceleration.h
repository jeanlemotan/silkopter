#pragma once

#include "IStream.h"

namespace silk
{
namespace node
{
namespace stream
{

class ILinear_Acceleration : public IStream
{
    DEFINE_RTTI_CLASS(ILinear_Acceleration, IStream);
public:
    typedef math::vec3f       Value; //meters per second^2
    typedef stream::Sample<Value>     Sample;

    virtual ~ILinear_Acceleration() {}

    virtual auto get_samples() const -> std::vector<Sample> const& = 0;
};
DECLARE_CLASS_PTR(ILinear_Acceleration);


}
}
}
