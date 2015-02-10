#pragma once

#include "IADC.h"
#include "IAmmeter.h"
#include "buses/II2C.h"

namespace silk
{
namespace sensors
{

class RC5T619 : q::util::Noncopyable
{
public:
    RC5T619(q::String const& name);

    struct Params
    {
        size_t adc0_rate = 50;
        size_t adc1_ratio = 10;
    };

    auto init(buses::II2C* bus, Params const& params) -> bool;

    void process();

    auto get_adc0() -> IADC&;
    auto get_adc1() -> IADC&;

private:
    auto init(Params const& params) -> bool;

    buses::II2C* m_i2c = nullptr;

    Params m_params;

    q::Clock::time_point m_last_time_point = q::Clock::now();
    q::Clock::duration m_dt;

    size_t m_stage = 0;

    struct ADC : public IADC
    {
        auto get_name() const -> q::String const& { return name; }
    //    void set_adc_config(Config const& config);
    //    auto get_adc_config() const -> Config const&;
        auto get_samples() const -> std::vector<ADC_Sample> const& { return samples; }

        q::String name;
        std::vector<ADC_Sample> samples;
        q::Clock::time_point last_time_point = q::Clock::now();
        uint32_t sample_idx = 0;
    } m_adc[2];
};


DECLARE_CLASS_PTR(RC5T619);

}
}
