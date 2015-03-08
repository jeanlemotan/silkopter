#pragma once

#include "IStream.h"
#include "utils/Butterworth.h"

namespace silk
{
namespace node
{
namespace stream
{

class IReference_Frame : public IStream
{
    DEFINE_RTTI_CLASS(IReference_Frame, IStream);
public:
    struct Value
    {
        math::quatf local_to_world;
    };
    typedef stream::Sample<Value>     Sample;

    virtual ~IReference_Frame() {}

    virtual auto get_samples() const -> std::vector<Sample> const& = 0;
};
DECLARE_CLASS_PTR(IReference_Frame);


}
}
}

namespace util
{
namespace dsp
{
template<> inline bool equals(silk::node::stream::IReference_Frame::Value const& a, silk::node::stream::IReference_Frame::Value const& b)
{
    return math::equals(a.local_to_world, b.local_to_world);
}
template<> inline silk::node::stream::IReference_Frame::Value add(silk::node::stream::IReference_Frame::Value const& a, silk::node::stream::IReference_Frame::Value const& b)
{
    silk::node::stream::IReference_Frame::Value r;
    r.local_to_world = a.local_to_world * b.local_to_world;
    return r;
}
template<> inline silk::node::stream::IReference_Frame::Value scale(silk::node::stream::IReference_Frame::Value const& a, double scale)
{
    silk::node::stream::IReference_Frame::Value r;
    r.local_to_world = math::slerp(math::quatf(), a.local_to_world, scale);
    return r;
}
}
}
