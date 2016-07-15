#pragma once

#include "utils/Serialization.h"

namespace silk
{
namespace stream
{

enum class Space : uint8_t
{
    SCALAR,
    LLA,
    ECEF,
    ENU,
    LOCAL,
    GIMBAL
};

#define GET_AS_STRING(str, details_str) q::util::format<std::string>(str "{}", (details) ? (" " details_str) : "")

inline auto get_as_string(Space s, bool details) -> std::string
{
    switch (s)
    {
    case Space::SCALAR: return GET_AS_STRING("Scalar", "(Non-spatial)");
    case Space::LLA: return GET_AS_STRING("LLA", "(Latitude Longitude Altitude)");
    case Space::ECEF: return GET_AS_STRING("ECEF", "(Earth Centered Earth Fixed)");
    case Space::ENU: return GET_AS_STRING("ENU", "(East North Up)");
    case Space::LOCAL: return GET_AS_STRING("LOCAL", "(Local Frame)");
    case Space::GIMBAL: return GET_AS_STRING("Gimbal", "(Gimbal Frame)");
    default: QASSERT(false); return "Unknown";
    }
}

enum class Semantic : uint8_t
{
    ACCELERATION,
    ADC,
    ANGULAR_VELOCITY,
    BATTERY_STATE,
    MULTIROTOR_COMMANDS,
    MULTIROTOR_STATE,
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

inline auto get_as_string(Semantic type, bool details) -> std::string
{
    switch (type)
    {
    case Semantic::ACCELERATION: return GET_AS_STRING("Acceleration", "(vec3, m/s^2)");
    case Semantic::ADC: return GET_AS_STRING("ADC", "(float, 0..1)");
    case Semantic::ANGULAR_VELOCITY: return GET_AS_STRING("Angular Velocity", "(vec3, Rad/s)");
    case Semantic::BATTERY_STATE: return GET_AS_STRING("Battery State", "(struct)");
    case Semantic::MULTIROTOR_COMMANDS: return GET_AS_STRING("Multirotor Commands", "(struct)");
    case Semantic::MULTIROTOR_STATE: return GET_AS_STRING("Multirotor State", "(struct)");
    case Semantic::CURRENT: return GET_AS_STRING("Current", "(float, A)");
    case Semantic::DISTANCE: return GET_AS_STRING("Distance", "(vec3, Ray)");
    case Semantic::FORCE: return GET_AS_STRING("Force", "(vec3, N)");
    case Semantic::FRAME: return GET_AS_STRING("Frame", "(quat)");
    case Semantic::LINEAR_ACCELERATION: return GET_AS_STRING("Linear Acceleration", "(vec3, m/s^2)");
    case Semantic::POSITION: return GET_AS_STRING("Position", "(vec3, m)");
    case Semantic::MAGNETIC_FIELD: return GET_AS_STRING("Magnetic Field", "(vec3, T)");
    case Semantic::PRESSURE: return GET_AS_STRING("Pressure", "(double, Pa)");
    case Semantic::PWM: return GET_AS_STRING("PWM", "(float, 0..1)");
    case Semantic::TEMPERATURE: return GET_AS_STRING("Temperature", "(float, °C)");
    case Semantic::TORQUE: return GET_AS_STRING("Torque", "(vec3, Nm)");
    case Semantic::VELOCITY: return GET_AS_STRING("Velocity", "(vec3, m/s)");
    case Semantic::THROTTLE: return GET_AS_STRING("Throttle", "(float, 0..1)");
    case Semantic::VIDEO: return GET_AS_STRING("Video", "(blob)");
    case Semantic::PROXIMITY: return GET_AS_STRING("Proximity", "(vector<vec3>, Rays>)");
    case Semantic::GPS_INFO: return GET_AS_STRING("GPS Info", "(struct)");
    case Semantic::VOLTAGE: return GET_AS_STRING("Voltage", "(float, V)");
    case Semantic::FLOAT: return GET_AS_STRING("Float", "(float)");
    case Semantic::BOOL: return GET_AS_STRING("Bool", "(bool)");
    default: QASSERT(false); return "Unknown";
    }
}

struct Type
{
    constexpr Type(Semantic semantic, Space space)
        : id((static_cast<uint16_t>(semantic) << 8) | static_cast<uint16_t>(space))
    {
    }
    Type() : Type(Semantic::ACCELERATION, Space::LOCAL) {}

    Semantic get_semantic() const { return static_cast<Semantic>((id >> 8) & 0xFF); }
    Space get_space() const { return static_cast<Space>((id) & 0xFF); }
    uint16_t get_id() const { return id; }

    bool operator==(Type const& other) const { return id == other.id; }
    bool operator!=(Type const& other) const { return !operator==(other); }
    bool operator<(Type const& other) const { return id < other.id; }
    bool operator>(Type const& other) const { return id > other.id; }

private:
    uint16_t id;
};

inline auto get_as_string(Type type, bool details) -> std::string
{
    return get_as_string(type.get_semantic(), details) + " " + get_as_string(type.get_space(), details);
}

#undef GET_AS_STRING

//for streams of data with a fixed sample rate
class IStream : q::util::Noncopyable
{
public:
    virtual auto get_rate() const -> uint32_t = 0;
    virtual auto get_type() const -> Type = 0;
};


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

template<> inline void serialize(Buffer_t& buffer, silk::stream::Type const& type, size_t& off)
{
    serialize(buffer, type.get_semantic(), off);
    serialize(buffer, type.get_space(), off);
}

template<> inline auto deserialize(Buffer_t const& buffer, silk::stream::Type& type, size_t& off) -> bool
{
    silk::stream::Semantic semantic;
    silk::stream::Space space;
    if (!deserialize(buffer, semantic, off) || !deserialize(buffer, space, off))
    {
        return false;
    }
    type = silk::stream::Type(semantic, space);
    return true;
}


}
}
