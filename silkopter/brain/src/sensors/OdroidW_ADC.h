#pragma once

#include "i2c.h"

namespace silk
{

class OdroidW_ADC : public q::util::Noncopyable
{
public:
    auto init() -> bool;

    void process();

    auto read_current() -> boost::optional<float>;
    auto read_voltage() -> boost::optional<float>;

    auto get_current_sample_time() const -> q::Clock::duration;
    auto get_voltage_sample_time() const -> q::Clock::duration;

private:
    i2c m_i2c;

    struct ADC
    {
        boost::optional<float> data;
        q::Clock::duration sample_time = q::Clock::duration(0);
        q::Clock::time_point last_time_point = q::Clock::now();
    };

    q::Clock::time_point m_last_time_point = q::Clock::now();
    size_t m_stage = 0;

    ADC m_adc_current;
    ADC m_adc_voltage;
};

}




