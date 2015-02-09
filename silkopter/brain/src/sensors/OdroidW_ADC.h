#pragma once

#include "i2c.h"
#include "IVoltage.h"
#include "ICurrent.h"

namespace silk
{
namespace sensors
{

class OdroidW_ADC : public IVoltage, public ICurrent, q::util::Noncopyable
{
public:
    auto init() -> bool;

    void process();

    auto get_voltage_samples() const -> std::vector<Voltage_Sample> const&;
    auto get_current_samples() const -> std::vector<Current_Sample> const&;

private:
    i2c m_i2c;

    q::Clock::time_point m_last_time_point = q::Clock::now();
    size_t m_stage = 0;

    struct ADC_V
    {
        std::vector<Voltage_Sample> samples;
        q::Clock::time_point last_time_point = q::Clock::now();
        uint32_t sample_idx = 0;
    } m_adc_voltage;


    struct ADC_C
    {
        std::vector<Current_Sample> samples;
        q::Clock::time_point last_time_point = q::Clock::now();
        uint32_t sample_idx = 0;
    } m_adc_current;

};

}
}
