#pragma once

#include "IHAL.h"
#include "sensors/Bus_Config.h"
#include "rapidjson/document.h"

namespace silk
{

class HAL_Pi : public IHAL
{
public:
    HAL_Pi();
    ~HAL_Pi();

    auto init() -> bool;
    void process();
    void shutdown();

    auto get_all_i2cs() const       -> std::vector<buses::II2C*> const&;
    auto find_i2c_by_name(q::String const& name) const -> buses::II2C*;

    auto get_all_spis() const       -> std::vector<buses::ISPI*> const&;
    auto find_spi_by_name(q::String const& name) const -> buses::ISPI*;

    auto get_all_uarts() const      -> std::vector<buses::IUART*> const&;
    auto find_uart_by_name(q::String const& name) const -> buses::IUART*;

    //sensors
    auto get_all_cameras() const        -> std::vector<sensors::ICamera*> const&;
    auto find_camera_by_name(q::String const& name) const -> sensors::ICamera*;

    auto get_all_accelerometers() const -> std::vector<sensors::IAccelerometer*> const&;
    auto find_accelerometer_by_name(q::String const& name) const -> sensors::IAccelerometer*;

    auto get_all_gyroscopes() const     -> std::vector<sensors::IGyroscope*> const&;
    auto find_gyroscope_by_name(q::String const& name) const -> sensors::IGyroscope*;

    auto get_all_compasses() const      -> std::vector<sensors::ICompass*> const&;
    auto find_compass_by_name(q::String const& name) const -> sensors::ICompass*;

    auto get_all_barometers() const     -> std::vector<sensors::IBarometer*> const&;
    auto find_barometer_by_name(q::String const& name) const -> sensors::IBarometer*;

    auto get_all_thermometers() const   -> std::vector<sensors::IThermometer*> const&;
    auto find_thermometer_by_name(q::String const& name) const -> sensors::IThermometer*;

    auto get_all_adcs() const     -> std::vector<sensors::IADC*> const&;
    auto find_adc_by_name(q::String const& name) const -> sensors::IADC*;

    auto get_all_voltmeters() const     -> std::vector<sensors::IVoltmeter*> const&;
    auto find_voltmeter_by_name(q::String const& name) const -> sensors::IVoltmeter*;

    auto get_all_ammeters() const       -> std::vector<sensors::IAmmeter*> const&;
    auto find_ammeter_by_name(q::String const& name) const -> sensors::IAmmeter*;

    auto get_all_gpses() const          -> std::vector<sensors::IGPS*> const&;
    auto find_gps_by_name(q::String const& name) const -> sensors::IGPS*;

    auto get_all_sonars() const         -> std::vector<sensors::ISonar*> const&;
    auto find_sonar_by_name(q::String const& name) const -> sensors::ISonar*;

    //actuators
    auto get_motors() const             -> actuators::IMotors* const&;

    auto get_all_gimbals() const        -> std::vector<actuators::IGimbal*> const&;
    auto find_gimbal_by_name(q::String const& name) const -> actuators::IGimbal*;

    auto get_settings(q::Path const& path) -> rapidjson::Value&;
    void save_settings() const;

private:
    auto is_bus_name_unique(q::String const& name) const -> bool;
    auto is_sensor_name_unique(q::String const& name) const -> bool;
    auto is_actuator_name_unique(q::String const& name) const -> bool;

    bool m_is_initialized = false;

    rapidjson::Value m_emptyValue;
    rapidjson::Document m_settings;

    struct Hardware;
    std::shared_ptr<Hardware> m_hw;

    auto load_settings() -> bool;
};

}
