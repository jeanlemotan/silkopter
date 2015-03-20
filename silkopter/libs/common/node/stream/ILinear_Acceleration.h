#pragma once

#include "IStream.h"

namespace silk
{
namespace node
{
namespace stream
{

class ILinear_Acceleration : public IStream
{
    DEFINE_RTTI_CLASS(ILinear_Acceleration, IStream);
public:
    typedef math::vec3f       Value; //meters per second^2
    typedef stream::Sample<Value>     Sample;
    virtual auto get_samples() const -> std::vector<Sample> const& = 0;
};
DECLARE_CLASS_PTR(ILinear_Acceleration);

class IENU_Linear_Acceleration : public IStream
{
    DEFINE_RTTI_CLASS(IENU_Linear_Acceleration, IStream);
public:
    typedef math::vec3f       Value; //meters per second^2
    typedef stream::Sample<Value>     Sample;
    virtual auto get_samples() const -> std::vector<Sample> const& = 0;
};
DECLARE_CLASS_PTR(IENU_Linear_Acceleration);

class IECEF_Linear_Acceleration : public IStream
{
    DEFINE_RTTI_CLASS(IECEF_Linear_Acceleration, IStream);
public:
    typedef math::vec3f       Value; //meters per second^2
    typedef stream::Sample<Value>     Sample;
    virtual auto get_samples() const -> std::vector<Sample> const& = 0;
};
DECLARE_CLASS_PTR(IECEF_Linear_Acceleration);


}
}
}
