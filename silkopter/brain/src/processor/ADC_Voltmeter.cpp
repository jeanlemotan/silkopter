#include "BrainStdAfx.h"
#include "ADC_Voltmeter.h"

#include "uav.def.h"
//#include "sz_ADC_Voltmeter.hpp"

namespace silk
{
namespace node
{

ADC_Voltmeter::ADC_Voltmeter(UAV& uav)
    : m_uav(uav)
    , m_descriptor(new ADC_Voltmeter_Descriptor())
    , m_config(new ADC_Voltmeter_Config())
{
    m_output_stream = std::make_shared<Output_Stream>();
}

auto ADC_Voltmeter::init(std::shared_ptr<INode_Descriptor> descriptor) -> bool
{
    QLOG_TOPIC("adc_voltmeter::init");

    auto specialized = std::dynamic_pointer_cast<ADC_Voltmeter_Descriptor>(descriptor);
    if (!specialized)
    {
        QLOGE("Wrong descriptor type");
        return false;
    }

    *m_descriptor = *specialized;
    return init();
}
auto ADC_Voltmeter::init() -> bool
{
    m_output_stream->set_rate(m_descriptor->get_rate());
    return true;
}

auto ADC_Voltmeter::start(q::Clock::time_point tp) -> bool
{
    m_output_stream->set_tp(tp);
    return true;
}

auto ADC_Voltmeter::get_inputs() const -> std::vector<Input>
{
    std::vector<Input> inputs =
    {{
        { stream::IADC::TYPE, m_descriptor->get_rate(), "ADC", m_accumulator.get_stream_path(0) }
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
            m_output_stream->push_sample(i_sample.value * m_config->get_scale() + m_config->get_bias(), true);
        }
        else
        {
            m_output_stream->push_last_sample(false);
        }
    });
}

void ADC_Voltmeter::set_input_stream_path(size_t idx, q::Path const& path)
{
    m_accumulator.set_stream_path(idx, path, m_output_stream->get_rate(), m_uav);
}

auto ADC_Voltmeter::set_config(std::shared_ptr<INode_Config> config) -> bool
{
    QLOG_TOPIC("adc_voltmeter::set_config");

    auto specialized = std::dynamic_pointer_cast<ADC_Voltmeter_Config>(config);
    if (!specialized)
    {
        QLOGE("Wrong config type");
        return false;
    }

    *m_config = *specialized;

    return true;
}
auto ADC_Voltmeter::get_config() const -> std::shared_ptr<INode_Config>
{
    return m_config;
}

auto ADC_Voltmeter::get_descriptor() const -> std::shared_ptr<INode_Descriptor>
{
    return m_descriptor;
}

//auto ADC_Voltmeter::send_message(rapidjson::Value const& /*json*/) -> rapidjson::Document
//{
//    return rapidjson::Document();
//}


}
}
