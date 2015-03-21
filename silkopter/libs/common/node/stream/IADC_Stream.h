#pragma once

#include "IStream.h"

namespace silk
{
namespace node
{

class IADC_Stream : public IScalar_Stream<IStream::Type::ADC>
{
public:
    typedef float                   Value; //0 .. 1
    typedef node::Sample<Value>     Sample;
    virtual auto get_samples() const -> std::vector<Sample> const& = 0;
};
DECLARE_CLASS_PTR(IADC_Stream);


}
}
