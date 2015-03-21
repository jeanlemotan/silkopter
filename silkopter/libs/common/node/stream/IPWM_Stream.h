#pragma once

#include "IStream.h"

namespace silk
{
namespace node
{

class IPWM_Stream : public IScalar_Stream<IStream::Type::PWM>
{
public:
    typedef float                   Value; //0 .. 1 representing duty cycle
    typedef node::Sample<Value>     Sample;
    virtual auto get_samples() const -> std::vector<Sample> const& = 0;
};
DECLARE_CLASS_PTR(IPWM_Stream);


}
}
