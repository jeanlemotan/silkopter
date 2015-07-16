#pragma once

#include "utils/Serialization.h"

namespace silk
{
namespace node
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
    q::Clock::duration dt{0}; //the duration of this sample.
    q::Clock::time_point tp{q::Clock::duration(0)}; //when was the sample generated.
    bool is_healthy = true;
};



}
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
    uint64_t dt : 20; //32us
    uint64_t tp : 35; //32us
    uint64_t is_healthy : 1;
};
#pragma pack(pop)
}


template<class T> inline void serialize(Buffer_t& buffer, silk::node::stream::Sample<T> const& sample, size_t& off)
{
    auto dt = static_cast<uint32_t>(std::chrono::duration_cast<std::chrono::microseconds>(sample.dt).count()) >> 5;
    if (dt >= (1 << 20))
    {
        QLOGE("Sample dt is too big!!! {} > {}", dt, 1 << 20);
        dt = (1 << 20) - 1;
    }
    auto tp = static_cast<uint64_t>(std::chrono::duration_cast<std::chrono::microseconds>(sample.tp.time_since_epoch()).count()) >> 5;
    if (tp >= (uint64_t(1) << 35))
    {
        QLOGE("Sample tp is too big!!! {} > {}", tp, uint64_t(1) << 35);
        tp = (uint64_t(1) << 35) - 1;
    }
    detail::Serialized_Sample_Data data;
    data.is_healthy = sample.is_healthy;
    data.dt = dt;
    data.tp = tp;
    serialize(buffer, data, off);
    serialize(buffer, sample.value, off);
}

template<class T> inline auto deserialize(Buffer_t const& buffer, silk::node::stream::Sample<T>& sample, size_t& off) -> bool
{
    detail::Serialized_Sample_Data data;
    if (!deserialize(buffer, data, off) || !deserialize(buffer, sample.value, off))
    {
        return false;
    }
    sample.dt = std::chrono::microseconds(data.dt << 5);
    sample.tp = q::Clock::time_point(std::chrono::microseconds(data.tp << 5));
    sample.is_healthy = data.is_healthy != 0;
    return true;
}


}
}
