#pragma once

#include "interface/IADC.h"
#include "interface/IAmmeter.h"
#include "interface/II2C.h"

namespace silk
{
namespace device
{

class RC5T619 : q::util::Noncopyable
{
public:
    RC5T619(q::String const& name);

    struct Init_Params
    {
        size_t adc0_rate = 50;
        size_t adc1_ratio = 10;
    };

    auto init(bus::II2C* bus, Init_Params const& params) -> bool;

    void process();

    auto get_adc0() -> sensor::IADC&;
    auto get_adc1() -> sensor::IADC&;

private:
    auto init(Init_Params const& params) -> bool;

    bus::II2C* m_i2c = nullptr;

    Init_Params m_params;

    q::Clock::time_point m_last_time_point = q::Clock::now();
    q::Clock::duration m_dt;

    size_t m_stage = 0;

    struct ADC : public sensor::IADC
    {
        auto get_name() const -> q::String const& { return name; }
    //    void set_adc_config(Config const& config);
    //    auto get_adc_config() const -> Config const&;
        auto get_samples() const -> std::vector<sensor::ADC_Sample> const& { return samples; }

        q::String name;
        std::vector<sensor::ADC_Sample> samples;
        q::Clock::time_point last_time_point = q::Clock::now();
        uint32_t sample_idx = 0;
    } m_adc[2];
};


DECLARE_CLASS_PTR(RC5T619);

}
}
