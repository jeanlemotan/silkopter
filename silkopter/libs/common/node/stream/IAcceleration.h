#pragma once

#include "IStream.h"

namespace silk
{
namespace node
{
namespace stream
{

class IAcceleration : public IStream
{
    DEFINE_RTTI_CLASS(IAcceleration, IStream);
public:
    typedef math::vec3f       Value; //meters per second^2
    typedef stream::Sample<Value>     Sample;
    virtual auto get_samples() const -> std::vector<Sample> const& = 0;
};
DECLARE_CLASS_PTR(IAcceleration);

class IENU_Acceleration : public IStream
{
    DEFINE_RTTI_CLASS(IENU_Acceleration, IStream);
public:
    typedef math::vec3f       Value; //meters per second^2
    typedef stream::Sample<Value>     Sample;
    virtual auto get_samples() const -> std::vector<Sample> const& = 0;
};
DECLARE_CLASS_PTR(IENU_Acceleration);

class IECEF_Acceleration : public IStream
{
    DEFINE_RTTI_CLASS(IECEF_Acceleration, IStream);
public:
    typedef math::vec3f       Value; //meters per second^2
    typedef stream::Sample<Value>     Sample;
    virtual auto get_samples() const -> std::vector<Sample> const& = 0;
};
DECLARE_CLASS_PTR(IECEF_Acceleration);

}
}
}
