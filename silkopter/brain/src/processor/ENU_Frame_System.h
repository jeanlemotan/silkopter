#pragma once

#include "common/node/IProcessor.h"
#include "common/stream/IFrame.h"
#include "common/stream/IPosition.h"
#include "HAL.h"
#include "Sample_Accumulator.h"
#include "Basic_Output_Stream.h"

namespace sz
{
namespace ENU_Frame_System
{
struct Init_Params;
struct Config;
}
}


namespace silk
{
namespace node
{

class ENU_Frame_System : public IProcessor
{
public:
    ENU_Frame_System(HAL& hal);

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

    std::shared_ptr<sz::ENU_Frame_System::Init_Params> m_init_params;
    std::shared_ptr<sz::ENU_Frame_System::Config> m_config;

    Sample_Accumulator<stream::IECEF_Position> m_accumulator;

    typedef Basic_Output_Stream<stream::IENU_Frame> Output_Stream;
    mutable std::shared_ptr<Output_Stream> m_output_stream;
};



}
}