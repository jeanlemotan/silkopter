#pragma once

#include "Stream_Base.h"

namespace silk
{
namespace stream
{

template<Space SPACE_VALUE>
class IAngular_VelocityT : public ISpatial_Stream<Semantic::ANGULAR_VELOCITY, SPACE_VALUE>
{
public:
    typedef std::true_type can_be_filtered_t;

    typedef math::vec3f       Value; //radians per second
    typedef stream::Sample<Value>     Sample;
    virtual auto get_samples() const -> std::vector<Sample> const& = 0;
};

typedef IAngular_VelocityT<Space::LOCAL>    IAngular_Velocity;
typedef IAngular_VelocityT<Space::ENU>      IENU_Angular_Velocity;
typedef IAngular_VelocityT<Space::ECEF>     IECEF_Angular_Velocity;


}
}
