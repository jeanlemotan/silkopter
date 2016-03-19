#pragma once

#include "Stream_Base.h"

namespace silk
{
namespace stream
{

class IMagnetic_Field : public ISpatial_Stream<Semantic::MAGNETIC_FIELD, Space::UAV>
{
public:
    typedef std::true_type can_be_filtered_t;

    typedef math::vec3f       Value; //micro T(eslas)
    typedef stream::Sample<Value>     Sample;
    virtual auto get_samples() const -> std::vector<Sample> const& = 0;
};

class IENU_Magnetic_Field : public ISpatial_Stream<Semantic::MAGNETIC_FIELD, Space::ENU>
{
public:
    typedef std::true_type can_be_filtered_t;

    typedef math::vec3f       Value; //micro T(eslas)
    typedef stream::Sample<Value>     Sample;
    virtual auto get_samples() const -> std::vector<Sample> const& = 0;
};

class IECEF_Magnetic_Field : public ISpatial_Stream<Semantic::MAGNETIC_FIELD, Space::ECEF>
{
public:
    typedef std::true_type can_be_filtered_t;

    typedef math::vec3f       Value; //micro T(eslas)
    typedef stream::Sample<Value>     Sample;
    virtual auto get_samples() const -> std::vector<Sample> const& = 0;
};


}
}
