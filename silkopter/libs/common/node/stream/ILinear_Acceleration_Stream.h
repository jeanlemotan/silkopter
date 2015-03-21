#pragma once

#include "IStream.h"

namespace silk
{
namespace node
{

class ILinear_Acceleration_Stream : public ISpatial_Stream<IStream::Type::LINEAR_ACCELERATION, Space::LOCAL>
{
public:
    typedef math::vec3f       Value; //meters per second^2
    typedef node::Sample<Value>     Sample;
    virtual auto get_samples() const -> std::vector<Sample> const& = 0;
};
DECLARE_CLASS_PTR(ILinear_Acceleration_Stream);

class IENU_Linear_Acceleration_Stream : public ISpatial_Stream<IStream::Type::LINEAR_ACCELERATION, Space::ENU>
{
public:
    typedef math::vec3f       Value; //meters per second^2
    typedef node::Sample<Value>     Sample;
    virtual auto get_samples() const -> std::vector<Sample> const& = 0;
};
DECLARE_CLASS_PTR(IENU_Linear_Acceleration_Stream);

class IECEF_Linear_Acceleration_Stream : public ISpatial_Stream<IStream::Type::LINEAR_ACCELERATION, Space::ECEF>
{
public:
    typedef math::vec3f       Value; //meters per second^2
    typedef node::Sample<Value>     Sample;
    virtual auto get_samples() const -> std::vector<Sample> const& = 0;
};
DECLARE_CLASS_PTR(IECEF_Linear_Acceleration_Stream);


}
}
