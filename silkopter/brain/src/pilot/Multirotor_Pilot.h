#pragma once

#include "common/node/IPilot.h"

#include "common/stream/IMultirotor_Commands.h"
#include "common/stream/IMultirotor_State.h"
#include "common/stream/IVideo.h"

#include "Comms.h"
#include "UAV.h"

#include "Sample_Accumulator.h"
#include "Basic_Output_Stream.h"


namespace silk
{
struct Multirotor_Pilot_Descriptor;
struct Multirotor_Pilot_Config;
}

namespace silk
{
namespace node
{

class Multirotor_Pilot : public IPilot
{
public:
    Multirotor_Pilot(UAV& uav, Comms& comms);

    bool init(std::shared_ptr<INode_Descriptor> descriptor) override;
    std::shared_ptr<INode_Descriptor> get_descriptor() const override;

    bool set_config(std::shared_ptr<INode_Config> config) override;
    std::shared_ptr<INode_Config> get_config() const override;

    //auto send_message(rapidjson::Value const& json) -> rapidjson::Document;

    auto start(q::Clock::time_point tp) -> bool override;

    void set_input_stream_path(size_t idx, q::Path const& path);
    auto get_inputs() const -> std::vector<Input>;
    auto get_outputs() const -> std::vector<Output>;

    void process();

private:
    auto init() -> bool;

    UAV& m_uav;
    Comms& m_comms;

    stream::IMultirotor_Commands::Value m_last_commands_value;
    q::Clock::time_point m_last_received_commands_value_tp = q::Clock::now();

    std::shared_ptr<Multirotor_Pilot_Descriptor> m_descriptor;
    std::shared_ptr<Multirotor_Pilot_Config> m_config;

    Sample_Accumulator<stream::IMultirotor_State> m_state_accumulator;
    Sample_Accumulator<stream::IVideo> m_video_accumulator;

    typedef Basic_Output_Stream<stream::IMultirotor_Commands> Output_Stream;
    mutable std::shared_ptr<Output_Stream> m_output_stream;
};



}
}
