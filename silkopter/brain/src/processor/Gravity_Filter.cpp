#include "BrainStdAfx.h"
#include "Gravity_Filter.h"
#include "physics/constants.h"

#include "hal.def.h"
//#include "sz_Gravity_Filter.hpp"

namespace silk
{
namespace node
{

Gravity_Filter::Gravity_Filter(HAL& hal)
    : m_hal(hal)
    , m_descriptor(new hal::Gravity_Filter_Descriptor())
    , m_config(new hal::Gravity_Filter_Config())
{
    m_output_stream = std::make_shared<Output_Stream>();
}

ts::Result<void> Gravity_Filter::init(hal::INode_Descriptor const& descriptor)
{
    QLOG_TOPIC("gravity_filter::init");

    auto specialized = dynamic_cast<hal::Gravity_Filter_Descriptor const*>(&descriptor);
    if (!specialized)
    {
        return make_error("Wrong descriptor type");
    }
    *m_descriptor = *specialized;

    return init();
}
ts::Result<void> Gravity_Filter::init()
{
    m_output_stream->set_rate(m_descriptor->get_rate());
    return ts::success;
}

ts::Result<void> Gravity_Filter::start(q::Clock::time_point tp)
{
    m_output_stream->set_tp(tp);
    return ts::success;
}

auto Gravity_Filter::get_inputs() const -> std::vector<Input>
{
    std::vector<Input> inputs =
    {{
        { stream::IFrame::TYPE, m_descriptor->get_rate(), "frame", m_accumulator.get_stream_path(0) },
        { stream::IAcceleration::TYPE, m_descriptor->get_rate(), "acceleration", m_accumulator.get_stream_path(1) }
    }};
    return inputs;
}
auto Gravity_Filter::get_outputs() const -> std::vector<Output>
{
    std::vector<Output> outputs =
    {{
         { "linear_acceleration", m_output_stream }
    }};
    return outputs;
}

void Gravity_Filter::process()
{
    QLOG_TOPIC("gravity_filter::process");

    m_output_stream->clear();

    m_accumulator.process([this](stream::IFrame::Sample const& f_sample,
                                stream::IAcceleration::Sample const& a_sample)
    {
        if (f_sample.is_healthy & a_sample.is_healthy)
        {
            auto p2l = math::inverse<float, math::safe>(f_sample.value);
            auto gravity_local = math::rotate(p2l, physics::constants::enu_gravity);
            m_output_stream->push_sample(a_sample.value - gravity_local, true);
        }
        else
        {
            m_output_stream->push_last_sample(false);
        }
    });
}

ts::Result<void> Gravity_Filter::set_input_stream_path(size_t idx, std::string const& path)
{
    return m_accumulator.set_stream_path(idx, path, m_output_stream->get_rate(), m_hal);
}

ts::Result<void> Gravity_Filter::set_config(hal::INode_Config const& config)
{
    QLOG_TOPIC("gravity_filter::set_config");

    auto specialized = dynamic_cast<hal::Gravity_Filter_Config const*>(&config);
    if (!specialized)
    {
        return make_error("Wrong config type");
    }
    *m_config = *specialized;

    return ts::success;
}
auto Gravity_Filter::get_config() const -> std::shared_ptr<const hal::INode_Config>
{
    return m_config;
}

auto Gravity_Filter::get_descriptor() const -> std::shared_ptr<const hal::INode_Descriptor>
{
    return m_descriptor;
}

//auto Gravity_Filter::send_message(rapidjson::Value const& /*json*/) -> rapidjson::Document
//{
//    return rapidjson::Document();
//}

}
}
