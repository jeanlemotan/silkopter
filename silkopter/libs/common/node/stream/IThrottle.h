#pragma once

#include "IStream.h"

namespace silk
{
namespace node
{
namespace stream
{

class IThrottle : public IScalar_Stream<Type::THROTTLE>
{
public:
    typedef float                   Value; //0 .. 1
    typedef stream::Sample<Value>   Sample;
    virtual auto get_samples() const -> std::vector<Sample> const& = 0;

    typedef void Calibration_Data;
};
DECLARE_CLASS_PTR(IThrottle);


}
}
}
