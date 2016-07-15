#pragma once

#include "Stream_Base.h"

namespace silk
{
namespace stream
{

template<Space SPACE_VALUE>
class IVelocityT : public ISpatial_Stream<Semantic::VELOCITY, SPACE_VALUE>
{
public:
    typedef std::true_type can_be_filtered_t;

    typedef math::vec3f             Value; //m/s
    typedef stream::Sample<Value>     Sample;
    virtual auto get_samples() const -> std::vector<Sample> const& = 0;
};

typedef IVelocityT<Space::LOCAL>    IVelocity;
typedef IVelocityT<Space::ENU>      IENU_Velocity;
typedef IVelocityT<Space::ECEF>     IECEF_Velocity;


}
}
