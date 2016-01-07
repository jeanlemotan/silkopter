#pragma once

#include "Stream_Base.h"

namespace silk
{
namespace stream
{


template<class T> struct Input_Value
{
    typedef T Value;

    Input_Value() = default;
    Input_Value(T value) : value(value) {}

    void set(Value const& v) { value = v; version++; }
    T const& get() const { return value; }

    uint32_t version = 0;
    T value = T();
};
template<> struct Input_Value<bool>
{
    typedef bool Value;

    Input_Value() : version(0), value(0) {}
    Input_Value(bool value) : version(0), value(value ? 1 : 0) {}

    void set(Value const& v) { value = v; version++; }
    bool get() const { return value != 0; }

    uint32_t version : 31;
    uint32_t value : 1;
};


class IMulti_Input : public IScalar_Stream<Type::MULTI_INPUT>
{
public:
    typedef std::false_type can_be_filtered_t;

    struct Toggles
    {
        Input_Value<bool> take_off;
        Input_Value<bool> land;
        Input_Value<bool> return_home;
    };

    enum class Mode : uint8_t
    {
        IDLE,
        ARMED,
    };

    struct Vertical
    {
        Vertical() = default;

        enum class Mode : uint8_t
        {
            THRUST_RATE,
            THRUST,
            ALTITUDE,
        };
        Input_Value<Mode> mode = Mode::THRUST;

        Input_Value<float> thrust_rate; //Newtons per second
        Input_Value<float> thrust;      //Newtons
        Input_Value<float> altitude;    //meters
    };

    struct Horizontal
    {
        Horizontal() = default;

        enum class Mode : uint8_t
        {
            ANGLE_RATE,
            ANGLE,
            POSITION,
        };
        Input_Value<Mode> mode = Mode::ANGLE;

        Input_Value<math::vec2f> angle_rate;   //angle rate of change - radians per second
        Input_Value<math::vec2f> angle;        //angle from horizontal. zero means horizontal
        Input_Value<math::vec2f> position;     //meters
    };

    struct Yaw
    {
        enum class Mode : uint8_t
        {
            ANGLE_RATE,
            ANGLE,
        };
        Input_Value<Mode> mode = Mode::ANGLE_RATE;

        Input_Value<float> angle_rate;  //radians per second
        Input_Value<float> angle;       //radians
    };

    //the reference frame for the user controls
    enum class Reference_Frame : uint8_t
    {
        LOCAL, 	//normal mode - back means back of uav
        USER,	//simple mode - back means towards the user, front away from her.
    };

    struct Helpers
    {
        Input_Value<bool> stay_in_range; //avoid out of range situations.
        Input_Value<bool> stay_in_battery_range; //avoid going too far considering current battery.
        Input_Value<bool> stay_in_perimeter; //stay in a configured perimeter.
        Input_Value<bool> avoid_altitude_drop; //avoid dropping too much altitude too fast.
        Input_Value<bool> avoid_the_user; //avoid getting too close to the launch position (the user).
        Input_Value<bool> avoid_proximity; //maintains a min distance from all objects around.
    };

    ///////////////////////////////
    /// Data


    struct Value
    {
        Toggles toggles;
        Vertical vertical;
        Horizontal horizontal;
        Yaw yaw;

        Input_Value<Mode> mode = Mode::IDLE;
        Input_Value<Reference_Frame> reference_frame = Reference_Frame::LOCAL;
        Helpers assists;
    };

    typedef stream::Sample<Value>     Sample;

    virtual ~IMulti_Input() {}

    virtual auto get_samples() const -> std::vector<Sample> const& = 0;
};
DECLARE_CLASS_PTR(IMulti_Input);


}
}


namespace util
{
namespace serialization
{

template<class T> inline void serialize(Buffer_t& buffer, silk::stream::Input_Value<T> const& value, size_t& off)
{
    serialize(buffer, value.get(), off);
    serialize(buffer, value.version, off);
}
template<> inline void serialize(Buffer_t& buffer, silk::stream::Input_Value<bool> const& value, size_t& off)
{
    uint32_t x = value.version;
    x |= value.value << 31;
    serialize(buffer, x, off);
}

template<class T> inline auto deserialize(Buffer_t const& buffer, silk::stream::Input_Value<T>& value, size_t& off) -> bool
{
    return  deserialize(buffer, value.value, off) &&
        deserialize(buffer, value.version, off);
}
template<> inline auto deserialize(Buffer_t const& buffer, silk::stream::Input_Value<bool>& value, size_t& off) -> bool
{
    uint32_t x = 0;
    if (!deserialize(buffer, x, off))
    {
        return false;
    }
    value.version = x & 0x7FFFFFFF;
    value.value = x >> 31;
    return true;
}


template<> inline void serialize(Buffer_t& buffer, silk::stream::IMulti_Input::Value const& value, size_t& off)
{
    serialize(buffer, value.toggles.land, off);
    serialize(buffer, value.toggles.return_home, off);
    serialize(buffer, value.toggles.take_off, off);
    serialize(buffer, value.vertical.mode, off);
    serialize(buffer, value.vertical.thrust_rate, off);
    serialize(buffer, value.vertical.thrust, off);
    serialize(buffer, value.vertical.altitude, off);
    serialize(buffer, value.horizontal.mode, off);
    serialize(buffer, value.horizontal.angle_rate, off);
    serialize(buffer, value.horizontal.angle, off);
    serialize(buffer, value.horizontal.position, off);
    serialize(buffer, value.yaw.mode, off);
    serialize(buffer, value.yaw.angle_rate, off);
    serialize(buffer, value.yaw.angle, off);
    serialize(buffer, value.mode, off);
    serialize(buffer, value.reference_frame, off);
    serialize(buffer, value.assists.stay_in_battery_range, off);
    serialize(buffer, value.assists.stay_in_perimeter, off);
    serialize(buffer, value.assists.stay_in_range, off);
    serialize(buffer, value.assists.avoid_altitude_drop, off);
    serialize(buffer, value.assists.avoid_proximity, off);
    serialize(buffer, value.assists.avoid_the_user, off);
}

template<> inline auto deserialize(Buffer_t const& buffer, silk::stream::IMulti_Input::Value& value, size_t& off) -> bool
{
    return deserialize(buffer, value.toggles.land, off) &&
        deserialize(buffer, value.toggles.return_home, off) &&
        deserialize(buffer, value.toggles.take_off, off) &&
        deserialize(buffer, value.vertical.mode, off) &&
        deserialize(buffer, value.vertical.thrust_rate, off) &&
        deserialize(buffer, value.vertical.thrust, off) &&
        deserialize(buffer, value.vertical.altitude, off) &&
        deserialize(buffer, value.horizontal.mode, off) &&
        deserialize(buffer, value.horizontal.angle_rate, off) &&
        deserialize(buffer, value.horizontal.angle, off) &&
        deserialize(buffer, value.horizontal.position, off) &&
        deserialize(buffer, value.yaw.mode, off) &&
        deserialize(buffer, value.yaw.angle_rate, off) &&
        deserialize(buffer, value.yaw.angle, off) &&
        deserialize(buffer, value.mode, off) &&
        deserialize(buffer, value.reference_frame, off) &&
        deserialize(buffer, value.assists.stay_in_battery_range, off) &&
        deserialize(buffer, value.assists.stay_in_perimeter, off) &&
        deserialize(buffer, value.assists.stay_in_range, off) &&
        deserialize(buffer, value.assists.avoid_altitude_drop, off) &&
        deserialize(buffer, value.assists.avoid_proximity, off) &&
        deserialize(buffer, value.assists.avoid_the_user, off);
}


}
}
