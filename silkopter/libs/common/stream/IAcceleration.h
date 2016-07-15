#pragma once

#include "Stream_Base.h"

namespace silk
{
namespace stream
{

template<Space SPACE_VALUE>
class IAccelerationT : public ISpatial_Stream<Semantic::ACCELERATION, SPACE_VALUE>
{
public:
    typedef std::true_type can_be_filtered_t;

    typedef math::vec3f             Value; //meters per second^2
    typedef stream::Sample<Value>     Sample;
    virtual auto get_samples() const -> std::vector<Sample> const& = 0;
};

typedef IAccelerationT<Space::LOCAL>    IAcceleration;
typedef IAccelerationT<Space::ENU>      IENU_Acceleration;
typedef IAccelerationT<Space::ECEF>     IECEF_Acceleration;

}
}
