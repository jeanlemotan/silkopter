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

    void set(Value const& v) { if (value != v) { value = v; version++; } }
    T const& get() const { return value; }

//    bool operator==(Input_Value const& other) = delete;//{ return value == other.value; }
//    bool operator!=(Input_Value const& other) = delete;//{ return !operator==(other); }
//    Input_Value const& operator=(Input_Value const& other)  = delete;//{ value = other.value; return *this; }

    uint32_t version = 0;
    T value = T();
};
template<> struct Input_Value<bool>
{
    typedef bool Value;

    Input_Value() : version(0), value(0) {}
    Input_Value(bool value) : version(0), value(value ? 1 : 0) {}

    void set(Value const& v) { if (value != v) { value = v; version++; } }
    bool get() const { return value != 0; }

//    bool operator==(Input_Value const& other) = delete;// { return value == other.value; }
//    bool operator!=(Input_Value const& other) = delete;// { return !operator==(other); }
//    Input_Value const& operator=(Input_Value const& other) = delete;// { value = other.value; return *this; }

    uint32_t version : 31;
    uint32_t value : 1;
};


class IMulti_Commands : public IScalar_Stream<Type::MULTI_COMMANDS>
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

    virtual ~IMulti_Commands() {}

    virtual auto get_samples() const -> std::vector<Sample> const& = 0;


    //helper function that applies a functor on every member of the value
    template<class F, class... V>
    static bool apply(F& functor, V&&... values)
    {
        return     functor(values.toggles.land                  ...)
                && functor(values.toggles.return_home           ...)
                && functor(values.toggles.take_off              ...)
                && functor(values.vertical.mode                 ...)
                && functor(values.vertical.thrust_rate          ...)
                && functor(values.vertical.thrust               ...)
                && functor(values.vertical.altitude             ...)
                && functor(values.horizontal.mode               ...)
                && functor(values.horizontal.angle_rate         ...)
                && functor(values.horizontal.angle              ...)
                && functor(values.horizontal.position           ...)
                && functor(values.yaw.mode                      ...)
                && functor(values.yaw.angle_rate                ...)
                && functor(values.yaw.angle                     ...)
                && functor(values.mode                          ...)
                && functor(values.reference_frame               ...)
                && functor(values.assists.stay_in_battery_range ...)
                && functor(values.assists.stay_in_perimeter     ...)
                && functor(values.assists.stay_in_range         ...)
                && functor(values.assists.avoid_altitude_drop   ...)
                && functor(values.assists.avoid_proximity       ...)
                && functor(values.assists.avoid_the_user        ...);
    }

    struct Equality_Functor
    {
        template<class T> bool operator()(T const& v1, T const& v2) { return v1.value == v2.value; }
    };
    struct Equality_Version_Functor
    {
        template<class T> bool operator()(T const& v1, T const& v2) { return v1.value == v2.value && v1.version == v2.version; }
    };
    struct Assignment_Functor
    {
        template<class T> bool operator()(T& v1, T const& v2) { v1.value = v2.value; return true; }
    };
    struct Assignment_Version_Functor
    {
        template<class T> bool operator()(T& v1, T const& v2) { v1.value = v2.value; v1.version = v2.version; return true; }
    };
};
DECLARE_CLASS_PTR(IMulti_Commands);


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

namespace detail
{
struct Serializer
{
    Serializer(Buffer_t& buffer, size_t& off) : m_buffer(buffer), m_off(off) {}
    Buffer_t& m_buffer;
    size_t& m_off;

    template<class T>
    bool operator()(T const& v)
    {
        serialize(m_buffer, v, m_off);
        return true;
    }
};
struct Deserializer
{
    Deserializer(Buffer_t const& buffer, size_t& off) : m_buffer(buffer), m_off(off) {}
    Buffer_t const& m_buffer;
    size_t& m_off;

    template<class T>
    bool operator()(T& v)
    {
        return deserialize(m_buffer, v, m_off);
    }
};
}

template<> inline void serialize(Buffer_t& buffer, silk::stream::IMulti_Commands::Value const& value, size_t& off)
{
    detail::Serializer serializer(buffer, off);
    silk::stream::IMulti_Commands::apply(serializer, value);
}

template<> inline auto deserialize(Buffer_t const& buffer, silk::stream::IMulti_Commands::Value& value, size_t& off) -> bool
{
    detail::Deserializer deserializer(buffer, off);
    return silk::stream::IMulti_Commands::apply(deserializer, value);
}


}
}
