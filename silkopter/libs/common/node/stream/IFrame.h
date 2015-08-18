#pragma once

#include "Stream_Base.h"
#include "utils/Butterworth.h"

namespace silk
{
namespace node
{
namespace stream
{


class IFrame : public ISpatial_Stream<Type::FRAME, Space::LOCAL>
{
public:
    typedef std::false_type can_be_filtered_t;

    typedef math::quatf Value; //local to parent. vec local * rotation == vec parent
    typedef stream::Sample<Value>     Sample;
    virtual auto get_samples() const -> std::vector<Sample> const& = 0;

    static constexpr Space PARENT_SPACE = Space::ENU;
};
DECLARE_CLASS_PTR(IFrame);

//class IENU_Frame : public ISpatial_Stream<Type::FRAME, Space::ENU>
//{
//public:
//    typedef std::false_type can_be_filtered_t;

//    struct Value
//    {
//        math::quatf rotation;        //local to parent. vec local * rotation == vec parent
//        math::vec3d ecef_origin;     //this ENU frame is tangent to this ecef point
//    };

//    typedef stream::Sample<Value>     Sample;
//    virtual auto get_samples() const -> std::vector<Sample> const& = 0;

//    static constexpr Space PARENT_SPACE = Space::ECEF;
//};
//DECLARE_CLASS_PTR(IENU_Frame);


}
}
}
