#pragma once

#include "IStream.h"

namespace silk
{
namespace node
{

class IVelocity_Stream : public ISpatial_Stream<IStream::Type::VELOCITY, Space::LOCAL>
{
public:
    typedef float             Value; //Nm
    typedef node::Sample<Value>     Sample;
    virtual auto get_samples() const -> std::vector<Sample> const& = 0;
};
DECLARE_CLASS_PTR(IVelocity_Stream);

class IENU_Velocity_Stream : public ISpatial_Stream<IStream::Type::VELOCITY, Space::ENU>
{
public:
    typedef float             Value; //Nm
    typedef node::Sample<Value>     Sample;
    virtual auto get_samples() const -> std::vector<Sample> const& = 0;
};
DECLARE_CLASS_PTR(IENU_Velocity_Stream);

class IECEF_Velocity_Stream : public ISpatial_Stream<IStream::Type::VELOCITY, Space::ECEF>
{
public:
    typedef float             Value; //Nm
    typedef node::Sample<Value>     Sample;
    virtual auto get_samples() const -> std::vector<Sample> const& = 0;
};
DECLARE_CLASS_PTR(IECEF_Velocity_Stream);

}
}
