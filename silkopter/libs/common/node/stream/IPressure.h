#pragma once

#include "IStream.h"

namespace silk
{
namespace node
{
namespace stream
{

class IPressure : public IStream
{
    DEFINE_RTTI_CLASS(IPressure, IStream);
public:
    typedef float             Value; //??
    typedef stream::Sample<Value>     Sample;

    virtual ~IPressure() {}

    virtual auto get_samples() const -> std::vector<Sample> const& = 0;
};
DECLARE_CLASS_PTR(IPressure);


}
}
}
