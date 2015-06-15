#pragma once

#include "Stream_Base.h"

namespace silk
{
namespace node
{
namespace stream
{

class IAngular_Velocity : public ISpatial_Stream<Type::ANGULAR_VELOCITY, Space::LOCAL, Bias_Scale_Calibration_Data<math::vec3f>>
{
public:
    typedef math::vec3f       Value; //radians per second
    typedef stream::Sample<Value>     Sample;
    virtual auto get_samples() const -> std::vector<Sample> const& = 0;
};
DECLARE_CLASS_PTR(IAngular_Velocity);

class IENU_Angular_Velocity : public ISpatial_Stream<Type::ANGULAR_VELOCITY, Space::ENU>
{
public:
    typedef math::vec3f       Value; //radians per second
    typedef stream::Sample<Value>     Sample;
    virtual auto get_samples() const -> std::vector<Sample> const& = 0;
};
DECLARE_CLASS_PTR(IENU_Angular_Velocity);

class IECEF_Angular_Velocity : public ISpatial_Stream<Type::ANGULAR_VELOCITY, Space::ECEF>
{
public:
    typedef math::vec3f       Value; //radians per second
    typedef stream::Sample<Value>     Sample;
    virtual auto get_samples() const -> std::vector<Sample> const& = 0;
};
DECLARE_CLASS_PTR(IECEF_Angular_Velocity);

}
}
}
