#pragma once

#include "IStream.h"

namespace silk
{
namespace node
{
namespace stream
{

class IForce : public ISpatial_Stream<Type::FORCE, Space::LOCAL>
{
public:
    typedef math::vec3f             Value; //N
    typedef stream::Sample<Value>     Sample;
    virtual auto get_samples() const -> std::vector<Sample> const& = 0;

    typedef void Calibration_Data;
};
DECLARE_CLASS_PTR(IForce);

class IENU_Force : public ISpatial_Stream<Type::FORCE, Space::ENU>
{
public:
    typedef math::vec3f             Value; //N
    typedef stream::Sample<Value>     Sample;
    virtual auto get_samples() const -> std::vector<Sample> const& = 0;

    typedef void Calibration_Data;
};
DECLARE_CLASS_PTR(IENU_Force);

class IECEF_Force : public ISpatial_Stream<Type::FORCE, Space::ECEF>
{
public:
    typedef math::vec3f             Value; //N
    typedef stream::Sample<Value>     Sample;
    virtual auto get_samples() const -> std::vector<Sample> const& = 0;

    typedef void Calibration_Data;
};
DECLARE_CLASS_PTR(IECEF_Force);

}
}
}
