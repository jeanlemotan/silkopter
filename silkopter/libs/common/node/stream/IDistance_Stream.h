#pragma once

#include "IStream.h"

namespace silk
{
namespace node
{

class IDistance_Stream : public ISpatial_Stream<IStream::Type::DISTANCE, Space::LOCAL>
{
public:
    typedef math::vec3f               Value; //meters
    typedef node::Sample<Value>     Sample;
    virtual auto get_samples() const -> std::vector<Sample> const& = 0;
};
DECLARE_CLASS_PTR(IDistance_Stream);

class IENU_Distance_Stream : public ISpatial_Stream<IStream::Type::DISTANCE, Space::ENU>
{
public:
    typedef math::vec3f               Value; //meters
    typedef node::Sample<Value>     Sample;
    virtual auto get_samples() const -> std::vector<Sample> const& = 0;
};
DECLARE_CLASS_PTR(IENU_Distance_Stream);

class IECEF_Distance_Stream : public ISpatial_Stream<IStream::Type::DISTANCE, Space::ECEF>
{
public:
    typedef math::vec3f               Value; //meters
    typedef node::Sample<Value>     Sample;
    virtual auto get_samples() const -> std::vector<Sample> const& = 0;
};
DECLARE_CLASS_PTR(IECEF_Distance_Stream);

}
}
