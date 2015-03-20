#pragma once

#include "IStream.h"
#include "utils/Butterworth.h"

namespace silk
{
namespace node
{
namespace stream
{

class IFrame : public IStream
{
    DEFINE_RTTI_CLASS(IFrame, IStream);
public:
    struct Value
    {
        math::quatf enu_to_local;
        math::quatf local_to_enu;
    };
    typedef stream::Sample<Value>     Sample;
    virtual auto get_samples() const -> std::vector<Sample> const& = 0;
};
DECLARE_CLASS_PTR(IFrame);

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

template<> inline bool equals(silk::node::stream::IFrame::Value const& a, silk::node::stream::IFrame::Value const& b)
{
    return math::equals(a.local_to_enu, b.local_to_enu);
}
template<> inline silk::node::stream::IFrame::Value add(silk::node::stream::IFrame::Value const& a, silk::node::stream::IFrame::Value const& b)
{
    silk::node::stream::IFrame::Value r;
    r.local_to_enu = a.local_to_enu * b.local_to_enu;
    return r;
}
template<> inline silk::node::stream::IFrame::Value scale(silk::node::stream::IFrame::Value const& a, double scale)
{
    silk::node::stream::IFrame::Value r;
    r.local_to_enu = math::slerp(math::quatf(), a.local_to_enu, scale);
    return r;
}
template<> inline void fix(silk::node::stream::IFrame::Value& a)
{
    a.local_to_enu.normalize<math::safe>();
    a.enu_to_local = math::inverse<float, math::safe>(a.local_to_enu);
}



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
    a.enu_to_ecef.normalize<math::safe>();
    a.ecef_to_enu = math::inverse<float, math::safe>(a.enu_to_ecef);
}

}
}
