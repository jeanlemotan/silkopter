#pragma once

#include "i2c.h"

namespace silk
{

class OdroidW_ADC : public q::util::Noncopyable
{
public:
    auto init(q::Clock::duration adc0_sample_time, q::Clock::duration adc1_sample_time) -> bool;

    void process();

    auto read_adc0_sample() -> boost::optional<float>;
    auto read_adc1_sample() -> boost::optional<float>;

    auto get_adc0_sample_time() const -> q::Clock::duration;
    auto get_adc1_sample_time() const -> q::Clock::duration;

private:
    i2c m_i2c;

    auto read_sample(size_t idx) -> boost::optional<float>;

    struct ADC
    {
        boost::optional<float> data;
        q::Clock::duration sample_time;
        q::Clock::time_point last_time_point;
    };

    ADC m_adc0;
    ADC m_adc1;
};

}




