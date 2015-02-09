#pragma once

#include "buses/II2C.h"
#include "buses/ISPI.h"
#include "buses/IUART.h"

#include "sensors/ICamera.h"
#include "sensors/IAccelerometer.h"
#include "sensors/IGyroscope.h"
#include "sensors/ICompass.h"
#include "sensors/IBarometer.h"
#include "sensors/IThermometer.h"
#include "sensors/IADC.h"
#include "sensors/IVoltmeter.h"
#include "sensors/IAmmeter.h"
#include "sensors/IGPS.h"
#include "sensors/ISonar.h"

#include "actuators/IGimbal.h"
#include "actuators/IMotors.h"

#include "rapidjson/document.h"

namespace silk
{

class IHAL : q::util::Noncopyable
{
public:
    virtual ~IHAL() {}

    virtual auto init() -> bool = 0;
    virtual void process() = 0;
    virtual void shutdown() = 0;

    //busses
    virtual auto get_all_i2cs() const       -> std::vector<buses::II2C*> const& = 0;
    virtual auto find_i2c_by_name(q::String const& name) const -> buses::II2C* = 0;

    virtual auto get_all_spis() const       -> std::vector<buses::ISPI*> const& = 0;
    virtual auto find_spi_by_name(q::String const& name) const -> buses::ISPI* = 0;

    virtual auto get_all_uarts() const      -> std::vector<buses::IUART*> const& = 0;
    virtual auto find_uart_by_name(q::String const& name) const -> buses::IUART* = 0;

    //sensors
    virtual auto get_all_cameras() const        -> std::vector<sensors::ICamera*> const& = 0;
    virtual auto find_camera_by_name(q::String const& name) const -> sensors::ICamera* = 0;

    virtual auto get_all_accelerometers() const -> std::vector<sensors::IAccelerometer*> const& = 0;
    virtual auto find_accelerometer_by_name(q::String const& name) const -> sensors::IAccelerometer* = 0;

    virtual auto get_all_gyroscopes() const     -> std::vector<sensors::IGyroscope*> const& = 0;
    virtual auto find_gyroscope_by_name(q::String const& name) const -> sensors::IGyroscope* = 0;

    virtual auto get_all_compasses() const      -> std::vector<sensors::ICompass*> const& = 0;
    virtual auto find_compass_by_name(q::String const& name) const -> sensors::ICompass* = 0;

    virtual auto get_all_barometers() const     -> std::vector<sensors::IBarometer*> const& = 0;
    virtual auto find_barometer_by_name(q::String const& name) const -> sensors::IBarometer* = 0;

    virtual auto get_all_thermometers() const   -> std::vector<sensors::IThermometer*> const& = 0;
    virtual auto find_thermometer_by_name(q::String const& name) const -> sensors::IThermometer* = 0;

    virtual auto get_all_adcs() const     -> std::vector<sensors::IADC*> const& = 0;
    virtual auto find_adc_by_name(q::String const& name) const -> sensors::IADC* = 0;

    virtual auto get_all_voltmeters() const     -> std::vector<sensors::IVoltmeter*> const& = 0;
    virtual auto find_voltmeter_by_name(q::String const& name) const -> sensors::IVoltmeter* = 0;

    virtual auto get_all_ammeters() const       -> std::vector<sensors::IAmmeter*> const& = 0;
    virtual auto find_ammeter_by_name(q::String const& name) const -> sensors::IAmmeter* = 0;

    virtual auto get_all_gpses() const          -> std::vector<sensors::IGPS*> const& = 0;
    virtual auto find_gps_by_name(q::String const& name) const -> sensors::IGPS* = 0;

    virtual auto get_all_sonars() const         -> std::vector<sensors::ISonar*> const& = 0;
    virtual auto find_sonar_by_name(q::String const& name) const -> sensors::ISonar* = 0;

    //actuators
    virtual auto get_motors() const             -> actuators::IMotors* const& = 0;

    virtual auto get_all_gimbals() const        -> std::vector<actuators::IGimbal*> const& = 0;
    virtual auto find_gimbal_by_name(q::String const& name) const -> actuators::IGimbal* = 0;

    //settings
    virtual auto get_settings(q::Path const& path) -> rapidjson::Value& = 0;
    virtual void save_settings() const = 0;
};


}
