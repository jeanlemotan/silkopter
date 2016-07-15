#pragma once

#include "Stream_Base.h"

namespace silk
{
namespace stream
{

template<Space SPACE_VALUE>
class ITorqueT : public ISpatial_Stream<Semantic::TORQUE, SPACE_VALUE>
{
public:
    typedef std::true_type can_be_filtered_t;

    typedef math::vec3f             Value; //Nm
    typedef stream::Sample<Value>     Sample;
    virtual auto get_samples() const -> std::vector<Sample> const& = 0;
};

typedef ITorqueT<Space::LOCAL>    ITorque;
typedef ITorqueT<Space::ENU>      IENU_Torque;
typedef ITorqueT<Space::ECEF>     IECEF_Torque;

}
}
