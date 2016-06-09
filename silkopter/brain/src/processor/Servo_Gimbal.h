#pragma once

#include "common/node/IProcessor.h"
#include "common/stream/IFrame.h"
#include "common/stream/IFloat.h"
#include "common/stream/IPWM.h"
#include "common/stream/IMultirotor_Commands.h"
#include "UAV.h"

#include "Sample_Accumulator.h"
#include "Basic_Output_Stream.h"


namespace silk
{
struct Servo_Gimbal_Descriptor;
struct Servo_Gimbal_Config;
}



namespace silk
{
namespace node
{

class Servo_Gimbal : public IProcessor
{
public:
    Servo_Gimbal(UAV& uav);

    bool init(std::shared_ptr<Node_Descriptor_Base> descriptor) override;
    std::shared_ptr<Node_Descriptor_Base> get_descriptor() const override;

    bool set_config(std::shared_ptr<Node_Config_Base> config) override;
    std::shared_ptr<Node_Config_Base> get_config() const override;

    auto send_message(rapidjson::Value const& json) -> rapidjson::Document;

    auto start(q::Clock::time_point tp) -> bool override;

    void set_input_stream_path(size_t idx, q::Path const& path);
    auto get_inputs() const -> std::vector<Input>;
    auto get_outputs() const -> std::vector<Output>;

    void process();

private:
    auto init() -> bool;

    UAV& m_uav;

    std::shared_ptr<Servo_Gimbal_Descriptor> m_descriptor;
    std::shared_ptr<Servo_Gimbal_Config> m_config;

    Sample_Accumulator<stream::IUAV_Frame> m_frame_accumulator;
    Sample_Accumulator<stream::IMultirotor_Commands> m_commands_accumulator;

    stream::IMultirotor_Commands::Sample m_commands_sample;

    std::vector<stream::IUAV_Frame::Sample> m_frame_samples;
    std::vector<stream::IUAV_Frame::Sample> m_target_frame_samples;

    typedef Basic_Output_Stream<stream::IPWM> Output_Stream;
    mutable std::shared_ptr<Output_Stream> m_x_output_stream;
    mutable std::shared_ptr<Output_Stream> m_y_output_stream;
    mutable std::shared_ptr<Output_Stream> m_z_output_stream;
};



}
}
