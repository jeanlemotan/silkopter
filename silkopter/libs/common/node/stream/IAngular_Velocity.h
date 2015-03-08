#pragma once

#include "IStream.h"

namespace silk
{
namespace node
{
namespace stream
{

class IAngular_Velocity : public IStream
{
    DEFINE_RTTI_CLASS(IAngular_Velocity, IStream);
public:
    typedef math::vec3f       Value; //radians per second
    typedef stream::Sample<Value>     Sample;

    virtual ~IAngular_Velocity() {}

    virtual auto get_samples() const -> std::vector<Sample> const& = 0;
};
DECLARE_CLASS_PTR(IAngular_Velocity);


}
}
}
