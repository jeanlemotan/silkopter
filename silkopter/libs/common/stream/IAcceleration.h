#pragma once

#include "Stream_Base.h"

namespace silk
{
namespace stream
{

class IAcceleration : public ISpatial_Stream<Semantic::ACCELERATION, Space::UAV>
{
public:
    typedef std::true_type can_be_filtered_t;

    typedef math::vec3f             Value; //meters per second^2
    typedef stream::Sample<Value>     Sample;
    virtual auto get_samples() const -> std::vector<Sample> const& = 0;
};

class IENU_Acceleration : public ISpatial_Stream<Semantic::ACCELERATION, Space::ENU>
{
public:
    typedef std::true_type can_be_filtered_t;

    typedef math::vec3f             Value; //meters per second^2
    typedef stream::Sample<Value>     Sample;
    virtual auto get_samples() const -> std::vector<Sample> const& = 0;
};

class IECEF_Acceleration : public ISpatial_Stream<Semantic::ACCELERATION, Space::ECEF>
{
public:
    typedef std::true_type can_be_filtered_t;

    typedef math::vec3f             Value; //meters per second^2
    typedef stream::Sample<Value>     Sample;
    virtual auto get_samples() const -> std::vector<Sample> const& = 0;
};

}
}
