#pragma once

#include "Stream_Base.h"

namespace silk
{
namespace stream
{

class ITorque : public ISpatial_Stream<Semantic::TORQUE, Space::UAV>
{
public:
    typedef std::true_type can_be_filtered_t;

    typedef math::vec3f             Value; //Nm
    typedef stream::Sample<Value>     Sample;
    virtual auto get_samples() const -> std::vector<Sample> const& = 0;
};

class IENU_Torque : public ISpatial_Stream<Semantic::TORQUE, Space::ENU>
{
public:
    typedef std::true_type can_be_filtered_t;

    typedef math::vec3f             Value; //Nm
    typedef stream::Sample<Value>     Sample;
    virtual auto get_samples() const -> std::vector<Sample> const& = 0;
};

class IECEF_Torque : public ISpatial_Stream<Semantic::TORQUE, Space::ECEF>
{
public:
    typedef std::true_type can_be_filtered_t;

    typedef math::vec3f             Value; //Nm
    typedef stream::Sample<Value>     Sample;
    virtual auto get_samples() const -> std::vector<Sample> const& = 0;
};

}
}
