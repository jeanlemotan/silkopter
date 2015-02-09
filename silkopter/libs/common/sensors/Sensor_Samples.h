#pragma once

namespace silk
{
namespace sensors
{

struct Acceleration
{
    math::vec3f value;//meters / second^2
};
struct Angular_Velocity
{
    math::vec3f value;//radians per second
};
struct Magnetic_Field
{
    math::vec3f value;//NOT normalized
};
struct Pressure
{
    float value = 0;//kp
};
struct Distance
{
    float value = 0;//meters
};
struct Temperature
{
    float value = 0;//degrees celsius
};
struct Voltage
{
    float value = 0;//volts
};
struct Current
{
    float value = 0;//amperes / second
};
struct Location
{
    enum class Fix : uint8_t
    {
        NONE,
        FIX_2D,
        FIX_3D
    };

    Fix fix;
    uint8_t sattelite_count = 0;
    float precision = std::numeric_limits<float>::max();

    boost::optional<double> longitude; //radians
    boost::optional<double> latitude; //radians
    boost::optional<float> altitude; //meters
    boost::optional<math::vec2f> velocity_2d; //meters/second but x/y only
    boost::optional<math::vec3f> velocity; //meters/second
    boost::optional<math::vec2f> direction; //normalized
};


///////////////////////////
///
/// Samples

template<class T> struct Sample
{
    Sample() : value() {}
    T value;
    uint32_t sample_idx = 0; //different for each sample. NOTE - it's allowed to wrap so don't compare like this: if (my_sample_idx > sample_idx) !!!!!!!
    q::Clock::duration dt{0}; //the duration of this sample.
};
template<class T> struct TP_Sample : public Sample<T>
{
    q::Clock::time_point time_point;
};


typedef Sample<Acceleration>        Accelerometer_Sample;
typedef Sample<Angular_Velocity>    Gyroscope_Sample;
typedef Sample<Magnetic_Field>      Compass_Sample;
typedef Sample<Pressure>            Barometer_Sample;
typedef Sample<Distance>            Sonar_Sample;
typedef Sample<Temperature>         Thermometer_Sample;
typedef Sample<Voltage>             Voltmeter_Sample;
typedef Sample<Current>             Ammeter_Sample;
typedef Sample<Location>            GPS_Sample;

///////////////////////////////////////////////
/// timestamped samples
///

typedef TP_Sample<Acceleration>         Accelerometer_TP_Sample;
typedef TP_Sample<Angular_Velocity>     Gyroscope_TP_Sample;
typedef TP_Sample<Magnetic_Field>       Compass_TP_Sample;
typedef TP_Sample<Pressure>             Barometer_TP_Sample;
typedef TP_Sample<Distance>             Sonar_TP_Sample;
typedef TP_Sample<Temperature>          Thermometer_TP_Sample;
typedef TP_Sample<Voltage>              Voltmeter_TP_Sample;
typedef TP_Sample<Current>              Ammeter_TP_Sample;
typedef TP_Sample<Location>             GPS_TP_Sample;

}
}
