#pragma once

#include "IStream.h"

namespace silk
{
namespace node
{

class IAngular_Velocity_Stream : public ISpatial_Stream<IStream::Type::ANGULAR_VELOCITY, Space::LOCAL>
{
public:
    typedef math::vec3f       Value; //radians per second
    typedef node::Sample<Value>     Sample;
    virtual auto get_samples() const -> std::vector<Sample> const& = 0;
};
DECLARE_CLASS_PTR(IAngular_Velocity_Stream);

class IENU_Angular_Velocity_Stream : public ISpatial_Stream<IStream::Type::ANGULAR_VELOCITY, Space::ENU>
{
public:
    typedef math::vec3f       Value; //radians per second
    typedef node::Sample<Value>     Sample;
    virtual auto get_samples() const -> std::vector<Sample> const& = 0;
};
DECLARE_CLASS_PTR(IENU_Angular_Velocity_Stream);

class IECEF_Angular_Velocity_Stream : public ISpatial_Stream<IStream::Type::ANGULAR_VELOCITY, Space::ECEF>
{
public:
    typedef math::vec3f       Value; //radians per second
    typedef node::Sample<Value>     Sample;
    virtual auto get_samples() const -> std::vector<Sample> const& = 0;
};
DECLARE_CLASS_PTR(IECEF_Angular_Velocity_Stream);

}
}
