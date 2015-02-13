#pragma once

#include "IStream.h"
#include "Sample.h"

namespace silk
{
namespace node
{
namespace stream
{

class IMagnetic_Field : public IStream
{
public:
    typedef math::vec3f       Value; //??
    typedef Sample<Value>     Sample;

    virtual ~IMagnetic_Field() {}

    virtual auto get_samples() const -> std::vector<Sample> const& = 0;
};

DECLARE_CLASS_PTR(IMagnetic_Field);

}
}
}
