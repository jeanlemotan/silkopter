#pragma once

#include "IStream.h"

namespace silk
{
namespace node
{

class IAcceleration_Stream : public ISpatial_Stream<IStream::Type::ACCELERATION, Space::LOCAL>
{
public:
    typedef math::vec3f             Value; //meters per second^2
    typedef node::Sample<Value>     Sample;
    virtual auto get_samples() const -> std::vector<Sample> const& = 0;
};
DECLARE_CLASS_PTR(IAcceleration_Stream);

class IENU_Acceleration_Stream : public ISpatial_Stream<IStream::Type::ACCELERATION, Space::ENU>
{
public:
    typedef math::vec3f             Value; //meters per second^2
    typedef node::Sample<Value>     Sample;
    virtual auto get_samples() const -> std::vector<Sample> const& = 0;
};
DECLARE_CLASS_PTR(IENU_Acceleration_Stream);

class IECEF_Acceleration_Stream : public ISpatial_Stream<IStream::Type::ACCELERATION, Space::ECEF>
{
public:
    typedef math::vec3f             Value; //meters per second^2
    typedef node::Sample<Value>     Sample;
    virtual auto get_samples() const -> std::vector<Sample> const& = 0;
};
DECLARE_CLASS_PTR(IECEF_Acceleration_Stream);

}
}
