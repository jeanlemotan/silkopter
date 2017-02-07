#pragma once

#include "Stream_Base.h"
#include "IFrame.h"

namespace silk
{
namespace stream
{


template<class T> struct Input_Value
{
    typedef T Value;

    Input_Value() = default;
    Input_Value(T value) : value(value) {}

    void set(Value const& v) { if (value != v) { value = v; version++; } }
    void set_unversioned(Value const& v) { value = v; }
    T const& get() const { return value; }

    //    bool operator==(Input_Value const& other) = delete;//{ return value == other.value; }
    //    bool operator!=(Input_Value const& other) = delete;//{ return !operator==(other); }
    //    Input_Value const& operator=(Input_Value const& other)  = delete;//{ value = other.value; return *this; }

    uint8_t version = 0;
private:
    T value = T();
};
template<> struct Input_Value<bool>
{
    typedef bool Value;

    Input_Value() : version(0), value(0) {}
    Input_Value(bool value) : version(0), value(value ? 1 : 0) {}

    void set(Value const& v) { if (value != v) { value = v; version++; } }
    void set_unversioned(Value const& v) { value = v; }
    bool get() const { return value != 0; }

    //    bool operator==(Input_Value const& other) = delete;// { return value == other.value; }
    //    bool operator!=(Input_Value const& other) = delete;// { return !operator==(other); }
    //    Input_Value const& operator=(Input_Value const& other) = delete;// { value = other.value; return *this; }

    uint8_t version : 7;
private:
    uint8_t value : 1;
};


class IMultirotor_Commands : public IScalar_Stream<Semantic::MULTIROTOR_COMMANDS>
{
public:
    typedef std::false_type can_be_filtered_t;

    enum class Mode : uint8_t
    {
        IDLE,
        TAKE_OFF,
        FLY,
        RETURN_HOME,
        LAND
    };

    enum class Vertical_Mode : uint8_t
    {
        THRUST,
        ALTITUDE,
    };
    enum class Horizontal_Mode : uint8_t
    {
        ANGLE_RATE,
        ANGLE,
        POSITION,
    };
    enum class Yaw_Mode : uint8_t
    {
        ANGLE_RATE,
        ANGLE,
    };

    struct Helpers
    {
        uint8_t stay_in_range : 1; //avoid out of range situations.
        uint8_t stay_in_battery_range : 1; //avoid going too far considering current battery.
        uint8_t stay_in_perimeter : 1; //stay in a configured perimeter.
        uint8_t avoid_altitude_drop : 1; //avoid dropping too much altitude too fast.
        uint8_t avoid_the_user : 1; //avoid getting too close to the launch position (the user).
        uint8_t avoid_proximity : 1; //maintains a min distance from all objects around.
    };

    struct Sticks
    {
        float yaw = 0;
        float pitch = 0.f;
        float roll = 0.f;
        float throttle = 0.f;
    };

    ///////////////////////////////
    /// Data

    struct Value
    {
        Mode mode = Mode::IDLE;

        Sticks sticks;

        Vertical_Mode vertical_mode = Vertical_Mode::THRUST;
        Horizontal_Mode horizontal_mode = Horizontal_Mode::ANGLE;
        Yaw_Mode yaw_mode = Yaw_Mode::ANGLE_RATE;

        float gimbal_pitch = 0.f; //+- radians

        union
        {
            Helpers helpers;
            uint8_t all_helpers = 0;
        };
    };

    typedef stream::Sample<Value>     Sample;

    virtual ~IMultirotor_Commands() = default;

    virtual auto get_samples() const -> std::vector<Sample> const& = 0;
};


}
}


namespace util
{
namespace serialization
{

template<> inline void serialize(Buffer_t& buffer, silk::stream::IMultirotor_Commands::Value const& value, size_t& off)
{
    serialize(buffer, value.mode, off);

    serialize(buffer, static_cast<uint8_t>((math::clamp(value.sticks.yaw, 0.f, 1.f) * 255.f)), off);
    serialize(buffer, static_cast<uint8_t>((math::clamp(value.sticks.pitch, 0.f, 1.f) * 255.f)), off);
    serialize(buffer, static_cast<uint8_t>((math::clamp(value.sticks.roll, 0.f, 1.f) * 255.f)), off);
    serialize(buffer, static_cast<uint8_t>((math::clamp(value.sticks.throttle, 0.f, 1.f) * 255.f)), off);

    serialize(buffer, value.vertical_mode, off);
    serialize(buffer, value.horizontal_mode, off);
    serialize(buffer, value.yaw_mode, off);

    serialize(buffer, static_cast<int8_t>((math::clamp(value.gimbal_pitch, -math::anglef::pi, math::anglef::pi) * 40.f)), off);

    serialize(buffer, value.all_helpers, off);
}

template<> inline auto deserialize(Buffer_t const& buffer, silk::stream::IMultirotor_Commands::Value& value, size_t& off) -> bool
{
    if (!deserialize(buffer, value.mode, off))
    {
        return false;
    }

    //sticks
    uint8_t v1, v2, v3, v4;
    if (!deserialize(buffer, v1, off) ||
            !deserialize(buffer, v2, off) ||
            !deserialize(buffer, v3, off) ||
            !deserialize(buffer, v4, off))
    {
        return false;
    }

    value.sticks.yaw = v1 / 255.f;
    value.sticks.pitch = v2 / 255.f;
    value.sticks.roll = v3 / 255.f;
    value.sticks.throttle = v4 / 255.f;

    //vertical mode
    if (!deserialize(buffer, value.vertical_mode, off))
    {
        return false;
    }

    //horizontal mode
    if (!deserialize(buffer, value.horizontal_mode, off))
    {
        return false;
    }

    //yaw mode
    if (!deserialize(buffer, value.yaw_mode, off))
    {
        return false;
    }

    //gimbal pitch
    if (!deserialize(buffer, v1, off))
    {
        return false;
    }
    value.gimbal_pitch = v1 / 40.f;

    //helpers
    if (!deserialize(buffer, value.all_helpers, off))
    {
        return false;
    }

    return true;
}


}
}
