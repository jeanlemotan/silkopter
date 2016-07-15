#pragma once

#include "Stream_Base.h"

namespace silk
{
namespace stream
{

template<Space SPACE_VALUE>
class IDistanceT : public ISpatial_Stream<Semantic::DISTANCE, SPACE_VALUE>
{
public:
    typedef std::true_type can_be_filtered_t;

    typedef math::vec3f               Value; //meters
    typedef stream::Sample<Value>     Sample;
    virtual auto get_samples() const -> std::vector<Sample> const& = 0;
};

typedef IDistanceT<Space::LOCAL>    IDistance;
typedef IDistanceT<Space::ENU>      IENU_Distance;
typedef IDistanceT<Space::ECEF>     IECEF_Distance;


}
}
