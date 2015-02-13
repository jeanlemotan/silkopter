#include "BrainStdAfx.h"
#include "ADC_Voltmeter.h"

namespace silk
{
namespace node
{
namespace source
{

ADC_Voltmeter::ADC_Voltmeter()
{
}

auto ADC_Voltmeter::init(stream::IADC_Value* stream, Init_Params const& params) -> bool
{
    if (!stream)
    {
        QLOGE("No source specified");
        return false;
    }

    m_source_stream = stream;
}

auto ADC_Voltmeter::get_stream() const -> stream::IVoltage const&
{
//    return m_stream;
}


void ADC_Voltmeter::process()
{
    QASSERT(m_source_stream);
    if (!m_source_stream)
    {
        return;
    }
//    m_stream.samples.clear();
//    auto const& s = m_source->get_samples();
//    m_stream.samples.resize(s.size());

//    std::transform(s.begin(), s.end(), m_stream.samples.begin(), [](node::IADC_Value_Stream::Sample const& sample)
//    {
//       Stream::Sample vs;
//       vs.dt = sample.dt;
//       vs.sample_idx = sample.sample_idx;
//       vs.value = sample.value;
//       return vs;
//    });

    //TODO - apply scale - bias
}


}
}
}
