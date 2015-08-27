#pragma once

#include "Stream_Base.h"

namespace silk
{
namespace stream
{

class IADC : public IScalar_Stream<Type::ADC>
{
public:
    typedef std::true_type can_be_filtered_t;

    typedef double                   Value; //0 .. 1
    typedef stream::Sample<Value>     Sample;
    virtual auto get_samples() const -> std::vector<Sample> const& = 0;
};
DECLARE_CLASS_PTR(IADC);


}
}
