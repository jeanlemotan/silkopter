#pragma once

#include "Stream_Base.h"

namespace silk
{
namespace node
{
namespace stream
{

class IVelocity : public ISpatial_Stream<Type::VELOCITY, Space::LOCAL>
{
public:
    typedef std::true_type can_be_filtered_t;

    typedef math::vec3f             Value; //m/s
    typedef stream::Sample<Value>     Sample;
    virtual auto get_samples() const -> std::vector<Sample> const& = 0;
};
DECLARE_CLASS_PTR(IVelocity);

class IENU_Velocity : public ISpatial_Stream<Type::VELOCITY, Space::ENU>
{
public:
    typedef std::true_type can_be_filtered_t;

    typedef math::vec3f             Value; //m/s
    typedef stream::Sample<Value>     Sample;
    virtual auto get_samples() const -> std::vector<Sample> const& = 0;
};
DECLARE_CLASS_PTR(IENU_Velocity);

class IECEF_Velocity : public ISpatial_Stream<Type::VELOCITY, Space::ECEF>
{
public:
    typedef std::true_type can_be_filtered_t;

    typedef math::vec3f             Value; //m/s
    typedef stream::Sample<Value>     Sample;
    virtual auto get_samples() const -> std::vector<Sample> const& = 0;
};
DECLARE_CLASS_PTR(IECEF_Velocity);

}
}
}
