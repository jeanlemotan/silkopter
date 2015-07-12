#pragma once

#include "Stream_Base.h"

namespace silk
{
namespace node
{
namespace stream
{

class IForce : public ISpatial_Stream<Type::FORCE, Space::LOCAL>
{
public:
    typedef std::true_type can_be_filtered_t;

    typedef math::vec3f             Value; //N
    typedef stream::Sample<Value>     Sample;
    virtual auto get_samples() const -> std::vector<Sample> const& = 0;
};
DECLARE_CLASS_PTR(IForce);

class IENU_Force : public ISpatial_Stream<Type::FORCE, Space::ENU>
{
public:
    typedef std::true_type can_be_filtered_t;

    typedef math::vec3f             Value; //N
    typedef stream::Sample<Value>     Sample;
    virtual auto get_samples() const -> std::vector<Sample> const& = 0;
};
DECLARE_CLASS_PTR(IENU_Force);

class IECEF_Force : public ISpatial_Stream<Type::FORCE, Space::ECEF>
{
public:
    typedef std::true_type can_be_filtered_t;

    typedef math::vec3f             Value; //N
    typedef stream::Sample<Value>     Sample;
    virtual auto get_samples() const -> std::vector<Sample> const& = 0;
};
DECLARE_CLASS_PTR(IECEF_Force);

}
}
}
