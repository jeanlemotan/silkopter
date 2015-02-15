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
public:
    typedef float             Value;
    typedef Sample<Value>     Sample;

    virtual ~IADC_Value() {}

    virtual auto get_samples() const -> std::vector<Sample> const& = 0;
};


}
}
}
