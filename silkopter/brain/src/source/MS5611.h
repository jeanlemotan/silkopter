#pragma once

#include "HAL.h"
#include "common/node/ISource.h"
#include "common/node/stream/IPressure.h"
#include "common/node/stream/ITemperature.h"
#include "common/node/bus/II2C.h"
#include "common/node/bus/ISPI.h"

namespace sz
{
namespace MS5611
{
struct Init_Params;
struct Config;
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

    auto init(rapidjson::Value const& init_params) -> bool;
    auto get_init_params() const -> rapidjson::Document;

    auto set_config(rapidjson::Value const& json) -> bool;
    auto get_config() const -> rapidjson::Document;

    auto get_outputs() const -> std::vector<Output>;

    void process();

private:
    auto init() -> bool;

    HAL& m_hal;
    bus::II2C_wptr m_i2c;
    bus::ISPI_wptr m_spi;

    struct Buses
    {
        bus::II2C_ptr i2c;
        bus::ISPI_ptr spi;
    };

    auto lock(Buses& buses) -> bool;
    void unlock(Buses& buses);
    auto bus_read(Buses& buses, uint8_t reg, uint8_t* data, uint32_t size) -> bool;
    auto bus_read_u8(Buses& buses, uint8_t reg, uint8_t& dst) -> bool;
    auto bus_read_u16(Buses& buses, uint8_t reg, uint16_t& dst) -> bool;
    auto bus_write(Buses& buses, uint8_t reg, uint8_t const* data, uint32_t size) -> bool;
    auto bus_write_u8(Buses& buses, uint8_t reg, uint8_t const& t) -> bool;
    auto bus_write_u16(Buses& buses, uint8_t reg, uint16_t const& t) -> bool;

    std::shared_ptr<sz::MS5611::Init_Params> m_init_params;
    std::shared_ptr<sz::MS5611::Config> m_config;

    struct Common
    {
        double      reading = 0;
        uint32_t rate = 0;
    };

    struct Pressure : public stream::IPressure, public Common
    {
        auto get_samples() const -> std::vector<Sample> const& { return samples; }
        auto get_rate() const -> uint32_t { return rate; }

        std::vector<Sample> samples;
        Sample last_sample;
    };
    mutable std::shared_ptr<Pressure> m_pressure;

    struct Temperature : public stream::ITemperature, public Common
    {
        auto get_samples() const -> std::vector<Sample> const& { return samples; }
        auto get_rate() const -> uint32_t { return rate; }

        std::vector<Sample> samples;
        Sample last_sample;
    };
    mutable std::shared_ptr<Temperature> m_temperature;

    double		m_c1 = 0;
    double		m_c2 = 0;
    double		m_c3 = 0;
    double		m_c4 = 0;
    double		m_c5 = 0;
    double		m_c6 = 0;

    void calculate(q::Clock::duration dt);

    uint8_t         m_stage = 0;

    q::Clock::time_point m_last_timestamp;
    q::Clock::duration m_dt;
};


}
}
