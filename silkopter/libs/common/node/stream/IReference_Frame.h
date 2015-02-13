#pragma once

#include "IStream.h"
#include "Sample.h"

namespace silk
{
namespace node
{
namespace stream
{

class IReference_Frame : public IStream
{
public:
    typedef math::vec3f       Value; //meters per second^2
    typedef Sample<Value>     Sample;

    virtual ~IReference_Frame() {}

    virtual auto get_samples() const -> std::vector<Sample> const& = 0;
};

DECLARE_CLASS_PTR(IReference_Frame);

}
}
}

