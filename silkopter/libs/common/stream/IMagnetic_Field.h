#pragma once

#include "Stream_Base.h"

namespace silk
{
namespace stream
{

template<Space SPACE_VALUE>
class IMagnetic_FieldT : public ISpatial_Stream<Semantic::MAGNETIC_FIELD, SPACE_VALUE>
{
public:
    typedef std::true_type can_be_filtered_t;

    typedef math::vec3f       Value; //micro T(eslas)
    typedef stream::Sample<Value>     Sample;
    virtual auto get_samples() const -> std::vector<Sample> const& = 0;
};

typedef IMagnetic_FieldT<Space::LOCAL>    IMagnetic_Field;
typedef IMagnetic_FieldT<Space::ENU>      IENU_Magnetic_Field;
typedef IMagnetic_FieldT<Space::ECEF>     IECEF_Magnetic_Field;


}
}
