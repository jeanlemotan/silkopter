#include "BrainStdAfx.h"
#include "ADC_Ammeter.h"

namespace silk
{
namespace sw_device
{

ADC_Ammeter::ADC_Ammeter(q::String const& name)
    : m_name(name)
{
}

auto ADC_Ammeter::init(node::IADC* adc, Init_Params const& params) -> bool
{
    if (!adc)
    {
        QLOGE("No adc specified");
        return false;
    }

    m_adc = adc;
}

auto ADC_Ammeter::get_name() const -> q::String const&
{
    return m_name;
}
auto ADC_Ammeter::get_samples() const -> std::vector<node::IAmmeter::Sample> const&
{
    return m_samples;
}

void ADC_Ammeter::process()
{
    QASSERT(m_adc);
    if (!m_adc)
    {
        return;
    }
    m_samples.clear();
    auto const& s = m_adc->get_samples();
    m_samples.resize(s.size());

    std::transform(s.begin(), s.end(), m_samples.begin(), [](node::IADC::Sample const& adc_s)
    {
       node::IAmmeter::Sample vs;
       vs.dt = adc_s.dt;
       vs.sample_idx = adc_s.sample_idx;
       vs.value = adc_s.value;
       return vs;
    });

    //TODO - apply scale - bias
}


}
}
