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

    typedef void Calibration_Data;
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
template<> inline void apply_coefficients(silk::node::stream::IFrame_Base::Value& x,
                                   silk::node::stream::IFrame_Base::Value& w0,
                                   silk::node::stream::IFrame_Base::Value& w1,
                                   silk::node::stream::IFrame_Base::Value& w2, float d1, float d2, float A)
{
}

}
}
