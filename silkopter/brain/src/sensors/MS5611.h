#pragma once

#include "IBarometer.h"
#include "IThermometer.h"
#include "buses/II2C.h"
#include "buses/ISPI.h"

namespace silk
{
namespace sensors
{

class MS5611 : public IBarometer, public IThermometer, q::util::Noncopyable
{
public:
    MS5611(q::String const& name);

    struct Params
    {
        size_t rate = 100;
        size_t pressure_to_temperature_ratio = 10;
    };

    auto init(buses::II2C* bus, Params const& params) -> bool;
    auto init(buses::ISPI* bus, Params const& params) -> bool;

    void process();

    auto get_barometer_name() const -> q::String const&;
    auto get_thermometer_name() const -> q::String const&;

    auto get_barometer_samples() const -> std::vector<Barometer_Sample> const&;
    auto get_thermometer_samples() const -> std::vector<Thermometer_Sample> const&;

private:
    auto init(Params const& params) -> bool;

    void lock();
    void unlock();
    auto bus_read(uint8_t reg, uint8_t* data, uint32_t size) -> bool;
    auto bus_read_u8(uint8_t reg, uint8_t& dst) -> bool;
    auto bus_read_u16(uint8_t reg, uint16_t& dst) -> bool;
    auto bus_write(uint8_t reg, uint8_t const* data, uint32_t size) -> bool;
    auto bus_write_u8(uint8_t reg, uint8_t const& t) -> bool;
    auto bus_write_u16(uint8_t reg, uint16_t const& t) -> bool;

    buses::II2C* m_i2c = nullptr;
    buses::ISPI* m_spi = nullptr;

    Params m_params;

    q::String m_barometer_name;
    q::String m_thermometer_name;

    double		m_c1 = 0;
    double		m_c2 = 0;
    double		m_c3 = 0;
    double		m_c4 = 0;
    double		m_c5 = 0;
    double		m_c6 = 0;

    double      m_pressure_reading = 0;
    double      m_temperature_reading = 0;

    uint32_t m_thermometer_sample_idx = 0;
    uint32_t m_barometer_sample_idx = 0;

    std::vector<Barometer_Sample> m_pressures;
    std::vector<Thermometer_Sample> m_temperatures;

    void calculate(q::Clock::duration dt);

    uint8_t         m_stage = 0;

    q::Clock::time_point m_last_timestamp;
    q::Clock::duration m_dt;
};


DECLARE_CLASS_PTR(MS5611);

}
}
