#pragma once

#include "IBarometer.h"
#include "IThermometer.h"
#include "i2c.h"

namespace silk
{
namespace sensors
{

class MS5611 : public IBarometer, public IThermometer, q::util::Noncopyable
{
public:
    auto init(std::string const& device) -> bool;

    void process();

    struct Data
    {
        float value;
        q::Clock::duration dt;
    };

    auto get_barometer_samples() const -> std::vector<Barometer_Sample> const&;
    auto get_thermometer_samples() const -> std::vector<Thermometer_Sample> const&;

private:
    i2c m_i2c;

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
};


DECLARE_CLASS_PTR(MS5611);

}
}
