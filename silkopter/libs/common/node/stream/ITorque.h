#pragma once

#include "IStream.h"

namespace silk
{
namespace node
{
namespace stream
{

class ITorque : public ISpatial_Stream<Type::TORQUE, Space::LOCAL>
{
public:
    typedef math::vec3f             Value; //Nm
    typedef stream::Sample<Value>     Sample;
    virtual auto get_samples() const -> std::vector<Sample> const& = 0;
};
DECLARE_CLASS_PTR(ITorque);

class IENU_Torque : public ISpatial_Stream<Type::TORQUE, Space::ENU>
{
public:
    typedef math::vec3f             Value; //Nm
    typedef stream::Sample<Value>     Sample;
    virtual auto get_samples() const -> std::vector<Sample> const& = 0;
};
DECLARE_CLASS_PTR(IENU_Torque);

class IECEF_Torque : public ISpatial_Stream<Type::TORQUE, Space::ECEF>
{
public:
    typedef math::vec3f             Value; //Nm
    typedef stream::Sample<Value>     Sample;
    virtual auto get_samples() const -> std::vector<Sample> const& = 0;
};
DECLARE_CLASS_PTR(IECEF_Torque);

}
}
}
