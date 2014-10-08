#pragma once

namespace silk
{

//----------------------------------------------------------------------
//data interface

template<class T> struct Sensor_Sample
{
    Sensor_Sample() : value() {}
    T value;
    uint32_t sample_idx = 0; //incremented when for new samples
    q::Clock::duration dt{0}; //the duration of this sample.
    q::Clock::time_point time_point; //the moment in time when this sample was acquired. All samples from all sensors use the same epoch so their time_points can be compared directly
};

class HAL_Sensors : q::util::Noncopyable
{
public:
    virtual ~HAL_Sensors() {}

    //----------------------------------------------------------------------
    //calibration
    virtual void set_accelerometer_calibration_data(math::vec3f const& bias, math::vec3f const& scale) = 0;
    virtual void get_accelerometer_calibration_data(math::vec3f& bias, math::vec3f& scale) const = 0;

    virtual void set_gyroscope_calibration_data(math::vec3f const& bias) = 0;
    virtual void get_gyroscope_calibration_data(math::vec3f& bias) const = 0;

    virtual void set_compass_calibration_data(math::vec3f const& bias) = 0;
    virtual void get_compass_calibration_data(math::vec3f& bias) const = 0;

    //----------------------------------------------------------------------
    //sensors

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

    virtual auto get_accelerometer_samples() const  -> std::vector<Accelerometer_Sample> const& = 0;
    virtual auto get_gyroscope_samples() const      -> std::vector<Gyroscope_Sample> const& = 0;
    virtual auto get_compass_samples() const        -> std::vector<Compass_Sample> const& = 0;
    virtual auto get_barometer_samples() const      -> std::vector<Barometer_Sample> const& = 0;
    virtual auto get_sonar_samples() const          -> std::vector<Sonar_Sample> const& = 0;
    virtual auto get_thermometer_samples() const    -> std::vector<Thermometer_Sample> const& = 0;
    virtual auto get_voltage_samples() const        -> std::vector<Voltage_Sample> const& = 0;
    virtual auto get_current_samples() const        -> std::vector<Current_Sample> const& = 0;
    virtual auto get_gps_samples() const            -> std::vector<GPS_Sample> const& = 0;

    //----------------------------------------------------------------------
    //returns how many comm errors we've got
    virtual size_t get_error_count() const = 0;

    //----------------------------------------------------------------------
    virtual void process() = 0;
};

}
