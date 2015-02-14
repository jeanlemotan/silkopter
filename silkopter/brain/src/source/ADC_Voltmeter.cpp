#include "BrainStdAfx.h"
#include "ADC_Voltmeter.h"

namespace silk
{
namespace node
{
namespace source
{

ADC_Voltmeter::ADC_Voltmeter(HAL& hal)
    : m_hal(hal)
{
}

auto ADC_Voltmeter::init(Init_Params const& params) -> bool
{
    m_params = params;

    m_source_stream = m_hal.get_streams().find_by_name<stream::IADC_Value>(params.source_stream);
    if (!init(params) ||
        !m_hal.get_sources().add<IVoltmeter>(params.name, *this) ||
        !m_hal.get_streams().add<stream::IVoltage>(q::util::format2<q::String>("{}/stream", params.name), m_stream))
    {
        return false;
    }
    return true;
}

auto ADC_Voltmeter::init() -> bool
{
    if (!m_source_stream)
    {
        QLOGE("No source specified");
        return false;
    }
}

auto ADC_Voltmeter::get_stream() -> stream::IVoltage&
{
    return m_stream;
}


void ADC_Voltmeter::process()
{
    QASSERT(m_source_stream);
    if (!m_source_stream)
    {
        return;
    }
    m_stream.samples.clear();
    auto const& s = m_source_stream->get_samples();
    m_stream.samples.resize(s.size());

    std::transform(s.begin(), s.end(), m_stream.samples.begin(), [](stream::IADC_Value::Sample const& sample)
    {
       Stream::Sample vs;
       vs.dt = sample.dt;
       vs.sample_idx = sample.sample_idx;
       vs.value = sample.value;
       return vs;
    });

    //TODO - apply scale - bias
}


}
}
}
