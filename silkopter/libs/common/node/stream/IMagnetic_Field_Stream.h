#pragma once

#include "IStream.h"

namespace silk
{
namespace node
{

class IMagnetic_Field_Stream : public ISpatial_Stream<IStream::Type::MAGNETIC_FIELD, Space::LOCAL>
{
public:
    typedef math::vec3f       Value; //??
    typedef node::Sample<Value>     Sample;
    virtual auto get_samples() const -> std::vector<Sample> const& = 0;
};
DECLARE_CLASS_PTR(IMagnetic_Field_Stream);

class IENU_Magnetic_Field_Stream : public ISpatial_Stream<IStream::Type::MAGNETIC_FIELD, Space::ENU>
{
public:
    typedef math::vec3f       Value; //??
    typedef node::Sample<Value>     Sample;
    virtual auto get_samples() const -> std::vector<Sample> const& = 0;
};
DECLARE_CLASS_PTR(IENU_Magnetic_Field_Stream);

class IECEF_Magnetic_Field_Stream : public ISpatial_Stream<IStream::Type::MAGNETIC_FIELD, Space::ECEF>
{
public:
    typedef math::vec3f       Value; //??
    typedef node::Sample<Value>     Sample;
    virtual auto get_samples() const -> std::vector<Sample> const& = 0;
};
DECLARE_CLASS_PTR(IECEF_Magnetic_Field_Stream);


}
}
