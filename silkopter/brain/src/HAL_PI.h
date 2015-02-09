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

    auto get_all_i2cs() const       -> std::vector<buses::IBus_I2C*> const&;
    auto get_all_spis() const       -> std::vector<buses::IBus_SPI*> const&;
    auto get_all_uarts() const      -> std::vector<buses::IBus_UART*> const&;

    auto get_all_cameras() const        -> std::vector<sensors::ICamera*> const&;
    auto get_all_accelerometers() const -> std::vector<sensors::IAccelerometer*> const&;
    auto get_all_gyroscopes() const     -> std::vector<sensors::IGyroscope*> const&;
    auto get_all_compasses() const      -> std::vector<sensors::ICompass*> const&;
    auto get_all_barometers() const     -> std::vector<sensors::IBarometer*> const&;
    auto get_all_thermometers() const   -> std::vector<sensors::IThermometer*> const&;
    auto get_all_voltmeters() const     -> std::vector<sensors::IVoltmeter*> const&;
    auto get_all_ammeters() const       -> std::vector<sensors::IAmmeter*> const&;
    auto get_all_gpses() const          -> std::vector<sensors::IGPS*> const&;
    auto get_all_sonars() const         -> std::vector<sensors::ISonar*> const&;

    auto get_motors() const             -> actuators::IMotors* const&;
    auto get_all_gimbals() const        -> std::vector<actuators::IGimbal*> const&;

    auto get_settings(q::Path const& path) -> rapidjson::Value&;
    void save_settings() const;

    struct Sensor_Config
    {
        std::string sensor;
        Bus_Config bus_config;
    };

private:
    bool m_is_initialized = false;

    rapidjson::Value m_emptyValue;
    rapidjson::Document m_settings;

    struct Hardware;
    std::shared_ptr<Hardware> m_hw;

    auto load_settings() -> bool;
};

}
