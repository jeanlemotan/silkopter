#pragma once

#include "Stream_Base.h"

namespace silk
{
namespace stream
{

template<Space SPACE_VALUE>
class ILinear_AccelerationT : public ISpatial_Stream<Semantic::LINEAR_ACCELERATION, SPACE_VALUE>
{
public:
    typedef std::true_type can_be_filtered_t;

    typedef math::vec3f       Value; //meters per second^2
    typedef stream::Sample<Value>     Sample;
    virtual auto get_samples() const -> std::vector<Sample> const& = 0;
};

typedef ILinear_AccelerationT<Space::LOCAL>    ILinear_Acceleration;
typedef ILinear_AccelerationT<Space::ENU>      IENU_Linear_Acceleration;
typedef ILinear_AccelerationT<Space::ECEF>     IECEF_Linear_Acceleration;


}
}
