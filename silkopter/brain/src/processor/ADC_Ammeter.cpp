#include "BrainStdAfx.h"
#include "ADC_Ammeter.h"

#include "sz_math.hpp"
#include "sz_ADC_Ammeter.hpp"

namespace silk
{
namespace node
{

ADC_Ammeter::ADC_Ammeter(UAV& uav)
    : m_uav(uav)
    , m_init_params(new sz::ADC_Ammeter::Init_Params())
    , m_config(new sz::ADC_Ammeter::Config())
{
    m_output_stream = std::make_shared<Output_Stream>();
}

auto ADC_Ammeter::init(rapidjson::Value const& init_params) -> bool
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
    return init();
}

auto ADC_Ammeter::init() -> bool
{
    if (m_init_params->rate == 0)
    {
        QLOGE("Bad rate: {}Hz", m_init_params->rate);
        return false;
    }
    m_output_stream->set_rate(m_init_params->rate);
    return true;
}

auto ADC_Ammeter::start(q::Clock::time_point tp) -> bool
{
    m_output_stream->set_tp(tp);
    return true;
}

auto ADC_Ammeter::get_inputs() const -> std::vector<Input>
{
    std::vector<Input> inputs =
    {{
        { stream::IADC::TYPE, m_init_params->rate, "ADC", m_accumulator.get_stream_path(0) }
    }};
    return inputs;
}
auto ADC_Ammeter::get_outputs() const -> std::vector<Output>
{
    std::vector<Output> outputs(1);
    outputs[0].name = "Current";
    outputs[0].stream = m_output_stream;
    return outputs;
}

void ADC_Ammeter::process()
{
    QLOG_TOPIC("adc_ammeter::process");

    m_output_stream->clear();

    m_accumulator.process([this](stream::IADC::Sample const& i_sample)
    {
        if (i_sample.is_healthy)
        {
            m_output_stream->push_sample(i_sample.value * m_config->scale + m_config->bias, true);
        }
        else
        {
            m_output_stream->push_last_sample(false);
        }
    });
}

void ADC_Ammeter::set_input_stream_path(size_t idx, q::Path const& path)
{
    m_accumulator.set_stream_path(idx, path, m_output_stream->get_rate(), m_uav);
}

auto ADC_Ammeter::set_config(rapidjson::Value const& json) -> bool
{
    QLOG_TOPIC("adc_ammeter::set_config");

    sz::ADC_Ammeter::Config sz;
    autojsoncxx::error::ErrorStack result;
    if (!autojsoncxx::from_value(sz, json, result))
    {
        std::ostringstream ss;
        ss << result;
        QLOGE("Cannot deserialize ADC_Ammeter config data: {}", ss.str());
        return false;
    }

    *m_config = sz;

    return true;
}
auto ADC_Ammeter::get_config() const -> rapidjson::Document
{
    rapidjson::Document json;
    autojsoncxx::to_document(*m_config, json);
    return std::move(json);
}

auto ADC_Ammeter::get_init_params() const -> rapidjson::Document
{
    rapidjson::Document json;
    autojsoncxx::to_document(*m_init_params, json);
    return std::move(json);
}
auto ADC_Ammeter::send_message(rapidjson::Value const& /*json*/) -> rapidjson::Document
{
    return rapidjson::Document();
}


}
}
