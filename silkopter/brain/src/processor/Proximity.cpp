#include "BrainStdAfx.h"
#include "Proximity.h"

#include "uav.def.h"
//#include "sz_Proximity.hpp"

namespace silk
{
namespace node
{

Proximity::Proximity(UAV& uav)
    : m_uav(uav)
    , m_descriptor(new uav::Proximity_Descriptor())
    , m_config(new uav::Proximity_Config())
{
    m_output_stream = std::make_shared<Output_Stream>();
}

auto Proximity::init(uav::INode_Descriptor const& descriptor) -> bool
{
    QLOG_TOPIC("Proximity::init");

    auto specialized = dynamic_cast<uav::Proximity_Descriptor const*>(&descriptor);
    if (!specialized)
    {
        QLOGE("Wrong descriptor type");
        return false;
    }
    *m_descriptor = *specialized;

    return init();
}
auto Proximity::init() -> bool
{
    m_accumulators.resize(m_descriptor->get_channel_count());
    m_output_stream->set_rate(m_descriptor->get_rate());

    return true;
}

auto Proximity::start(q::Clock::time_point tp) -> bool
{
    m_output_stream->set_tp(tp);
    return true;
}

auto Proximity::get_inputs() const -> std::vector<Input>
{
    std::vector<Input> inputs(m_descriptor->get_channel_count());
    for (size_t i = 0; i < inputs.size(); i++)
    {
        inputs[i] = { stream::IDistance::TYPE, m_descriptor->get_rate(), q::util::format<std::string>("Distance {}", i), m_accumulators[i].get_stream_path(0) };
    }
    return inputs;
}
auto Proximity::get_outputs() const -> std::vector<Output>
{
    std::vector<Output> outputs =
    {{
         { "Proximity", m_output_stream }
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

void Proximity::set_input_stream_path(size_t idx, q::Path const& path)
{
    m_accumulators[idx].set_stream_path(0, path, m_output_stream->get_rate(), m_uav);
}

auto Proximity::set_config(uav::INode_Config const& config) -> bool
{
    QLOG_TOPIC("Proximity::set_config");

    auto specialized = dynamic_cast<uav::Proximity_Config const*>(&config);
    if (!specialized)
    {
        QLOGE("Wrong config type");
        return false;
    }
    *m_config = *specialized;

    return true;
}
auto Proximity::get_config() const -> std::shared_ptr<const uav::INode_Config>
{
    return m_config;
}

auto Proximity::get_descriptor() const -> std::shared_ptr<const uav::INode_Descriptor>
{
    return m_descriptor;
}
//auto Proximity::send_message(rapidjson::Value const& /*json*/) -> rapidjson::Document
//{
//    return rapidjson::Document();
//}


}
}
