#include "BrainStdAfx.h"
#include "Proximity.h"

#include "hal.def.h"
//#include "sz_Proximity.hpp"

namespace silk
{
namespace node
{

Proximity::Proximity(HAL& hal)
    : m_hal(hal)
    , m_descriptor(new hal::Proximity_Descriptor())
    , m_config(new hal::Proximity_Config())
{
    m_output_stream = std::make_shared<Output_Stream>();
}

ts::Result<void> Proximity::init(hal::INode_Descriptor const& descriptor)
{
    QLOG_TOPIC("Proximity::init");

    auto specialized = dynamic_cast<hal::Proximity_Descriptor const*>(&descriptor);
    if (!specialized)
    {
        return make_error("Wrong descriptor type");
    }
    *m_descriptor = *specialized;

    return init();
}
ts::Result<void> Proximity::init()
{
    m_accumulators.resize(m_descriptor->get_channel_count());
    m_output_stream->set_rate(m_descriptor->get_rate());

    return ts::success;
}

ts::Result<void> Proximity::start(Clock::time_point tp)
{
    m_output_stream->set_tp(tp);
    return ts::success;
}

auto Proximity::get_inputs() const -> std::vector<Input>
{
    std::vector<Input> inputs(m_accumulators.size());
    for (size_t i = 0; i < inputs.size(); i++)
    {
        inputs[i] = { stream::IDistance::TYPE, m_descriptor->get_rate(), q::util::format<std::string>("distance_{}", i), m_accumulators[i].get_stream_path(0) };
    }
    return inputs;
}
auto Proximity::get_outputs() const -> std::vector<Output>
{
    std::vector<Output> outputs =
    {{
         { "proximity", m_output_stream }
    }};
    return outputs;
}

void Proximity::process()
{
    QLOG_TOPIC("Proximity::process");

    m_output_stream->clear();
    m_output_value.distances.clear();

    for (size_t i = 0; i < m_accumulators.size(); i++)
    {
        m_accumulators[i].process([this, i](stream::IDistance::Sample const& i_sample)
        {
            if (i_sample.is_healthy)
            {
                m_output_value.distances.push_back(i_sample.value);
            }
        });
    }

    size_t samples_needed = m_output_stream->compute_samples_needed();
    if (samples_needed > 0)
    {
        for (size_t i = 0; i < samples_needed; i++)
        {
            m_output_stream->push_sample(m_output_value, !m_output_value.distances.empty());
        }
    }
}

ts::Result<void> Proximity::set_input_stream_path(size_t idx, std::string const& path)
{
    return m_accumulators[idx].set_stream_path(0, path, m_output_stream->get_rate(), m_hal);
}

ts::Result<void> Proximity::set_config(hal::INode_Config const& config)
{
    QLOG_TOPIC("Proximity::set_config");

    auto specialized = dynamic_cast<hal::Proximity_Config const*>(&config);
    if (!specialized)
    {
        return make_error("Wrong config type");
    }
    *m_config = *specialized;

    return ts::success;
}
auto Proximity::get_config() const -> std::shared_ptr<const hal::INode_Config>
{
    return m_config;
}

auto Proximity::get_descriptor() const -> std::shared_ptr<const hal::INode_Descriptor>
{
    return m_descriptor;
}
ts::Result<std::shared_ptr<messages::INode_Message>> Proximity::send_message(messages::INode_Message const& message)
{
    return make_error("Unknown message");
}


}
}
