#include "BrainStdAfx.h"
#include "Multirotor_Pilot.h"

#include "hal.def.h"
//#include "sz_Multirotor_Pilot.hpp"

namespace silk
{
namespace node
{

Multirotor_Pilot::Multirotor_Pilot(HAL& hal, RC_Comms& rc_comms)
    : m_hal(hal)
    , m_rc_comms(rc_comms)
    , m_descriptor(new hal::Multirotor_Pilot_Descriptor())
    , m_config(new hal::Multirotor_Pilot_Config())
{
    m_output_stream = std::make_shared<Output_Stream>();
}

ts::Result<void> Multirotor_Pilot::init(hal::INode_Descriptor const& descriptor)
{
    QLOG_TOPIC("Multirotor_Pilot::init");

    auto specialized = dynamic_cast<hal::Multirotor_Pilot_Descriptor const*>(&descriptor);
    if (!specialized)
    {
        return make_error("Wrong descriptor type");
    }
    *m_descriptor = *specialized;

    return init();
}

ts::Result<void> Multirotor_Pilot::init()
{
    m_output_stream->set_rate(m_descriptor->get_commands_rate());
    return ts::success;
}

ts::Result<void> Multirotor_Pilot::start(Clock::time_point tp)
{
    m_output_stream->set_tp(tp);
    return ts::success;
}

auto Multirotor_Pilot::get_inputs() const -> std::vector<Input>
{
    std::vector<Input> inputs =
    {{
        { stream::IMultirotor_State::TYPE,  m_descriptor->get_state_rate(), "state", m_state_accumulator.get_stream_path(0) },
        { stream::IVideo::TYPE,        m_descriptor->get_video_rate(), "video", m_video_accumulator.get_stream_path(0) },
    }};
    return inputs;
}
auto Multirotor_Pilot::get_outputs() const -> std::vector<Output>
{
    std::vector<Output> outputs =
    {{
         { "commands", m_output_stream },
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
        auto const& commandsOpt = m_rc_comms.get_multirotor_commands();

        auto now = Clock::now();

        stream::IMultirotor_Commands::Value& commands_value = m_last_commands_value;
        if (commandsOpt)
        {
            commands_value = commandsOpt.get();
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
        m_rc_comms.set_multirotor_state(i_state.value);
    });
    //write back the video
    m_video_accumulator.process([this](stream::IVideo::Sample const& i_video)
    {
        m_rc_comms.add_video_data(i_video.value);
    });
}

ts::Result<void> Multirotor_Pilot::set_input_stream_path(size_t idx, std::string const& path)
{
    if (idx == 0)
    {
        return m_state_accumulator.set_stream_path(0, path, m_descriptor->get_state_rate(), m_hal);
    }
    else
    {
        return m_video_accumulator.set_stream_path(0, path, m_descriptor->get_video_rate(), m_hal);
    }
}

ts::Result<void> Multirotor_Pilot::set_config(hal::INode_Config const& config)
{
    QLOG_TOPIC("Multirotor_Pilot::set_config");

    auto specialized = dynamic_cast<hal::Multirotor_Pilot_Config const*>(&config);
    if (!specialized)
    {
        return make_error("Wrong config type");
    }
    *m_config = *specialized;

    return ts::success;
}
auto Multirotor_Pilot::get_config() const -> std::shared_ptr<const hal::INode_Config>
{
    return m_config;
}

auto Multirotor_Pilot::get_descriptor() const -> std::shared_ptr<const hal::INode_Descriptor>
{
    return m_descriptor;
}

//auto Multirotor_Pilot::send_message(rapidjson::Value const& /*json*/) -> rapidjson::Document
//{
//    return rapidjson::Document();
//}

}
}
