#pragma once

#include "HAL.h"
#include "common/node/ISource.h"
#include "common/stream/IPressure.h"
#include "common/stream/ITemperature.h"
#include "common/bus/II2C.h"
#include "common/bus/ISPI.h"

#include "Basic_Output_Stream.h"


namespace silk
{
namespace hal
{
struct MS5611_Descriptor;
struct MS5611_Config;
}
}


namespace silk
{
namespace node
{

class MS5611 : public ISource
{
public:
    MS5611(HAL& hal);

    ts::Result<void> init(hal::INode_Descriptor const& descriptor) override;
    std::shared_ptr<const hal::INode_Descriptor> get_descriptor() const override;

    ts::Result<void> set_config(hal::INode_Config const& config) override;
    std::shared_ptr<const hal::INode_Config> get_config() const override;

    //auto send_message(rapidjson::Value const& json) -> rapidjson::Document;

    ts::Result<void> start(Clock::time_point tp) override;

    auto get_outputs() const -> std::vector<Output>;

    void process();

private:
    ts::Result<void> init();

    HAL& m_hal;

    std::weak_ptr<bus::II2C> m_i2c;
    std::weak_ptr<bus::ISPI> m_spi;

    struct Buses
    {
        std::shared_ptr<bus::II2C> i2c;
        std::shared_ptr<bus::ISPI> spi;
    };

    auto lock(Buses& buses) -> bool;
    void unlock(Buses& buses);
    auto bus_read_u24(Buses& buses, uint8_t reg, uint32_t& dst) -> bool;
    auto bus_read_u8(Buses& buses, uint8_t reg, uint8_t& dst) -> bool;
    auto bus_read_u16(Buses& buses, uint8_t reg, uint16_t& dst) -> bool;
    auto bus_write(Buses& buses, uint8_t data) -> bool;

    std::shared_ptr<hal::MS5611_Descriptor> m_descriptor;
    std::shared_ptr<hal::MS5611_Config> m_config;

    template<class Base>
    struct Common : public Base
    {
        Common() = default;
        double      reading = 0;
    };

    typedef Basic_Output_Stream<Common<stream::IPressure>> Pressure_Stream;
    mutable std::shared_ptr<Pressure_Stream> m_pressure;

    typedef Basic_Output_Stream<Common<stream::ITemperature>> Temperature_Stream;
    mutable std::shared_ptr<Temperature_Stream> m_temperature;

    double		m_c1 = 0;
    double		m_c2 = 0;
    double		m_c3 = 0;
    double		m_c4 = 0;
    double		m_c5 = 0;
    double		m_c6 = 0;

    enum class Stage
    {
        PRESSURE,
        TEMPERATURE,
        UNKNOWN
    };

    Stage         m_stage = Stage::PRESSURE;
    Clock::time_point m_last_process_tp = Clock::now();
    Clock::time_point m_last_temperature_reading_tp = Clock::now();
    Clock::time_point m_last_pressure_reading_tp = Clock::now();

    struct Stats
    {
        Clock::time_point last_report_tp = Clock::now();
        struct Pressure
        {
            bool operator==(Pressure const& o) const { return memcmp(this, &o, sizeof(*this)) == 0; }
            bool operator!=(Pressure const& o) const { return !operator==(o); }
            size_t added = 0;
        } pres;
        struct Temperature
        {
            bool operator==(Temperature const& o) const { return memcmp(this, &o, sizeof(*this)) == 0; }
            bool operator!=(Temperature const& o) const { return !operator==(o); }
            size_t added = 0;
        } temp;

        size_t bus_failures = 0;

        bool operator==(Stats const& o) const { return pres == o.pres && temp == o.temp && bus_failures == o.bus_failures; }
        bool operator!=(Stats const& o) const { return !operator==(o); }
    } m_stats;

};


}
}
