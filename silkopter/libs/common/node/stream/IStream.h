#pragma once

namespace silk
{
namespace node
{
namespace stream
{

enum class Space : uint8_t
{
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
    COMMANDS,
    CURRENT,
    DISTANCE,
    FORCE,
    FRAME,
    LINEAR_ACCELERATION,
    LOCATION,
    MAGNETIC_FIELD,
    PRESSURE,
    PWM,
    TEMPERATURE,
    TORQUE,
    VELOCITY,
    THROTTLE,
    VIDEO,
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

//for streams of data with a fixed sample rate
template <Type TYPE_VALUE>
class IScalar_Stream : public IStream
{
public:
    static constexpr Type TYPE = TYPE_VALUE;
    virtual auto get_type() const -> Type { return TYPE; }
};

//for vectorial streams of data with a fixed sample rate
template <Type TYPE_VALUE, Space SPACE_VALUE>
class ISpatial_Stream : public IStream
{
public:
    static constexpr Type TYPE = TYPE_VALUE;
    static constexpr Space SPACE = SPACE_VALUE;
    virtual auto get_type() const -> Type { return TYPE; }
};

//A stream sample
template<class T> struct Sample
{
    typedef T Value;

    Sample() : value() {}
    T value;
    uint32_t sample_idx = 0; //different for each sample. NOTE - it's allowed to wrap so don't compare like this: if (my_sample_idx > sample_idx) !!!!!!!
    q::Clock::duration dt{0}; //the duration of this sample.
    q::Clock::time_point tp{q::Clock::duration(0)}; //when was the sample generated.
    bool is_healthy = true;
};

}
}
}
