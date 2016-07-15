#pragma once

#include "Stream_Base.h"
#include "utils/Butterworth.h"

namespace silk
{
namespace stream
{

template<Space SPACE_VALUE, Space PARENT_SPACE_VALUE>
class IFrameT : public ISpatial_Stream<Semantic::FRAME, SPACE_VALUE>
{
public:
    typedef std::false_type can_be_filtered_t;

    typedef math::quatf Value; //local to parent. vec local * rotation == vec parent
    typedef stream::Sample<Value>     Sample;
    virtual auto get_samples() const -> std::vector<Sample> const& = 0;

    static constexpr Space PARENT_SPACE = PARENT_SPACE_VALUE;
};


typedef IFrameT<Space::GIMBAL, Space::LOCAL>    IGimbal_Frame;
typedef IFrameT<Space::LOCAL, Space::ENU>       IFrame;
typedef IFrameT<Space::ENU, Space::ECEF>        IENU_Frame;


}
}
