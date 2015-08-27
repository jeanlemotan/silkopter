#pragma once

#include "Stream_Base.h"

namespace silk
{
namespace stream
{

class ITorque : public ISpatial_Stream<Type::TORQUE, Space::LOCAL>
{
public:
    typedef std::true_type can_be_filtered_t;

    typedef math::vec3d             Value; //Nm
    typedef stream::Sample<Value>     Sample;
    virtual auto get_samples() const -> std::vector<Sample> const& = 0;
};
DECLARE_CLASS_PTR(ITorque);

class IENU_Torque : public ISpatial_Stream<Type::TORQUE, Space::ENU>
{
public:
    typedef std::true_type can_be_filtered_t;

    typedef math::vec3d             Value; //Nm
    typedef stream::Sample<Value>     Sample;
    virtual auto get_samples() const -> std::vector<Sample> const& = 0;
};
DECLARE_CLASS_PTR(IENU_Torque);

class IECEF_Torque : public ISpatial_Stream<Type::TORQUE, Space::ECEF>
{
public:
    typedef std::true_type can_be_filtered_t;

    typedef math::vec3d             Value; //Nm
    typedef stream::Sample<Value>     Sample;
    virtual auto get_samples() const -> std::vector<Sample> const& = 0;
};
DECLARE_CLASS_PTR(IECEF_Torque);

}
}
