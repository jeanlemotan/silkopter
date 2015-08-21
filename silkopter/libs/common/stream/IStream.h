#pragma once

#include "utils/Serialization.h"

namespace silk
{
namespace stream
{

enum class Space : uint8_t
{
    LLA,
    ECEF,
    ENU,
    LOCAL
};

constexpr const char* Space_Name[] =
{
    "ECEF",
    "ENU",
    "Local"
};

enum class Type
{
    ACCELERATION,
    ADC,
    ANGULAR_VELOCITY,
    BATTERY_STATE,
    MULTI_INPUT,
    MULTI_STATE,
    CURRENT,
    DISTANCE,
    FORCE,
    FRAME,
    LINEAR_ACCELERATION,
    POSITION,
    MAGNETIC_FIELD,
    PRESSURE,
    PWM,
    TEMPERATURE,
    TORQUE,
    VELOCITY,
    THROTTLE,
    VIDEO,
    PROXIMITY,
    GPS_INFO,
    VOLTAGE,
    FLOAT,
    BOOL
};

//for streams of data with a fixed sample rate
class IStream : q::util::Noncopyable
{
public:
    virtual auto get_rate() const -> uint32_t = 0;
    virtual auto get_type() const -> Type = 0;
};
DECLARE_CLASS_PTR(IStream);


//A stream sample
template<class T> struct Sample
{
    typedef T Value;

    Sample() : value() {}
    T value;
    bool is_healthy = true;
};



}
}


namespace util
{
namespace serialization
{

namespace detail
{
#pragma pack(push, 1)
struct Serialized_Sample_Data
{
    uint8_t is_healthy : 1;
};
#pragma pack(pop)
}


template<class T> inline void serialize(Buffer_t& buffer, silk::stream::Sample<T> const& sample, size_t& off)
{
    detail::Serialized_Sample_Data data;
    data.is_healthy = sample.is_healthy;
    serialize(buffer, data, off);
    serialize(buffer, sample.value, off);
}

template<class T> inline auto deserialize(Buffer_t const& buffer, silk::stream::Sample<T>& sample, size_t& off) -> bool
{
    detail::Serialized_Sample_Data data;
    if (!deserialize(buffer, data, off) || !deserialize(buffer, sample.value, off))
    {
        return false;
    }
    sample.is_healthy = data.is_healthy != 0;
    return true;
}


}
}
