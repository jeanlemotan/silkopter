#pragma once

#include "common/node/IBarometer.h"
#include "common/node/IThermometer.h"
#include "common/node/II2C.h"
#include "common/node/ISPI.h"

namespace silk
{
namespace hw_device
{

class MS5611 : q::util::Noncopyable
{
public:
    MS5611(q::String const& name);

    struct Init_Params
    {
        size_t rate = 100;
        size_t pressure_to_temperature_ratio = 10;
    };

    auto init(node::II2C* bus, Init_Params const& params) -> bool;
    auto init(node::ISPI* bus, Init_Params const& params) -> bool;

    void process();

    auto get_barometer() -> node::IBarometer&;
    auto get_thermometer() -> node::IThermometer&;

private:
    auto init(Init_Params const& params) -> bool;

    void lock();
    void unlock();
    auto bus_read(uint8_t reg, uint8_t* data, uint32_t size) -> bool;
    auto bus_read_u8(uint8_t reg, uint8_t& dst) -> bool;
    auto bus_read_u16(uint8_t reg, uint16_t& dst) -> bool;
    auto bus_write(uint8_t reg, uint8_t const* data, uint32_t size) -> bool;
    auto bus_write_u8(uint8_t reg, uint8_t const& t) -> bool;
    auto bus_write_u16(uint8_t reg, uint16_t const& t) -> bool;

    node::II2C* m_i2c = nullptr;
    node::ISPI* m_spi = nullptr;

    Init_Params m_params;

    struct Barometer : public node::IBarometer
    {
        auto get_name() const -> q::String const& { return name; }
        auto get_samples() const -> std::vector<node::IBarometer::Sample> const& { return samples; }

        q::String name;
        std::vector<node::IBarometer::Sample> samples;
        uint32_t sample_idx = 0;
        double      reading = 0;
    } m_barometer;
    struct Thermometer : public node::IThermometer
    {
        auto get_name() const -> q::String const& { return name; }
        auto get_samples() const -> std::vector<node::IThermometer::Sample> const& { return samples; }

        q::String name;
        std::vector<node::IThermometer::Sample> samples;
        uint32_t sample_idx = 0;
        double      reading = 0;
    } m_thermometer;

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


DECLARE_CLASS_PTR(MS5611);

}
}
