#pragma once

#include "Stream_Base.h"

namespace silk
{
namespace node
{
namespace stream
{


class IVoltage : public IScalar_Stream<Type::VOLTAGE>
{
public:
    typedef std::true_type can_be_filtered_t;

    typedef float                   Value; //volts
    typedef stream::Sample<Value>     Sample;
    virtual auto get_samples() const -> std::vector<Sample> const& = 0;
};
DECLARE_CLASS_PTR(IVoltage);


}
}
}
