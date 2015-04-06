#pragma once

#include "IStream.h"
#include "utils/Butterworth.h"

namespace silk
{
namespace node
{
namespace stream
{

class IFrame_Base
{
public:
    struct Value
    {
        math::quatf rotation; //local to parent. vec local * rotation == vec parent
    };
    typedef stream::Sample<Value>     Sample;
    virtual auto get_samples() const -> std::vector<Sample> const& = 0;
};

class IFrame : public ISpatial_Stream<Type::FRAME, Space::LOCAL>, public IFrame_Base
{
public:
    static constexpr Space PARENT_SPACE = Space::ENU;
};
DECLARE_CLASS_PTR(IFrame);

class IENU_Frame : public ISpatial_Stream<Type::FRAME, Space::ENU>, public IFrame_Base
{
public:
    static constexpr Space PARENT_SPACE = Space::ECEF;
};
DECLARE_CLASS_PTR(IENU_Frame);


}
}
}


namespace util
{
namespace dsp
{

template<> inline bool equals(silk::node::stream::IFrame_Base::Value const& a, silk::node::stream::IFrame_Base::Value const& b)
{
    return math::equals(a.rotation, b.rotation);
}
template<> inline silk::node::stream::IFrame_Base::Value add(silk::node::stream::IFrame_Base::Value const& a, silk::node::stream::IFrame_Base::Value const& b)
{
    silk::node::stream::IFrame_Base::Value r;
    r.rotation = a.rotation * b.rotation;
    return r;
}
template<> inline silk::node::stream::IFrame_Base::Value scale(silk::node::stream::IFrame_Base::Value const& a, double scale)
{
    silk::node::stream::IFrame_Base::Value r;
    r.rotation = math::slerp(math::quatf(), a.rotation, scale);
    return r;
}
template<> inline void fix(silk::node::stream::IFrame_Base::Value& a)
{
    a.rotation.normalize<math::safe>();
}

}
}
