#pragma once

#include "IStream.h"

namespace silk
{
namespace node
{
namespace stream
{

class IDistance : public IStream
{
    DEFINE_RTTI_CLASS(IDistance, IStream);
public:
    typedef math::vec3f               Value; //meters
    typedef stream::Sample<Value>     Sample;
    virtual auto get_samples() const -> std::vector<Sample> const& = 0;
};
DECLARE_CLASS_PTR(IDistance);

class IENU_Distance : public IStream
{
    DEFINE_RTTI_CLASS(IENU_Distance, IStream);
public:
    typedef math::vec3f               Value; //meters
    typedef stream::Sample<Value>     Sample;
    virtual auto get_samples() const -> std::vector<Sample> const& = 0;
};
DECLARE_CLASS_PTR(IENU_Distance);

class IECEF_Distance : public IStream
{
    DEFINE_RTTI_CLASS(IECEF_Distance, IStream);
public:
    typedef math::vec3f               Value; //meters
    typedef stream::Sample<Value>     Sample;
    virtual auto get_samples() const -> std::vector<Sample> const& = 0;
};
DECLARE_CLASS_PTR(IECEF_Distance);

}
}
}

