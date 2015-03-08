#pragma once

#include "IStream.h"

namespace silk
{
namespace node
{
namespace stream
{

class IADC_Value : public IStream
{
    DEFINE_RTTI_CLASS(IADC_Value, IStream);
public:
    typedef float             Value; //0 .. 1
    typedef stream::Sample<Value>     Sample;

    virtual ~IADC_Value() {}

    virtual auto get_samples() const -> std::vector<Sample> const& = 0;
};
DECLARE_CLASS_PTR(IADC_Value);


}
}
}
