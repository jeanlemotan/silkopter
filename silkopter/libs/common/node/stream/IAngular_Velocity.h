#pragma once

#include "IStream.h"
#include "Sample.h"

namespace silk
{
namespace node
{
namespace stream
{

class IAngular_Velocity : public IStream
{
public:
    typedef math::vec3f       Value; //radians per second
    typedef Sample<Value>     Sample;

    virtual ~IAngular_Velocity() {}

    virtual auto get_samples() const -> std::vector<Sample> const& = 0;
};

DECLARE_CLASS_PTR(IAngular_Velocity);

}
}
}
