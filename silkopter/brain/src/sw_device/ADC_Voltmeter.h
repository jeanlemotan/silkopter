#pragma once

#include "common/node/IADC.h"
#include "common/node/IVoltmeter.h"

namespace silk
{
namespace sw_device
{

class ADC_Voltmeter : public node::IVoltmeter
{
public:
    ADC_Voltmeter(q::String const& name);

    struct Init_Params
    {

    };

    auto init(node::IADC* adc, Init_Params const& params) -> bool;

    auto get_name() const -> q::String const&;
    auto get_samples() const -> std::vector<node::IVoltmeter::Sample> const&;

    void process();

private:
    node::IADC* m_adc = nullptr;
    q::String m_name;
    std::vector<node::IVoltmeter::Sample> m_samples;
    uint32_t m_sample_idx = 0;
};


DECLARE_CLASS_PTR(ADC_Voltmeter);

}
}
