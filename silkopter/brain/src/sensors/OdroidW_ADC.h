#pragma once

#include "i2c.h"
#include "IVoltmeter.h"
#include "IAmmeter.h"

namespace silk
{
namespace sensors
{

class OdroidW_ADC : public IVoltmeter, public IAmmeter, q::util::Noncopyable
{
public:
    auto init() -> bool;

    void process();

    auto get_voltmeter_samples() const -> std::vector<Voltmeter_Sample> const&;
    auto get_ammeter_samples() const -> std::vector<Ammeter_Sample> const&;

private:
    i2c m_i2c;

    q::Clock::time_point m_last_time_point = q::Clock::now();
    size_t m_stage = 0;

    struct ADC_V
    {
        std::vector<Voltmeter_Sample> samples;
        q::Clock::time_point last_time_point = q::Clock::now();
        uint32_t sample_idx = 0;
    } m_adc_voltage;


    struct ADC_C
    {
        std::vector<Ammeter_Sample> samples;
        q::Clock::time_point last_time_point = q::Clock::now();
        uint32_t sample_idx = 0;
    } m_adc_current;

};


DECLARE_CLASS_PTR(OdroidW_ADC);

}
}
