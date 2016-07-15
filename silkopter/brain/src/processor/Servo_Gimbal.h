#pragma once

#include "common/node/IProcessor.h"
#include "common/stream/IFrame.h"
#include "common/stream/IFloat.h"
#include "common/stream/IPWM.h"
#include "common/stream/IMultirotor_Commands.h"
#include "HAL.h"

#include "Sample_Accumulator.h"
#include "Basic_Output_Stream.h"


namespace silk
{
namespace hal
{
struct Servo_Gimbal_Descriptor;
struct Servo_Gimbal_Config;
}
}



namespace silk
{
namespace node
{

class Servo_Gimbal : public IProcessor
{
public:
    Servo_Gimbal(HAL& hal);

    ts::Result<void> init(hal::INode_Descriptor const& descriptor) override;
    std::shared_ptr<const hal::INode_Descriptor> get_descriptor() const override;

    ts::Result<void> set_config(hal::INode_Config const& config) override;
    std::shared_ptr<const hal::INode_Config> get_config() const override;

    //auto send_message(rapidjson::Value const& json) -> rapidjson::Document;

    ts::Result<void> start(q::Clock::time_point tp) override;

    ts::Result<void> set_input_stream_path(size_t idx, std::string const& path);
    auto get_inputs() const -> std::vector<Input>;
    auto get_outputs() const -> std::vector<Output>;

    void process();

private:
    ts::Result<void> init();

    HAL& m_hal;

    std::shared_ptr<hal::Servo_Gimbal_Descriptor> m_descriptor;
    std::shared_ptr<hal::Servo_Gimbal_Config> m_config;

    Sample_Accumulator<stream::IFrame> m_frame_accumulator;
    Sample_Accumulator<stream::IMultirotor_Commands> m_commands_accumulator;

    stream::IMultirotor_Commands::Sample m_commands_sample;

    std::vector<stream::IFrame::Sample> m_frame_samples;
    std::vector<stream::IFrame::Sample> m_target_frame_samples;

    typedef Basic_Output_Stream<stream::IPWM> Output_Stream;
    mutable std::shared_ptr<Output_Stream> m_x_output_stream;
    mutable std::shared_ptr<Output_Stream> m_y_output_stream;
    mutable std::shared_ptr<Output_Stream> m_z_output_stream;
};



}
}
