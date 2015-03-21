#pragma once

#include "IStream.h"

namespace silk
{
namespace node
{

class ITorque_Stream : public ISpatial_Stream<IStream::Type::TORQUE, Space::LOCAL>
{
public:
    typedef math::vec3f             Value; //Nm
    typedef node::Sample<Value>     Sample;
    virtual auto get_samples() const -> std::vector<Sample> const& = 0;
};
DECLARE_CLASS_PTR(ITorque_Stream);

class IENU_Torque_Stream : public ISpatial_Stream<IStream::Type::TORQUE, Space::ENU>
{
public:
    typedef math::vec3f             Value; //Nm
    typedef node::Sample<Value>     Sample;
    virtual auto get_samples() const -> std::vector<Sample> const& = 0;
};
DECLARE_CLASS_PTR(IENU_Torque_Stream);

class IECEF_Torque_Stream : public ISpatial_Stream<IStream::Type::TORQUE, Space::ECEF>
{
public:
    typedef math::vec3f             Value; //Nm
    typedef node::Sample<Value>     Sample;
    virtual auto get_samples() const -> std::vector<Sample> const& = 0;
};
DECLARE_CLASS_PTR(IECEF_Torque_Stream);

}
}
