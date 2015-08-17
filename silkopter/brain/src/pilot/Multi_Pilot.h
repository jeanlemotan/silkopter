#pragma once

#include "common/node/IPilot.h"

#include "common/node/stream/IMulti_Input.h"
#include "common/node/stream/IMulti_State.h"

#include "Comms.h"
#include "HAL.h"

#include "Sample_Accumulator.h"
#include "Basic_Output_Stream.h"


namespace sz
{
namespace Multi_Pilot
{
struct Init_Params;
struct Config;
}
}

namespace silk
{
namespace node
{

class Multi_Pilot : public IPilot
{
public:
    Multi_Pilot(HAL& hal, Comms& comms);

    auto init(rapidjson::Value const& init_params) -> bool;
    auto get_init_params() const -> rapidjson::Document;

    auto set_config(rapidjson::Value const& json) -> bool;
    auto get_config() const -> rapidjson::Document;

    auto send_message(rapidjson::Value const& json) -> rapidjson::Document;

    void set_input_stream_path(size_t idx, q::Path const& path);
    auto get_inputs() const -> std::vector<Input>;
    auto get_outputs() const -> std::vector<Output>;

    void process();

private:
    auto init() -> bool;

    HAL& m_hal;
    Comms& m_comms;

    stream::IMulti_Input::Value m_last_input_value;
    q::Clock::time_point m_last_received_input_value_tp = q::Clock::now();

    std::shared_ptr<sz::Multi_Pilot::Init_Params> m_init_params;
    std::shared_ptr<sz::Multi_Pilot::Config> m_config;

    Sample_Accumulator<stream::IMulti_State> m_accumulator;

    typedef Basic_Output_Stream<stream::IMulti_Input> Output_Stream;
    mutable std::shared_ptr<Output_Stream> m_output_stream;
};



}
}
