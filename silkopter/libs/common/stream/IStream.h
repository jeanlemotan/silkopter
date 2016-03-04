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
    UAV,
    GIMBAL
};

#define GET_AS_STRING(str, details_str) q::util::format2<std::string>(str "{}", (details) ? (" " details_str) : "")

inline auto get_as_string(Space s, bool details) -> std::string
{
    switch (s)
    {
    case Space::LLA: return GET_AS_STRING("LLA", "(Latitude Longitude Altitude)");
    case Space::ECEF: return GET_AS_STRING("ECEF", "(Earth Centered Earth Fixed)");
    case Space::ENU: return GET_AS_STRING("ENU", "(East North Up)");
    case Space::UAV: return GET_AS_STRING("UAV", "(UAV Frame)");
    case Space::GIMBAL: return GET_AS_STRING("GIMBAL", "(Gimbal Frame)");
    default: QASSERT(false); return "Unknown";
    }
}

enum class Type
{
    ACCELERATION,
    ADC,
    ANGULAR_VELOCITY,
    BATTERY_STATE,
    MULTI_COMMANDS,
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

inline auto get_as_string(Type type, bool details) -> std::string
{
    switch (type)
    {
    case Type::ACCELERATION: return GET_AS_STRING("Acceleration", "(vec3, m/s^2)");
    case Type::ADC: return GET_AS_STRING("ADC", "(float, 0..1)");
    case Type::ANGULAR_VELOCITY: return GET_AS_STRING("Angular Velocity", "(vec3, Rad/s)");
    case Type::BATTERY_STATE: return GET_AS_STRING("Battery State", "(struct)");
    case Type::MULTI_COMMANDS: return GET_AS_STRING("Multi Commands", "(struct)");
    case Type::MULTI_STATE: return GET_AS_STRING("Multi State", "(struct)");
    case Type::CURRENT: return GET_AS_STRING("Current", "(float, A)");
    case Type::DISTANCE: return GET_AS_STRING("Distance", "(vec3, Ray)");
    case Type::FORCE: return GET_AS_STRING("Force", "(vec3, N)");
    case Type::FRAME: return GET_AS_STRING("Frame", "(quat)");
    case Type::LINEAR_ACCELERATION: return GET_AS_STRING("Linear Acceleration", "(vec3, m/s^2)");
    case Type::POSITION: return GET_AS_STRING("Position", "(vec3, m)");
    case Type::MAGNETIC_FIELD: return GET_AS_STRING("Magnetic Field", "(vec3, T)");
    case Type::PRESSURE: return GET_AS_STRING("Pressure", "(double, Pa)");
    case Type::PWM: return GET_AS_STRING("PWM", "(float, 0..1)");
    case Type::TEMPERATURE: return GET_AS_STRING("Temperature", "(float, °C)");
    case Type::TORQUE: return GET_AS_STRING("Torque", "(vec3, Nm)");
    case Type::VELOCITY: return GET_AS_STRING("Velocity", "(vec3, m/s)");
    case Type::THROTTLE: return GET_AS_STRING("Throttle", "(float, 0..1)");
    case Type::VIDEO: return GET_AS_STRING("Video", "(blob)");
    case Type::PROXIMITY: return GET_AS_STRING("Proximity", "(vector<vec3>, Rays>)");
    case Type::GPS_INFO: return GET_AS_STRING("GPS Info", "(struct)");
    case Type::VOLTAGE: return GET_AS_STRING("Voltage", "(float, V)");
    case Type::FLOAT: return GET_AS_STRING("Float", "(float)");
    case Type::BOOL: return GET_AS_STRING("Bool", "(bool)");
    default: QASSERT(false); return "Unknown";
    }
}

#undef GET_AS_STRING

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
    Sample(Sample const& other) = default;
    Sample(Sample&& other) = default;
    Sample& operator=(Sample const& other) = default;
    Sample& operator=(Sample&& other) = default;

    T value;
    bool is_healthy = false;
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
