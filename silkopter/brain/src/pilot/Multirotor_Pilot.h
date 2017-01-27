#pragma once

#include "common/node/IPilot.h"

#include "common/stream/IMultirotor_Commands.h"
#include "common/stream/IMultirotor_State.h"
#include "common/stream/IVideo.h"

#include "RC_Comms.h"
#include "HAL.h"

#include "Sample_Accumulator.h"
#include "Basic_Output_Stream.h"


namespace silk
{
namespace hal
{
struct Multirotor_Pilot_Descriptor;
struct Multirotor_Pilot_Config;
}
}

namespace silk
{
namespace node
{

class Multirotor_Pilot : public IPilot
{
public:
    Multirotor_Pilot(HAL& hal, RC_Comms& rc_comms);

    ts::Result<void> init(hal::INode_Descriptor const& descriptor) override;
    std::shared_ptr<const hal::INode_Descriptor> get_descriptor() const override;

    ts::Result<void> set_config(hal::INode_Config const& config) override;
    std::shared_ptr<const hal::INode_Config> get_config() const override;

    //auto send_message(rapidjson::Value const& json) -> rapidjson::Document;

    ts::Result<void> start(Clock::time_point tp) override;

    ts::Result<void> set_input_stream_path(size_t idx, std::string const& path);
    auto get_inputs() const -> std::vector<Input>;
    auto get_outputs() const -> std::vector<Output>;

    void process();

private:
    ts::Result<void> init();

    HAL& m_hal;

    RC_Comms& m_rc_comms;

    stream::IMultirotor_Commands::Value m_last_commands_value;
    Clock::time_point m_last_received_commands_value_tp = Clock::now();

    std::shared_ptr<hal::Multirotor_Pilot_Descriptor> m_descriptor;
    std::shared_ptr<hal::Multirotor_Pilot_Config> m_config;

    Sample_Accumulator<stream::IMultirotor_State> m_state_accumulator;
    Sample_Accumulator<stream::IVideo> m_video_accumulator;

    typedef Basic_Output_Stream<stream::IMultirotor_Commands> Output_Stream;
    mutable std::shared_ptr<Output_Stream> m_output_stream;
};



}
}
