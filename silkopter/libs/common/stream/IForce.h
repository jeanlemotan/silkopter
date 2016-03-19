#pragma once

#include "Stream_Base.h"

namespace silk
{
namespace stream
{

class IForce : public ISpatial_Stream<Semantic::FORCE, Space::UAV>
{
public:
    typedef std::true_type can_be_filtered_t;

    typedef math::vec3f             Value; //N
    typedef stream::Sample<Value>     Sample;
    virtual auto get_samples() const -> std::vector<Sample> const& = 0;
};

class IENU_Force : public ISpatial_Stream<Semantic::FORCE, Space::ENU>
{
public:
    typedef std::true_type can_be_filtered_t;

    typedef math::vec3f             Value; //N
    typedef stream::Sample<Value>     Sample;
    virtual auto get_samples() const -> std::vector<Sample> const& = 0;
};

class IECEF_Force : public ISpatial_Stream<Semantic::FORCE, Space::ECEF>
{
public:
    typedef std::true_type can_be_filtered_t;

    typedef math::vec3f             Value; //N
    typedef stream::Sample<Value>     Sample;
    virtual auto get_samples() const -> std::vector<Sample> const& = 0;
};

}
}
