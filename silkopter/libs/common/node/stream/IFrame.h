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
        math::quatf parent_to_this;
        math::quatf this_to_parent;
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
    return math::equals(a.this_to_parent, b.this_to_parent);
}
template<> inline silk::node::stream::IFrame_Base::Value add(silk::node::stream::IFrame_Base::Value const& a, silk::node::stream::IFrame_Base::Value const& b)
{
    silk::node::stream::IFrame_Base::Value r;
    r.this_to_parent = a.this_to_parent * b.this_to_parent;
    return r;
}
template<> inline silk::node::stream::IFrame_Base::Value scale(silk::node::stream::IFrame_Base::Value const& a, double scale)
{
    silk::node::stream::IFrame_Base::Value r;
    r.this_to_parent = math::slerp(math::quatf(), a.this_to_parent, scale);
    return r;
}
template<> inline void fix(silk::node::stream::IFrame_Base::Value& a)
{
    a.this_to_parent.normalize<math::safe>();
    a.parent_to_this = math::inverse<float, math::safe>(a.this_to_parent);
}

}
}
