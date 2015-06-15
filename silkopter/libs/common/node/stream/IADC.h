#pragma once

#include "IStream.h"

namespace silk
{
namespace node
{
namespace stream
{

class IADC : public IScalar_Stream<Type::ADC>
{
public:
    typedef float                   Value; //0 .. 1
    typedef stream::Sample<Value>     Sample;
    virtual auto get_samples() const -> std::vector<Sample> const& = 0;

    typedef stream::Calibration_Data<float> Calibration_Data;
    Calibration_Data calibration_data;
};
DECLARE_CLASS_PTR(IADC);


}
}
}
