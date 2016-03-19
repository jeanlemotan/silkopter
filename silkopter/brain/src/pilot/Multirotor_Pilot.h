#pragma once

#include "common/node/IPilot.h"

#include "common/stream/IMultirotor_Commands.h"
#include "common/stream/IMultirotor_State.h"
#include "common/stream/IVideo.h"

#include "Comms.h"
#include "HAL.h"

#include "Sample_Accumulator.h"
#include "Basic_Output_Stream.h"


namespace sz
{
namespace Multirotor_Pilot
{
struct Init_Params;
struct Config;
}
}

namespace silk
{
namespace node
{

class Multirotor_Pilot : public IPilot
{
public:
    Multirotor_Pilot(HAL& hal, Comms& comms);

    auto init(rapidjson::Value const& init_params) -> bool;
    auto get_init_params() const -> rapidjson::Document;

    auto set_config(rapidjson::Value const& json) -> bool;
    auto get_config() const -> rapidjson::Document;

    auto send_message(rapidjson::Value const& json) -> rapidjson::Document;

    auto start(q::Clock::time_point tp) -> bool override;

    void set_input_stream_path(size_t idx, q::Path const& path);
    auto get_inputs() const -> std::vector<Input>;
    auto get_outputs() const -> std::vector<Output>;

    void process();

private:
    auto init() -> bool;

    HAL& m_hal;
    Comms& m_comms;

    stream::IMultirotor_Commands::Value m_last_commands_value;
    q::Clock::time_point m_last_received_commands_value_tp = q::Clock::now();

    std::shared_ptr<sz::Multirotor_Pilot::Init_Params> m_init_params;
    std::shared_ptr<sz::Multirotor_Pilot::Config> m_config;

    Sample_Accumulator<stream::IMultirotor_State> m_state_accumulator;
    Sample_Accumulator<stream::IVideo> m_video_accumulator;

    typedef Basic_Output_Stream<stream::IMultirotor_Commands> Output_Stream;
    mutable std::shared_ptr<Output_Stream> m_output_stream;
};



}
}
