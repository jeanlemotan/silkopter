#pragma once

#include "IStream.h"

namespace silk
{
namespace node
{
namespace stream
{

class IVelocity : public IStream
{
    DEFINE_RTTI_CLASS(IVelocity, IStream);
public:
    typedef float             Value; //Nm
    typedef stream::Sample<Value>     Sample;
    virtual auto get_samples() const -> std::vector<Sample> const& = 0;
};
DECLARE_CLASS_PTR(IVelocity);

class IENU_Velocity : public IStream
{
    DEFINE_RTTI_CLASS(IENU_Velocity, IStream);
public:
    typedef float             Value; //Nm
    typedef stream::Sample<Value>     Sample;
    virtual auto get_samples() const -> std::vector<Sample> const& = 0;
};
DECLARE_CLASS_PTR(IENU_Velocity);

class IECEF_Velocity : public IStream
{
    DEFINE_RTTI_CLASS(IECEF_Velocity, IStream);
public:
    typedef float             Value; //Nm
    typedef stream::Sample<Value>     Sample;
    virtual auto get_samples() const -> std::vector<Sample> const& = 0;
};
DECLARE_CLASS_PTR(IECEF_Velocity);

}
}
}
