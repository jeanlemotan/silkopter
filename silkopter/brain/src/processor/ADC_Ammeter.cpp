#include "BrainStdAfx.h"
#include "ADC_Ammeter.h"

#include "uav.def.h"
//#include "sz_ADC_Ammeter.hpp"

namespace silk
{
namespace node
{

ADC_Ammeter::ADC_Ammeter(UAV& uav)
    : m_uav(uav)
    , m_descriptor(new uav::ADC_Ammeter_Descriptor())
    , m_config(new uav::ADC_Ammeter_Config())
{
    m_output_stream = std::make_shared<Output_Stream>();
}

auto ADC_Ammeter::init(uav::INode_Descriptor const& descriptor) -> bool
{
    QLOG_TOPIC("adc_ammeter::init");

    auto specialized = dynamic_cast<uav::ADC_Ammeter_Descriptor const*>(&descriptor);
    if (!specialized)
    {
        QLOGE("Wrong descriptor type");
        return false;
    }
    *m_descriptor = *specialized;

    return init();
}

auto ADC_Ammeter::init() -> bool
{
    m_output_stream->set_rate(m_descriptor->get_rate());
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
        { stream::IADC::TYPE, m_descriptor->get_rate(), "ADC", m_accumulator.get_stream_path(0) }
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
            m_output_stream->push_sample(i_sample.value * m_config->get_scale() + m_config->get_bias(), true);
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

auto ADC_Ammeter::set_config(uav::INode_Config const& config) -> bool
{
    QLOG_TOPIC("adc_ammeter::set_config");

    auto specialized = dynamic_cast<uav::ADC_Ammeter_Config const*>(&config);
    if (!specialized)
    {
        QLOGE("Wrong config type");
        return false;
    }
    *m_config = *specialized;

    return true;
}
auto ADC_Ammeter::get_config() const -> std::shared_ptr<const uav::INode_Config>
{
    return m_config;
}

auto ADC_Ammeter::get_descriptor() const -> std::shared_ptr<const uav::INode_Descriptor>
{
    return m_descriptor;
}
//auto ADC_Ammeter::send_message(rapidjson::Value const& /*json*/) -> rapidjson::Document
//{
//    return rapidjson::Document();
//}


}
}
