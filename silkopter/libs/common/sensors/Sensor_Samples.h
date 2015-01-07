#pragma once

namespace silk
{
namespace sensors
{

template<class T> struct Sample
{
    Sample() : value() {}
    T value;
    uint32_t sample_idx = 0; //different for each sample. NOTE - it's allowed to wrap so don't compare like this: if (my_sample_idx > sample_idx) !!!!!!!
    q::Clock::duration dt{0}; //the duration of this sample.
};

typedef Sample<math::vec3f> Accelerometer_Sample; //meters / second^2
typedef Sample<math::vec3f> Gyroscope_Sample; //radians per second
typedef Sample<math::vec3f> Compass_Sample; //NOT normalized
typedef Sample<float> Barometer_Sample; //kp
typedef Sample<float> Sonar_Sample; //meters
typedef Sample<float> Thermometer_Sample; //degrees celsius
typedef Sample<float> Voltage_Sample; //volts
typedef Sample<float> Current_Sample; //amperes / second

struct GPS
{
    uint8_t sattelite_count = 0;
    float precision = std::numeric_limits<float>::max();

    boost::optional<double> longitude; //radians
    boost::optional<double> latitude; //radians
    boost::optional<float> altitude; //meters
    boost::optional<math::vec2f> speed_2d; //meters/second but x/y only
    boost::optional<math::vec2f> speed; //meters/second
    boost::optional<math::vec2f> direction; //normalized
};
typedef Sample<GPS> GPS_Sample;

}
}
