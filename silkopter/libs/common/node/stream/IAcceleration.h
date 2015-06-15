#pragma once

#include "Stream_Base.h"

namespace silk
{
namespace node
{
namespace stream
{

class IAcceleration : public ISpatial_Stream<Type::ACCELERATION, Space::LOCAL, Bias_Scale_Calibration_Data<math::vec3f>>
{
public:
    typedef math::vec3f             Value; //meters per second^2
    typedef stream::Sample<Value>     Sample;
    virtual auto get_samples() const -> std::vector<Sample> const& = 0;
};
DECLARE_CLASS_PTR(IAcceleration);

class IENU_Acceleration : public ISpatial_Stream<Type::ACCELERATION, Space::ENU>
{
public:
    typedef math::vec3f             Value; //meters per second^2
    typedef stream::Sample<Value>     Sample;
    virtual auto get_samples() const -> std::vector<Sample> const& = 0;

    typedef void Calibration_Data;
};
DECLARE_CLASS_PTR(IENU_Acceleration);

class IECEF_Acceleration : public ISpatial_Stream<Type::ACCELERATION, Space::ECEF>
{
public:
    typedef math::vec3f             Value; //meters per second^2
    typedef stream::Sample<Value>     Sample;
    virtual auto get_samples() const -> std::vector<Sample> const& = 0;

    typedef void Calibration_Data;
};
DECLARE_CLASS_PTR(IECEF_Acceleration);

}
}
}
