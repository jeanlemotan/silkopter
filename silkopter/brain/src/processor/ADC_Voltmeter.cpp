#include "BrainStdAfx.h"
#include "ADC_Voltmeter.h"

namespace silk
{
namespace node
{
namespace processor
{

ADC_Voltmeter::ADC_Voltmeter(HAL& hal)
    : m_hal(hal)
{
}

auto ADC_Voltmeter::init(Init_Params const& params) -> bool
{
    m_params = params;

    m_stream.source_stream = m_hal.get_streams().find_by_name<stream::IADC_Value>(params.source_stream);
    if (!init(params) ||
        !m_hal.get_processors().add<Base>(params.name, *this) ||
        !m_hal.get_streams().add<stream::IVoltage>(q::util::format2<std::string>("{}/stream", params.name), m_stream))
    {
        return false;
    }
    return true;
}

auto ADC_Voltmeter::init() -> bool
{
    if (!m_stream.source_stream)
    {
        QLOGE("No source specified");
        return false;
    }
}

auto ADC_Voltmeter::get_input_stream() -> stream::IADC_Value&
{
    QASSERT(m_stream.source_stream);
    return *m_stream.source_stream;
}
auto ADC_Voltmeter::get_output_stream() -> stream::IVoltage&
{
    return m_stream;
}


void ADC_Voltmeter::process()
{
    m_stream.samples.clear();
    auto const& s = get_input_stream().get_samples();
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
