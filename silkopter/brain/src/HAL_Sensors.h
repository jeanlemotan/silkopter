#pragma once

#include "common/sensors/Sensor_Samples.h"

namespace silk
{

//----------------------------------------------------------------------

class HAL_Sensors : q::util::Noncopyable
{
public:
    virtual ~HAL_Sensors() {}

    virtual auto init() -> bool = 0;
    virtual void shutdown() = 0;

    //----------------------------------------------------------------------
    //calibration
    virtual void set_accelerometer_calibration_data(math::vec3f const& bias, math::vec3f const& scale) = 0;
    virtual void get_accelerometer_calibration_data(math::vec3f& bias, math::vec3f& scale) const = 0;

    virtual void set_gyroscope_calibration_data(math::vec3f const& bias) = 0;
    virtual void get_gyroscope_calibration_data(math::vec3f& bias) const = 0;

    virtual void set_compass_calibration_data(math::vec3f const& bias) = 0;
    virtual void get_compass_calibration_data(math::vec3f& bias) const = 0;

    virtual void set_current_calibration_data(float scale) = 0;
    virtual void get_current_calibration_data(float& scale) const = 0;

    virtual void set_voltage_calibration_data(float scale) = 0;
    virtual void get_voltage_calibration_data(float& scale) const = 0;

    //----------------------------------------------------------------------
    //sensors

    virtual auto get_accelerometer_samples() const  -> std::vector<sensor::Accelerometer_Sample> const& = 0;
    virtual auto get_gyroscope_samples() const      -> std::vector<sensor::Gyroscope_Sample> const& = 0;
    virtual auto get_compass_samples() const        -> std::vector<sensor::Compass_Sample> const& = 0;
    virtual auto get_barometer_samples() const      -> std::vector<sensor::Barometer_Sample> const& = 0;
    virtual auto get_sonar_samples() const          -> std::vector<sensor::Sonar_Sample> const& = 0;
    virtual auto get_thermometer_samples() const    -> std::vector<sensor::Thermometer_Sample> const& = 0;
    virtual auto get_voltmeter_samples() const        -> std::vector<sensor::Voltmeter_Sample> const& = 0;
    virtual auto get_ammeter_samples() const        -> std::vector<sensor::Ammeter_Sample> const& = 0;
    virtual auto get_gps_samples() const            -> std::vector<sensor::GPS_Sample> const& = 0;

    virtual auto get_last_accelerometer_sample() const  -> sensor::Accelerometer_TP_Sample const& = 0;
    virtual auto get_last_gyroscope_sample() const      -> sensor::Gyroscope_TP_Sample const& = 0;
    virtual auto get_last_compass_sample() const        -> sensor::Compass_TP_Sample const& = 0;
    virtual auto get_last_barometer_sample() const      -> sensor::Barometer_TP_Sample const& = 0;
    virtual auto get_last_sonar_sample() const          -> sensor::Sonar_TP_Sample const& = 0;
    virtual auto get_last_thermometer_sample() const    -> sensor::Thermometer_TP_Sample const& = 0;
    virtual auto get_last_voltage_sample() const        -> sensor::Voltmeter_TP_Sample const& = 0;
    virtual auto get_last_current_sample() const        -> sensor::Ammeter_TP_Sample const& = 0;
    virtual auto get_last_gps_sample() const            -> sensor::GPS_TP_Sample const& = 0;

    //----------------------------------------------------------------------
    virtual void process() = 0;
};

}
