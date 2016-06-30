#include "BrainStdAfx.h"
#include "Throttle_To_PWM.h"

#include "hal.def.h"
//#include "sz_Throttle_To_PWM.hpp"

namespace silk
{
namespace node
{

Throttle_To_PWM::Throttle_To_PWM(HAL& hal)
    : m_hal(hal)
    , m_descriptor(new hal::Throttle_To_PWM_Descriptor())
    , m_config(new hal::Throttle_To_PWM_Config())
{
}

ts::Result<void> Throttle_To_PWM::init(hal::INode_Descriptor const& descriptor)
{
    QLOG_TOPIC("Throttle_To_PWM::init");

    auto specialized = dynamic_cast<hal::Throttle_To_PWM_Descriptor const*>(&descriptor);
    if (!specialized)
    {
        return make_error("Wrong descriptor type");
    }
    *m_descriptor = *specialized;

    return init();
}
ts::Result<void> Throttle_To_PWM::init()
{
    m_accumulators.resize(m_descriptor->get_channel_count());
    m_output_streams.resize(m_descriptor->get_channel_count());

    for (auto& o: m_output_streams)
    {
        o.reset(new Output_Stream);
        o->set_rate(m_descriptor->get_rate());
    }
    return ts::success;
}

ts::Result<void> Throttle_To_PWM::start(q::Clock::time_point tp)
{
    for (auto& o: m_output_streams)
    {
        o->set_tp(tp);
    }
    return ts::success;
}

auto Throttle_To_PWM::get_inputs() const -> std::vector<Input>
{
    std::vector<Input> inputs(m_accumulators.size());
    for (size_t i = 0; i < inputs.size(); i++)
    {
        inputs[i] = { stream::IThrottle::TYPE, m_descriptor->get_rate(), q::util::format<std::string>("Throttle {}", i), m_accumulators[i].get_stream_path(0) };
    }
    return inputs;
}
auto Throttle_To_PWM::get_outputs() const -> std::vector<Output>
{
    std::vector<Output> outputs(m_accumulators.size());
    for (size_t i = 0; i < outputs.size(); i++)
    {
        outputs[i] = { q::util::format<std::string>("PWM {}", i), m_output_streams[i] };
    }
    return outputs;
}

void Throttle_To_PWM::process()
{
    QLOG_TOPIC("Throttle_To_PWM::process");

    for (size_t i = 0; i < m_output_streams.size(); i++)
    {
        m_output_streams[i]->clear();

        m_accumulators[i].process([this, i](stream::IThrottle::Sample const& i_sample)
        {
            if (i_sample.is_healthy)
            {
                m_output_streams[i]->push_sample(i_sample.value, true);
            }
            else
            {
                m_output_streams[i]->push_last_sample(false);
            }
        });
    }
}

ts::Result<void> Throttle_To_PWM::set_input_stream_path(size_t idx, std::string const& path)
{
    return m_accumulators[idx].set_stream_path(0, path, m_output_streams[0]->get_rate(), m_hal);
}

ts::Result<void> Throttle_To_PWM::set_config(hal::INode_Config const& config)
{
    QLOG_TOPIC("Throttle_To_PWM::set_config");

    auto specialized = dynamic_cast<hal::Throttle_To_PWM_Config const*>(&config);
    if (!specialized)
    {
        return make_error("Wrong config type");
    }
    *m_config = *specialized;

    return ts::success;
}
auto Throttle_To_PWM::get_config() const -> std::shared_ptr<const hal::INode_Config>
{
    return m_config;
}

auto Throttle_To_PWM::get_descriptor() const -> std::shared_ptr<const hal::INode_Descriptor>
{
    return m_descriptor;
}
//auto Throttle_To_PWM::send_message(rapidjson::Value const& /*json*/) -> rapidjson::Document
//{
//    return rapidjson::Document();
//}


}
}
