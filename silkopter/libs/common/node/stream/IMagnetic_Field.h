#pragma once

#include "IStream.h"

namespace silk
{
namespace node
{
namespace stream
{

class IMagnetic_Field : public ISpatial_Stream<Type::MAGNETIC_FIELD, Space::LOCAL>
{
public:
    typedef math::vec3f       Value; //??
    typedef stream::Sample<Value>     Sample;
    virtual auto get_samples() const -> std::vector<Sample> const& = 0;
};
DECLARE_CLASS_PTR(IMagnetic_Field);

class IENU_Magnetic_Field : public ISpatial_Stream<Type::MAGNETIC_FIELD, Space::ENU>
{
public:
    typedef math::vec3f       Value; //??
    typedef stream::Sample<Value>     Sample;
    virtual auto get_samples() const -> std::vector<Sample> const& = 0;
};
DECLARE_CLASS_PTR(IENU_Magnetic_Field);

class IECEF_Magnetic_Field : public ISpatial_Stream<Type::MAGNETIC_FIELD, Space::ECEF>
{
public:
    typedef math::vec3f       Value; //??
    typedef stream::Sample<Value>     Sample;
    virtual auto get_samples() const -> std::vector<Sample> const& = 0;
};
DECLARE_CLASS_PTR(IECEF_Magnetic_Field);


}
}
}
