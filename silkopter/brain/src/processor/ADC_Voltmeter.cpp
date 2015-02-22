#include "BrainStdAfx.h"
#include "ADC_Voltmeter.h"

#include "sz_math.hpp"
#include "sz_hal_nodes.hpp"

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

auto ADC_Voltmeter::get_name() const -> std::string const&
{
    return m_params.name;
}

auto ADC_Voltmeter::init(rapidjson::Value const& json) -> bool
{
    sz::ADC_Voltmeter sz;
    autojsoncxx::error::ErrorStack result;
    if (!autojsoncxx::from_value(sz, json, result))
    {
        std::ostringstream ss;
        ss << result;
        QLOGE("Cannot deserialize ADC_Voltmeter data: {}", ss.str());
        return false;
    }
    Init_Params params;
    params.name = sz.name;
    params.input_stream = m_hal.get_streams().find_by_name<stream::IADC_Value>(sz.input_stream);
    return init(params);
}

auto ADC_Voltmeter::init(Init_Params const& params) -> bool
{
    m_params = params;

    if (!init())
    {
        return false;
    }

    if (!m_params.name.empty())
    {
        m_stream.name = q::util::format2<std::string>("{}/stream", m_params.name);
        if (!m_hal.get_streams().add(m_stream))
        {
            return false;
        }
    }
    return true;
}

auto ADC_Voltmeter::init() -> bool
{
    m_stream.params = &m_params;
    if (!m_params.input_stream)
    {
        QLOGE("No input specified");
        return false;
    }
}

auto ADC_Voltmeter::set_config(rapidjson::Value const& json) -> bool
{
    return false;
}
auto ADC_Voltmeter::get_config() -> boost::optional<rapidjson::Value const&>
{
    return boost::none;
}

auto ADC_Voltmeter::get_input_stream_count() const -> size_t
{
    return 1;
}
auto ADC_Voltmeter::get_input_stream(size_t idx) -> stream::IADC_Value&
{
    QASSERT(idx == 0);
    QASSERT(m_params.input_stream);
    return *m_params.input_stream;
}
auto ADC_Voltmeter::get_output_stream_count() const -> size_t
{
    return 1;
}
auto ADC_Voltmeter::get_output_stream(size_t idx) -> stream::IVoltage&
{
    QASSERT(idx == 0);
    return m_stream;
}


void ADC_Voltmeter::process()
{
    m_stream.samples.clear();
    auto const& s = get_input_stream(0).get_samples();
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
