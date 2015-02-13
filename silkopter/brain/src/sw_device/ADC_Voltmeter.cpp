#include "BrainStdAfx.h"
#include "ADC_Voltmeter.h"

namespace silk
{
namespace sw_device
{

ADC_Voltmeter::ADC_Voltmeter(q::String const& name)
    : m_name(name)
{
}

auto ADC_Voltmeter::init(node::IADC* adc, Init_Params const& params) -> bool
{
    if (!adc)
    {
        QLOGE("No adc specified");
        return false;
    }

    m_adc = adc;
}

auto ADC_Voltmeter::get_name() const -> q::String const&
{
    return m_name;
}
auto ADC_Voltmeter::get_samples() const -> std::vector<node::IVoltmeter::Sample> const&
{
    return m_samples;
}

void ADC_Voltmeter::process()
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
       node::IVoltmeter::Sample vs;
       vs.dt = adc_s.dt;
       vs.sample_idx = adc_s.sample_idx;
       vs.value = adc_s.value;
       return vs;
    });

    //TODO - apply scale - bias
}


}
}
