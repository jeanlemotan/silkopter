#pragma once

#include "common/node/IProcessor.h"
#include "common/stream/IFrame.h"
#include "common/stream/IPosition.h"
#include "HAL.h"
#include "Sample_Accumulator.h"
#include "Basic_Output_Stream.h"


namespace silk
{
namespace hal
{
struct ENU_Frame_System_Descriptor;
struct ENU_Frame_System_Config;
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

    bool init(hal::INode_Descriptor const& descriptor) override;
    std::shared_ptr<const hal::INode_Descriptor> get_descriptor() const override;

    bool set_config(hal::INode_Config const& config) override;
    std::shared_ptr<const hal::INode_Config> get_config() const override;

    //auto send_message(rapidjson::Value const& json) -> rapidjson::Document;

    auto start(q::Clock::time_point tp) -> bool override;

    void set_input_stream_path(size_t idx, q::Path const& path);
    auto get_inputs() const -> std::vector<Input>;
    auto get_outputs() const -> std::vector<Output>;

    void process();

private:
    auto init() -> bool;

    HAL& m_hal;

    std::shared_ptr<hal::ENU_Frame_System_Descriptor> m_descriptor;
    std::shared_ptr<hal::ENU_Frame_System_Config> m_config;

    Sample_Accumulator<stream::IECEF_Position> m_accumulator;

    typedef Basic_Output_Stream<stream::IENU_Frame> Output_Stream;
    mutable std::shared_ptr<Output_Stream> m_output_stream;
};



}
}
