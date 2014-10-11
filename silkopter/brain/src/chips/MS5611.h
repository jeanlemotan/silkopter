#pragma once

#include "i2c.h"

namespace silk
{

class MS5611 : public q::util::Noncopyable
{
public:
    auto init(q::String const& device) -> bool;

    void process();

    auto read_pressure() -> boost::optional<float>;
    auto read_temperature() -> boost::optional<float>;

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

    boost::optional<float> m_pressure{0.0};
    boost::optional<float> m_temperature{0.0};

    void calculate();

    uint8_t         m_stage = 0;

    q::Clock::time_point m_last_timestamp;
};

}
