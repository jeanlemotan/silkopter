#include "BrainStdAfx.h"
#include "ADC_Voltmeter.h"

#include "sz_math.hpp"
#include "sz_ADC_Voltmeter.hpp"

namespace silk
{
namespace node
{

ADC_Voltmeter::ADC_Voltmeter(HAL& hal)
    : m_hal(hal)
    , m_init_params(new sz::ADC_Voltmeter::Init_Params())
    , m_config(new sz::ADC_Voltmeter::Config())
{
    m_output_stream = std::make_shared<Output_Stream>();
}

auto ADC_Voltmeter::init(rapidjson::Value const& init_params) -> bool
{
    QLOG_TOPIC("adc_voltmeter::init");

    sz::ADC_Voltmeter::Init_Params sz;
    autojsoncxx::error::ErrorStack result;
    if (!autojsoncxx::from_value(sz, init_params, result))
    {
        std::ostringstream ss;
        ss << result;
        QLOGE("Cannot deserialize ADC_Voltmeter data: {}", ss.str());
        return false;
    }
    *m_init_params = sz;
    return init();
}
auto ADC_Voltmeter::init() -> bool
{
    if (m_init_params->rate == 0)
    {
        QLOGE("Bad rate: {}Hz", m_init_params->rate);
        return false;
    }
    m_output_stream->set_rate(m_init_params->rate);
    m_output_stream->set_tp(q::Clock::now());
    return true;
}

auto ADC_Voltmeter::get_inputs() const -> std::vector<Input>
{
    std::vector<Input> inputs =
    {{
        { stream::IADC::TYPE, m_init_params->rate, "ADC", m_accumulator.get_stream_path(0) }
    }};
    return inputs;
}
auto ADC_Voltmeter::get_outputs() const -> std::vector<Output>
{
    std::vector<Output> outputs(1);
    outputs[0].name = "Voltage";
    outputs[0].stream = m_output_stream;
    return outputs;
}


void ADC_Voltmeter::process()
{
    QLOG_TOPIC("adc_voltmeter::process");

    m_output_stream->clear();

    m_accumulator.process([this](stream::IADC::Sample const& i_sample)
    {
        if (i_sample.is_healthy)
        {
            m_output_stream->push_sample(i_sample.value, true);
        }
        else
        {
            m_output_stream->push_last_sample(false);
        }
    });
}

void ADC_Voltmeter::set_input_stream_path(size_t idx, q::Path const& path)
{
    m_accumulator.set_stream_path(idx, path, m_output_stream->get_rate(), m_hal);
}

auto ADC_Voltmeter::set_config(rapidjson::Value const& json) -> bool
{
    QLOG_TOPIC("adc_voltmeter::set_config");

    sz::ADC_Voltmeter::Config sz;
    autojsoncxx::error::ErrorStack result;
    if (!autojsoncxx::from_value(sz, json, result))
    {
        std::ostringstream ss;
        ss << result;
        QLOGE("Cannot deserialize ADC_Voltmeter config data: {}", ss.str());
        return false;
    }

    *m_config = sz;

    return true;
}
auto ADC_Voltmeter::get_config() const -> rapidjson::Document
{
    rapidjson::Document json;
    autojsoncxx::to_document(*m_config, json);
    return std::move(json);
}

auto ADC_Voltmeter::get_init_params() const -> rapidjson::Document
{
    rapidjson::Document json;
    autojsoncxx::to_document(*m_init_params, json);
    return std::move(json);
}

auto ADC_Voltmeter::send_message(rapidjson::Value const& /*json*/) -> rapidjson::Document
{
    return rapidjson::Document();
}


}
}
