#pragma once

#include "common/sensors/Sensor_Samples.h"

namespace silk
{

//----------------------------------------------------------------------

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

    virtual auto get_accelerometer_samples() const  -> std::vector<Accelerometer_Sample> const& = 0;
    virtual auto get_gyroscope_samples() const      -> std::vector<Gyroscope_Sample> const& = 0;
    virtual auto get_compass_samples() const        -> std::vector<Compass_Sample> const& = 0;
    virtual auto get_barometer_samples() const      -> std::vector<Barometer_Sample> const& = 0;
    virtual auto get_sonar_samples() const          -> std::vector<Sonar_Sample> const& = 0;
    virtual auto get_thermometer_samples() const    -> std::vector<Thermometer_Sample> const& = 0;
    virtual auto get_voltage_samples() const        -> std::vector<Voltage_Sample> const& = 0;
    virtual auto get_current_samples() const        -> std::vector<Current_Sample> const& = 0;
    virtual auto get_gps_samples() const            -> std::vector<GPS_Sample> const& = 0;

    virtual auto get_last_accelerometer_sample() const  -> Accelerometer_Sample const& = 0;
    virtual auto get_last_gyroscope_sample() const      -> Gyroscope_Sample const& = 0;
    virtual auto get_last_compass_sample() const        -> Compass_Sample const& = 0;
    virtual auto get_last_barometer_sample() const      -> Barometer_Sample const& = 0;
    virtual auto get_last_sonar_sample() const          -> Sonar_Sample const& = 0;
    virtual auto get_last_thermometer_sample() const    -> Thermometer_Sample const& = 0;
    virtual auto get_last_voltage_sample() const        -> Voltage_Sample const& = 0;
    virtual auto get_last_current_sample() const        -> Current_Sample const& = 0;
    virtual auto get_last_gps_sample() const            -> GPS_Sample const& = 0;

    //----------------------------------------------------------------------
    //returns how many comm errors we've got
    virtual size_t get_error_count() const = 0;

    //----------------------------------------------------------------------
    virtual void process() = 0;
};

}
