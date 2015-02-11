#pragma once

#include "common/node/IADC.h"
#include "common/node/IAmmeter.h"

namespace silk
{
namespace sw_device
{

class ADC_Ammeter : public node::IAmmeter
{
public:
    ADC_Ammeter(q::String const& name);

    struct Init_Params
    {

    };

    auto init(node::IADC* adc, Init_Params const& params) -> bool;

    auto get_name() const -> q::String const&;
    auto get_samples() const -> std::vector<node::IADC::Sample> const&;

    void process();

private:
    node::IADC* m_adc = nullptr;
    q::String m_name;
    std::vector<node::IADC::Sample> m_samples;
    uint32_t m_sample_idx = 0;
};


DECLARE_CLASS_PTR(ADC_Ammeter);

}
}
