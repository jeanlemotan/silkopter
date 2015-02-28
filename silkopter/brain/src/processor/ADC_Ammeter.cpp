#include "BrainStdAfx.h"
#include "ADC_Ammeter.h"

#include "sz_math.hpp"
#include "sz_ADC_Ammeter.hpp"

namespace silk
{
namespace node
{
namespace processor
{

ADC_Ammeter::ADC_Ammeter(HAL& hal)
    : m_hal(hal)
    , m_init_params(new sz::ADC_Ammeter::Init_Params())
    , m_config(new sz::ADC_Ammeter::Config())
{
}

auto ADC_Ammeter::get_name() const -> std::string const&
{
    return m_init_params->name;
}

auto ADC_Ammeter::init(rapidjson::Value const& init_params, rapidjson::Value const& config) -> bool
{
    QLOG_TOPIC("adc_ammeter::init");

    sz::ADC_Ammeter::Init_Params sz;
    autojsoncxx::error::ErrorStack result;
    if (!autojsoncxx::from_value(sz, init_params, result))
    {
        std::ostringstream ss;
        ss << result;
        QLOGE("Cannot deserialize ADC_Ammeter data: {}", ss.str());
        return false;
    }
    *m_init_params = sz;
    return init() && set_config(config);
}
auto ADC_Ammeter::init() -> bool
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

auto ADC_Ammeter::get_input_stream_count() const -> size_t
{
    return m_adc_stream ? 1 : 0;
}
auto ADC_Ammeter::get_input_stream(size_t idx) -> stream::IADC_Value&
{
    QASSERT(idx == 0);
    QASSERT(m_adc_stream);
    return *m_adc_stream;
}
auto ADC_Ammeter::get_output_stream_count() const -> size_t
{
    return 1;
}
auto ADC_Ammeter::get_output_stream(size_t idx) -> stream::ICurrent&
{
    QASSERT(idx == 0);
    return m_stream;
}


void ADC_Ammeter::process()
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
       vs.value = sample.value * m_config->outputs.current.scale + m_config->outputs.current.bias;
       return vs;
    });

    //TODO - apply scale - bias
}

auto ADC_Ammeter::set_config(rapidjson::Value const& json) -> bool
{
    sz::ADC_Ammeter::Config sz;
    autojsoncxx::error::ErrorStack result;
    if (!autojsoncxx::from_value(sz, json, result))
    {
        std::ostringstream ss;
        ss << result;
        QLOGE("Cannot deserialize ADC_Ammeter config data: {}", ss.str());
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
    return true;
}
auto ADC_Ammeter::get_config() -> rapidjson::Document
{
    rapidjson::Document json;
    autojsoncxx::to_document(*m_config, json);
    return std::move(json);
}

auto ADC_Ammeter::get_init_params() -> rapidjson::Document
{
    rapidjson::Document json;
    autojsoncxx::to_document(*m_init_params, json);
    return std::move(json);
}


}
}
}
