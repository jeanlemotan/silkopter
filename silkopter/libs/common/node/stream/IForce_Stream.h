#pragma once

#include "IStream.h"

namespace silk
{
namespace node
{

class IForce_Stream : public ISpatial_Stream<IStream::Type::FORCE, Space::LOCAL>
{
public:
    typedef float             Value; //N
    typedef node::Sample<Value>     Sample;
    virtual auto get_samples() const -> std::vector<Sample> const& = 0;
};
DECLARE_CLASS_PTR(IForce_Stream);

class IENU_Force_Stream : public ISpatial_Stream<IStream::Type::FORCE, Space::ENU>
{
public:
    typedef float             Value; //N
    typedef node::Sample<Value>     Sample;
    virtual auto get_samples() const -> std::vector<Sample> const& = 0;
};
DECLARE_CLASS_PTR(IENU_Force_Stream);

class IECEF_Force_Stream : public ISpatial_Stream<IStream::Type::FORCE, Space::ECEF>
{
public:
    typedef float             Value; //N
    typedef node::Sample<Value>     Sample;
    virtual auto get_samples() const -> std::vector<Sample> const& = 0;
};
DECLARE_CLASS_PTR(IECEF_Force_Stream);

}
}
