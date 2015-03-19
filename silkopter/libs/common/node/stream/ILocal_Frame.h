#pragma once

#include "IStream.h"
#include "utils/Butterworth.h"

namespace silk
{
namespace node
{
namespace stream
{

class ILocal_Frame : public IStream
{
    DEFINE_RTTI_CLASS(ILocal_Frame, IStream);
public:
    struct Value
    {
        math::quatf local_to_enu;
        math::quatf enu_to_local;
    };
    typedef stream::Sample<Value>     Sample;

    virtual ~ILocal_Frame() {}

    virtual auto get_samples() const -> std::vector<Sample> const& = 0;
};
DECLARE_CLASS_PTR(ILocal_Frame);


}
}
}

namespace util
{
namespace dsp
{

template<> inline bool equals(silk::node::stream::ILocal_Frame::Value const& a, silk::node::stream::ILocal_Frame::Value const& b)
{
    return math::equals(a.local_to_enu, b.local_to_enu);
}
template<> inline silk::node::stream::ILocal_Frame::Value add(silk::node::stream::ILocal_Frame::Value const& a, silk::node::stream::ILocal_Frame::Value const& b)
{
    silk::node::stream::ILocal_Frame::Value r;
    r.local_to_enu = a.local_to_enu * b.local_to_enu;
    return r;
}
template<> inline silk::node::stream::ILocal_Frame::Value scale(silk::node::stream::ILocal_Frame::Value const& a, double scale)
{
    silk::node::stream::ILocal_Frame::Value r;
    r.local_to_enu = math::slerp(math::quatf(), a.local_to_enu, scale);
    return r;
}
template<> inline void fix(silk::node::stream::ILocal_Frame::Value& a)
{
    a.local_to_enu.normalize();
    a.enu_to_local = math::inverse(a.local_to_enu);
}

}
}
