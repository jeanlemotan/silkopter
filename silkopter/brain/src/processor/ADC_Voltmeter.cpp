#include "BrainStdAfx.h"
#include "ADC_Voltmeter.h"

#include "sz_math.hpp"
#include "sz_ADC_Voltmeter.hpp"

namespace silk
{
namespace node
{
namespace processor
{

ADC_Voltmeter::ADC_Voltmeter(HAL& hal)
    : m_hal(hal)
    , m_init_params(new sz::ADC_Voltmeter::Init_Params())
    , m_config(new sz::ADC_Voltmeter::Config())
{
}

auto ADC_Voltmeter::get_name() const -> std::string const&
{
    return m_init_params->name;
}

auto ADC_Voltmeter::init(rapidjson::Value const& json) -> bool
{
    QLOG_TOPIC("adc_voltmeter::init");

    sz::ADC_Voltmeter::Init_Params sz;
    autojsoncxx::error::ErrorStack result;
    if (!autojsoncxx::from_value(sz, json, result))
    {
        std::ostringstream ss;
        ss << result;
        QLOGE("Cannot deserialize ADC_Voltmeter data: {}", ss.str());
        return false;
    }
    *m_init_params = sz;
    autojsoncxx::to_document(sz, m_init_params_json);

    return init();
}
auto ADC_Voltmeter::init() -> bool
{
    if (!m_init_params->name.empty())
    {
        m_stream.name = q::util::format2<std::string>("{}/stream", m_init_params->name);
        if (!m_hal.get_streams().add(m_stream))
        {
            return false;
        }
    }
    return true;
}

auto ADC_Voltmeter::get_input_stream_count() const -> size_t
{
    return m_adc_stream ? 1 : 0;
}
auto ADC_Voltmeter::get_input_stream(size_t idx) -> stream::IADC_Value&
{
    QASSERT(idx == 0);
    QASSERT(m_adc_stream);
    return *m_adc_stream;
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

    if (get_input_stream_count() == 0)
    {
        return;
    }

    auto const& s = get_input_stream(0).get_samples();
    m_stream.samples.resize(s.size());

    std::transform(s.begin(), s.end(), m_stream.samples.begin(), [this](stream::IADC_Value::Sample const& sample)
    {
       Stream::Sample vs;
       vs.dt = sample.dt;
       vs.sample_idx = sample.sample_idx;
       vs.value = sample.value * m_config->outputs.voltage.scale + m_config->outputs.voltage.bias;
       return vs;
    });

    //TODO - apply scale - bias
}

auto ADC_Voltmeter::set_config(rapidjson::Value const& json) -> bool
{
    sz::ADC_Voltmeter::Config sz;
    autojsoncxx::error::ErrorStack result;
    if (!autojsoncxx::from_value(sz, json, result))
    {
        std::ostringstream ss;
        ss << result;
        QLOGE("Cannot deserialize ADC_Voltmeter config data: {}", ss.str());
        return false;
    }

    auto* adc_stream = m_hal.get_streams().find_by_name<stream::IADC_Value>(sz.inputs.adc);
    if (!adc_stream || adc_stream->get_rate() == 0)
    {
        QLOGE("No input angular velocity stream specified");
        return false;
    }
    if (m_stream.rate != 0 && m_stream.rate != adc_stream->get_rate())
    {
        QLOGE("Input streams rate has changed: {} != {}",
              adc_stream->get_rate(),
              m_stream.rate);
        return false;
    }

    m_adc_stream = adc_stream;
    m_stream.rate = m_adc_stream->get_rate();

    *m_config = sz;
    autojsoncxx::to_document(*m_config, m_config_json);
    return true;
}
auto ADC_Voltmeter::get_config() -> boost::optional<rapidjson::Value const&>
{
    return m_config_json;
}
auto ADC_Voltmeter::get_init_params() -> boost::optional<rapidjson::Value const&>
{
    return m_init_params_json;
}


}
}
}
