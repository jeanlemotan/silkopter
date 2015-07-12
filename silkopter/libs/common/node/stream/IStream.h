#pragma once

#include "rapidjson/document.h"     // rapidjson's DOM-style API

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
    uint32_t sample_idx = 0; //different for each sample. NOTE - it's allowed to wrap so don't compare like this: if (my_sample_idx > sample_idx) !!!!!!!
    q::Clock::duration dt{0}; //the duration of this sample.
    q::Clock::time_point tp{q::Clock::duration(0)}; //when was the sample generated.
    bool is_healthy = true;
};


}
}
}
