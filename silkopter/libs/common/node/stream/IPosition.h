#pragma once

#include "Stream_Base.h"
#include "utils/Butterworth.h"
#include "utils/Coordinates.h"

namespace silk
{
namespace node
{
namespace stream
{

class IECEF_Position : public ISpatial_Stream<Type::POSITION, Space::ECEF>
{
public:
    typedef std::true_type can_be_filtered_t;

    typedef util::coordinates::ECEF Value; //meters
    typedef stream::Sample<Value>     Sample;
    virtual auto get_samples() const -> std::vector<Sample> const& = 0;
};
DECLARE_CLASS_PTR(IECEF_Position);

class ILLA_Position : public ISpatial_Stream<Type::POSITION, Space::LLA>
{
public:
    typedef std::false_type can_be_filtered_t;

    typedef util::coordinates::LLA Value;
    typedef stream::Sample<Value>     Sample;
    virtual auto get_samples() const -> std::vector<Sample> const& = 0;
};
DECLARE_CLASS_PTR(ILLA_Position);

}
}
}



namespace util
{
namespace dsp
{
template<> inline void apply_coefficients(silk::node::stream::IECEF_Position::Value& x,
                                   silk::node::stream::IECEF_Position::Value& w0,
                                   silk::node::stream::IECEF_Position::Value& w1,
                                   silk::node::stream::IECEF_Position::Value& w2, float d1, float d2, float A)
{
    w0 = double(d1)*w1 + double(d2)*w2 + x;
    x = double(A)*(w0 + 2.0*w1 + w2);
    w2 = w1;
    w1 = w0;
}

}
}





