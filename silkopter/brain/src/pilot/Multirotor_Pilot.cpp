#include "BrainStdAfx.h"
#include "Multirotor_Pilot.h"

#include "uav.def.h"
//#include "sz_Multirotor_Pilot.hpp"

namespace silk
{
namespace node
{

Multirotor_Pilot::Multirotor_Pilot(UAV& uav, Comms& comms)
    : m_uav(uav)
    , m_comms(comms)
    , m_descriptor(new Multirotor_Pilot_Descriptor())
    , m_config(new Multirotor_Pilot_Config())
{
    m_output_stream = std::make_shared<Output_Stream>();
}

auto Multirotor_Pilot::init(std::shared_ptr<INode_Descriptor> descriptor) -> bool
{
    QLOG_TOPIC("Multirotor_Pilot::init");

    auto specialized = std::dynamic_pointer_cast<Multirotor_Pilot_Descriptor>(descriptor);
    if (!specialized)
    {
        QLOGE("Wrong descriptor type");
        return false;
    }

    *m_descriptor = *specialized;

    return init();
}

auto Multirotor_Pilot::init() -> bool
{
    m_output_stream->set_rate(m_descriptor->get_commands_rate());
    return true;
}

auto Multirotor_Pilot::start(q::Clock::time_point tp) -> bool
{
    m_output_stream->set_tp(tp);
    return true;
}

auto Multirotor_Pilot::get_inputs() const -> std::vector<Input>
{
    std::vector<Input> inputs =
    {{
        { stream::IMultirotor_State::TYPE,  m_descriptor->get_state_rate(), "State", m_state_accumulator.get_stream_path(0) },
        { stream::IVideo::TYPE,        m_descriptor->get_video_rate(), "Video", m_video_accumulator.get_stream_path(0) },
    }};
    return inputs;
}
auto Multirotor_Pilot::get_outputs() const -> std::vector<Output>
{
    std::vector<Output> outputs =
    {{
         { "Commands",        m_output_stream },
    }};
    return outputs;
}

static constexpr std::chrono::milliseconds COMMANDS_HEALTHY_TIMEOUT(200);

void Multirotor_Pilot::process()
{
    QLOG_TOPIC("Multirotor_Pilot::process");

    m_output_stream->clear();

    //process commandss
    {
        auto const& commands_values = m_comms.get_multirotor_commands_values();

        auto now = q::Clock::now();

        stream::IMultirotor_Commands::Value& commands_value = m_last_commands_value;
        if (!commands_values.empty())
        {
            commands_value = commands_values.back();
            m_last_received_commands_value_tp = now;
        }

        bool is_healthy = now - m_last_received_commands_value_tp < COMMANDS_HEALTHY_TIMEOUT;

        size_t samples_needed = m_output_stream->compute_samples_needed();
        for (size_t i = 0; i < samples_needed; i++)
        {
            m_output_stream->push_sample(commands_value, is_healthy);
        }
    }


    //write back the state
    m_state_accumulator.process([this](stream::IMultirotor_State::Sample const& i_state)
    {
        m_comms.add_multirotor_state_sample(i_state);
    });
    //write back the video
    m_video_accumulator.process([this](stream::IVideo::Sample const& i_video)
    {
        m_comms.add_video_sample(i_video);
    });
}

void Multirotor_Pilot::set_input_stream_path(size_t idx, q::Path const& path)
{
    if (idx == 0)
    {
        m_state_accumulator.set_stream_path(0, path, m_descriptor->get_state_rate(), m_uav);
    }
    else
    {
        m_video_accumulator.set_stream_path(0, path, m_descriptor->get_video_rate(), m_uav);
    }
}

auto Multirotor_Pilot::set_config(std::shared_ptr<INode_Config> config) -> bool
{
    QLOG_TOPIC("Multirotor_Pilot::set_config");

    auto specialized = std::dynamic_pointer_cast<Multirotor_Pilot_Config>(config);
    if (!specialized)
    {
        QLOGE("Wrong config type");
        return false;
    }

    *m_config = *specialized;

    return true;
}
auto Multirotor_Pilot::get_config() const -> std::shared_ptr<INode_Config>
{
    return m_config;
}

auto Multirotor_Pilot::get_descriptor() const -> std::shared_ptr<INode_Descriptor>
{
    return m_descriptor;
}

//auto Multirotor_Pilot::send_message(rapidjson::Value const& /*json*/) -> rapidjson::Document
//{
//    return rapidjson::Document();
//}

}
}
