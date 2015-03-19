#pragma once

#include "IStream.h"
#include "utils/Butterworth.h"

namespace silk
{
namespace node
{
namespace stream
{

class IENU_Frame : public IStream
{
    DEFINE_RTTI_CLASS(IENU_Frame, IStream);
public:
    struct Value
    {
        math::quatf ecef_to_enu;
        math::quatf enu_to_ecef;
    };
    typedef stream::Sample<Value>     Sample;

    virtual ~IENU_Frame() {}

    virtual auto get_samples() const -> std::vector<Sample> const& = 0;
};
DECLARE_CLASS_PTR(IENU_Frame);


}
}
}

namespace util
{
namespace dsp
{

template<> inline bool equals(silk::node::stream::IENU_Frame::Value const& a, silk::node::stream::IENU_Frame::Value const& b)
{
    return math::equals(a.enu_to_ecef, b.enu_to_ecef);
}
template<> inline silk::node::stream::IENU_Frame::Value add(silk::node::stream::IENU_Frame::Value const& a, silk::node::stream::IENU_Frame::Value const& b)
{
    silk::node::stream::IENU_Frame::Value r;
    r.enu_to_ecef = a.enu_to_ecef * b.enu_to_ecef;
    return r;
}
template<> inline silk::node::stream::IENU_Frame::Value scale(silk::node::stream::IENU_Frame::Value const& a, double scale)
{
    silk::node::stream::IENU_Frame::Value r;
    r.enu_to_ecef = math::slerp(math::quatf(), a.enu_to_ecef, scale);
    return r;
}
template<> inline void fix(silk::node::stream::IENU_Frame::Value& a)
{
    a.enu_to_ecef.normalize();
    a.ecef_to_enu = math::inverse(a.enu_to_ecef);
}

}
}
