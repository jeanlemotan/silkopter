#pragma once

#include "Stream_Base.h"

namespace silk
{
namespace stream
{

class IDistance : public ISpatial_Stream<Semantic::DISTANCE, Space::UAV>
{
public:
    typedef std::true_type can_be_filtered_t;

    typedef math::vec3f               Value; //meters
    typedef stream::Sample<Value>     Sample;
    virtual auto get_samples() const -> std::vector<Sample> const& = 0;
};

class IENU_Distance : public ISpatial_Stream<Semantic::DISTANCE, Space::ENU>
{
public:
    typedef std::true_type can_be_filtered_t;

    typedef math::vec3f               Value; //meters
    typedef stream::Sample<Value>     Sample;
    virtual auto get_samples() const -> std::vector<Sample> const& = 0;
};

class IECEF_Distance : public ISpatial_Stream<Semantic::DISTANCE, Space::ECEF>
{
public:
    typedef std::true_type can_be_filtered_t;

    typedef math::vec3f               Value; //meters
    typedef stream::Sample<Value>     Sample;
    virtual auto get_samples() const -> std::vector<Sample> const& = 0;
};

}
}
