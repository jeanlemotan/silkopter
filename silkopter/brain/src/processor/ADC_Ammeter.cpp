#include "BrainStdAfx.h"
#include "ADC_Ammeter.h"

#include "hal.def.h"
//#include "sz_ADC_Ammeter.hpp"

namespace silk
{
namespace node
{

ADC_Ammeter::ADC_Ammeter(HAL& hal)
    : m_hal(hal)
    , m_descriptor(new hal::ADC_Ammeter_Descriptor())
    , m_config(new hal::ADC_Ammeter_Config())
{
    m_output_stream = std::make_shared<Output_Stream>();
}

ts::Result<void> ADC_Ammeter::init(hal::INode_Descriptor const& descriptor)
{
    QLOG_TOPIC("adc_ammeter::init");

    auto specialized = dynamic_cast<hal::ADC_Ammeter_Descriptor const*>(&descriptor);
    if (!specialized)
    {
        return make_error("Wrong descriptor type");
    }
    *m_descriptor = *specialized;

    return init();
}

ts::Result<void> ADC_Ammeter::init()
{
    m_output_stream->set_rate(m_descriptor->get_rate());
    return ts::success;
}

ts::Result<void> ADC_Ammeter::start(q::Clock::time_point tp)
{
    m_output_stream->set_tp(tp);
    return ts::success;
}

auto ADC_Ammeter::get_inputs() const -> std::vector<Input>
{
    std::vector<Input> inputs =
    {{
        { stream::IADC::TYPE, m_descriptor->get_rate(), "adc", m_accumulator.get_stream_path(0) }
    }};
    return inputs;
}
auto ADC_Ammeter::get_outputs() const -> std::vector<Output>
{
    std::vector<Output> outputs(1);
    outputs[0].name = "current";
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
            m_output_stream->push_sample(i_sample.value * m_config->get_scale() + m_config->get_bias(), true);
        }
        else
        {
            m_output_stream->push_last_sample(false);
        }
    });
}

ts::Result<void> ADC_Ammeter::set_input_stream_path(size_t idx, std::string const& path)
{
    return m_accumulator.set_stream_path(idx, path, m_output_stream->get_rate(), m_hal);
}

ts::Result<void> ADC_Ammeter::set_config(hal::INode_Config const& config)
{
    QLOG_TOPIC("adc_ammeter::set_config");

    auto specialized = dynamic_cast<hal::ADC_Ammeter_Config const*>(&config);
    if (!specialized)
    {
        return make_error("Wrong config type");
    }
    *m_config = *specialized;

    return ts::success;
}
auto ADC_Ammeter::get_config() const -> std::shared_ptr<const hal::INode_Config>
{
    return m_config;
}

auto ADC_Ammeter::get_descriptor() const -> std::shared_ptr<const hal::INode_Descriptor>
{
    return m_descriptor;
}
//auto ADC_Ammeter::send_message(rapidjson::Value const& /*json*/) -> rapidjson::Document
//{
//    return rapidjson::Document();
//}


}
}
