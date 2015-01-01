#pragma once

#include "i2c.h"

namespace silk
{

class OdroidW_ADC : public q::util::Noncopyable
{
public:
    auto init() -> bool;

    void process();

    struct Data
    {
        float value;
        q::Clock::duration dt;
    };

    auto get_current_data() -> boost::optional<Data>;
    auto get_voltage_data() -> boost::optional<Data>;

private:
    i2c m_i2c;

    struct ADC
    {
        boost::optional<Data> data;
        q::Clock::time_point last_time_point = q::Clock::now();
    };

    q::Clock::time_point m_last_time_point = q::Clock::now();
    size_t m_stage = 0;

    ADC m_adc_current;
    ADC m_adc_voltage;
};

}




