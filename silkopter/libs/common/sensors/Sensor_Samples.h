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

struct GPS
{
    uint8_t fix_count = 0;
    float precision = 0;
    double latitude = 0;
    double longitude = 0;
};
typedef Sensor_Sample<GPS> GPS_Sample;

}
