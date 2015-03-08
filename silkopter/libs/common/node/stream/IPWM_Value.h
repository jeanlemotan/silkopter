#pragma once

#include "IStream.h"

namespace silk
{
namespace node
{
namespace stream
{

class IPWM_Value : public IStream
{
    DEFINE_RTTI_CLASS(IPWM_Value, IStream);
public:
    typedef float             Value; //0 .. 1 representing duty cycle
    typedef stream::Sample<Value>     Sample;

    virtual ~IPWM_Value() {}

    virtual auto get_samples() const -> std::vector<Sample> const& = 0;
};
DECLARE_CLASS_PTR(IPWM_Value);


}
}
}
