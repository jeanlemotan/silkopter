#pragma once

namespace silk
{

template<class T> struct Sensor_Sample
{
    Sensor_Sample() : value() {}
    T value;
    uint32_t sample_idx = 0; //different for each sample. NOTE - it's allowed to wrap so don't compare like this: if (my_sample_idx > sample_idx) !!!!!!!
    q::Clock::duration dt{0}; //the duration of this sample.
};

typedef Sensor_Sample<math::vec3f> Accelerometer_Sample; //meters / second^2
typedef Sensor_Sample<math::vec3f> Gyroscope_Sample; //radians per second
typedef Sensor_Sample<math::vec3f> Compass_Sample; //NOT normalized
typedef Sensor_Sample<float> Barometer_Sample; //kp
typedef Sensor_Sample<float> Sonar_Sample; //meters
typedef Sensor_Sample<float> Thermometer_Sample; //degrees celsius
typedef Sensor_Sample<float> Voltage_Sample; //volts
typedef Sensor_Sample<float> Current_Sample; //amperes / second

struct GPS_Data
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
typedef Sensor_Sample<GPS_Data> GPS_Sample;

}
