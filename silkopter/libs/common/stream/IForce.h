#pragma once

#include "Stream_Base.h"

namespace silk
{
namespace stream
{

template<Space SPACE_VALUE>
class IForceT : public ISpatial_Stream<Semantic::FORCE, SPACE_VALUE>
{
public:
    typedef std::true_type can_be_filtered_t;

    typedef math::vec3f             Value; //N
    typedef stream::Sample<Value>     Sample;
    virtual auto get_samples() const -> std::vector<Sample> const& = 0;
};

typedef IForceT<Space::LOCAL>    IForce;
typedef IForceT<Space::ENU>      IENU_Force;
typedef IForceT<Space::ECEF>     IECEF_Force;


}
}
