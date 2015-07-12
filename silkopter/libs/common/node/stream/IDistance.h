#pragma once

#include "Stream_Base.h"

namespace silk
{
namespace node
{
namespace stream
{

class IDistance : public ISpatial_Stream<Type::DISTANCE, Space::LOCAL>
{
public:
    typedef std::true_type can_be_filtered_t;

    typedef math::vec3f               Value; //meters
    typedef stream::Sample<Value>     Sample;
    virtual auto get_samples() const -> std::vector<Sample> const& = 0;
};
DECLARE_CLASS_PTR(IDistance);

class IENU_Distance : public ISpatial_Stream<Type::DISTANCE, Space::ENU>
{
public:
    typedef std::true_type can_be_filtered_t;

    typedef math::vec3f               Value; //meters
    typedef stream::Sample<Value>     Sample;
    virtual auto get_samples() const -> std::vector<Sample> const& = 0;
};
DECLARE_CLASS_PTR(IENU_Distance);

class IECEF_Distance : public ISpatial_Stream<Type::DISTANCE, Space::ECEF>
{
public:
    typedef std::true_type can_be_filtered_t;

    typedef math::vec3f               Value; //meters
    typedef stream::Sample<Value>     Sample;
    virtual auto get_samples() const -> std::vector<Sample> const& = 0;
};
DECLARE_CLASS_PTR(IECEF_Distance);

}
}
}
