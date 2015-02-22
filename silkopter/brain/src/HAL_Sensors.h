#pragma once

//#include "common/node/source/IAccelerometer.h"
//#include "common/node/source/IGyroscope.h"
//#include "common/node/source/IMagnetometer.h"
//#include "common/node/source/IBarometer.h"
//#include "common/node/source/IThermometer.h"
//#include "common/node/source/ISonar.h"
//#include "common/node/source/IGPS.h"
//#include "common/node/source/IAmmeter.h"
//#include "common/node/source/IVoltmeter.h"

//namespace silk
//{

////----------------------------------------------------------------------

//class HAL_Sensors : q::util::Noncopyable
//{
//public:
//    virtual ~HAL_Sensors() {}

//    virtual auto init() -> bool = 0;
//    virtual void shutdown() = 0;

//    //----------------------------------------------------------------------
//    //calibration
//    virtual void set_accelerometer_calibration_data(math::vec3f const& bias, math::vec3f const& scale) = 0;
//    virtual void get_accelerometer_calibration_data(math::vec3f& bias, math::vec3f& scale) const = 0;

//    virtual void set_gyroscope_calibration_data(math::vec3f const& bias) = 0;
//    virtual void get_gyroscope_calibration_data(math::vec3f& bias) const = 0;

//    virtual void set_compass_calibration_data(math::vec3f const& bias) = 0;
//    virtual void get_compass_calibration_data(math::vec3f& bias) const = 0;

//    virtual void set_current_calibration_data(float scale) = 0;
//    virtual void get_current_calibration_data(float& scale) const = 0;

//    virtual void set_voltage_calibration_data(float scale) = 0;
//    virtual void get_voltage_calibration_data(float& scale) const = 0;

//    //----------------------------------------------------------------------
//    //sensors

//    virtual auto get_accelerometer_samples() const  -> std::vector<node::IAccelerometer::Sample> const& = 0;
//    virtual auto get_gyroscope_samples() const      -> std::vector<node::IGyroscope::Sample> const& = 0;
//    virtual auto get_compass_samples() const        -> std::vector<node::ICompass::Sample> const& = 0;
//    virtual auto get_barometer_samples() const      -> std::vector<node::IBarometer::Sample> const& = 0;
//    virtual auto get_sonar_samples() const          -> std::vector<node::ISonar::Sample> const& = 0;
//    virtual auto get_thermometer_samples() const    -> std::vector<node::IThermometer::Sample> const& = 0;
//    virtual auto get_voltmeter_samples() const        -> std::vector<node::IVoltmeter::Sample> const& = 0;
//    virtual auto get_ammeter_samples() const        -> std::vector<node::IAmmeter::Sample> const& = 0;
//    virtual auto get_gps_samples() const            -> std::vector<node::IGPS::Sample> const& = 0;

//    virtual auto get_last_accelerometer_sample() const  -> node::IAccelerometer::TP_Sample const& = 0;
//    virtual auto get_last_gyroscope_sample() const      -> node::IGyroscope::TP_Sample const& = 0;
//    virtual auto get_last_compass_sample() const        -> node::ICompass::TP_Sample const& = 0;
//    virtual auto get_last_barometer_sample() const      -> node::IBarometer::TP_Sample const& = 0;
//    virtual auto get_last_sonar_sample() const          -> node::ISonar::TP_Sample const& = 0;
//    virtual auto get_last_thermometer_sample() const    -> node::IThermometer::TP_Sample const& = 0;
//    virtual auto get_last_voltage_sample() const        -> node::IVoltmeter::TP_Sample const& = 0;
//    virtual auto get_last_current_sample() const        -> node::IAmmeter::TP_Sample const& = 0;
//    virtual auto get_last_gps_sample() const            -> node::IGPS::TP_Sample const& = 0;

//    //----------------------------------------------------------------------
//    virtual void process() = 0;
//};

//}
