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

auto Gravity_Filter::init(hal::INode_Descriptor const& descriptor) -> bool
{
    QLOG_TOPIC("gravity_filter::init");

    auto specialized = dynamic_cast<hal::Gravity_Filter_Descriptor const*>(&descriptor);
    if (!specialized)
    {
        QLOGE("Wrong descriptor type");
        return false;
    }
    *m_descriptor = *specialized;

    return init();
}
auto Gravity_Filter::init() -> bool
{
    m_output_stream->set_rate(m_descriptor->get_rate());
    return true;
}

auto Gravity_Filter::start(q::Clock::time_point tp) -> bool
{
    m_output_stream->set_tp(tp);
    return true;
}

auto Gravity_Filter::get_inputs() const -> std::vector<Input>
{
    std::vector<Input> inputs =
    {{
        { stream::IUAV_Frame::TYPE, m_descriptor->get_rate(), "UAV Frame", m_accumulator.get_stream_path(0) },
        { stream::IAcceleration::TYPE, m_descriptor->get_rate(), "Acceleration", m_accumulator.get_stream_path(1) }
    }};
    return inputs;
}
auto Gravity_Filter::get_outputs() const -> std::vector<Output>
{
    std::vector<Output> outputs =
    {{
         { "Linear Acceleration", m_output_stream }
    }};
    return outputs;
}

void Gravity_Filter::process()
{
    QLOG_TOPIC("gravity_filter::process");

    m_output_stream->clear();

    m_accumulator.process([this](stream::IUAV_Frame::Sample const& f_sample,
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

void Gravity_Filter::set_input_stream_path(size_t idx, q::Path const& path)
{
    m_accumulator.set_stream_path(idx, path, m_output_stream->get_rate(), m_hal);
}

auto Gravity_Filter::set_config(hal::INode_Config const& config) -> bool
{
    QLOG_TOPIC("gravity_filter::set_config");

    auto specialized = dynamic_cast<hal::Gravity_Filter_Config const*>(&config);
    if (!specialized)
    {
        QLOGE("Wrong config type");
        return false;
    }
    *m_config = *specialized;

    return true;
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
