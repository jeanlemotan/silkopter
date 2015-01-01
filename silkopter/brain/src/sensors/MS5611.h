#pragma once

#include "i2c.h"

namespace silk
{

class MS5611 : public q::util::Noncopyable
{
public:
    auto init(std::string const& device) -> bool;

    void process();

    struct Data
    {
        float value;
        q::Clock::duration dt;
    };

    auto get_barometer_data() -> boost::optional<Data>;
    auto get_thermometer_data() -> boost::optional<Data>;

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

    boost::optional<Data> m_pressure;
    boost::optional<Data> m_temperature;

    void calculate(q::Clock::duration dt);

    uint8_t         m_stage = 0;

    q::Clock::time_point m_last_timestamp;
};

}
