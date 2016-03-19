#pragma once

#include "Stream_Base.h"
#include "utils/Butterworth.h"

namespace silk
{
namespace stream
{

class IGimbal_Frame : public ISpatial_Stream<Semantic::FRAME, Space::GIMBAL>
{
public:
    typedef std::false_type can_be_filtered_t;

    typedef math::quatf Value; //local to parent. vec local * rotation == vec parent
    typedef stream::Sample<Value>     Sample;
    virtual auto get_samples() const -> std::vector<Sample> const& = 0;

    static constexpr Space PARENT_SPACE = Space::UAV;
};


class IUAV_Frame : public ISpatial_Stream<Semantic::FRAME, Space::UAV>
{
public:
    typedef std::false_type can_be_filtered_t;

    typedef math::quatf Value; //local to parent. vec local * rotation == vec parent
    typedef stream::Sample<Value>     Sample;
    virtual auto get_samples() const -> std::vector<Sample> const& = 0;

    static constexpr Space PARENT_SPACE = Space::ENU;
};

class IENU_Frame : public ISpatial_Stream<Semantic::FRAME, Space::ENU>
{
public:
    typedef std::false_type can_be_filtered_t;

    typedef math::quatf Value; //local to parent. vec local * rotation == vec parent
    typedef stream::Sample<Value>     Sample;
    virtual auto get_samples() const -> std::vector<Sample> const& = 0;

    static constexpr Space PARENT_SPACE = Space::ECEF;
};


}
}
