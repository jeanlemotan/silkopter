#pragma once

#include "common/node/IProcessor.h"
#include "common/node/stream/IFrame.h"
#include "common/node/stream/IFloat.h"
#include "common/node/stream/IPWM.h"
#include "HAL.h"

#include "Sample_Accumulator.h"
#include "Basic_Output_Stream.h"

namespace sz
{
namespace Servo_Gimbal
{
struct Init_Params;
struct Config;
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

    std::shared_ptr<sz::Servo_Gimbal::Init_Params> m_init_params;
    std::shared_ptr<sz::Servo_Gimbal::Config> m_config;

    Sample_Accumulator<stream::IFrame/*, stream::IFrame*/> m_accumulator;

    std::vector<stream::IFrame::Sample> m_frame_samples;
    std::vector<stream::IFrame::Sample> m_target_frame_samples;

    typedef Basic_Output_Stream<stream::IPWM> Output_Stream;
    mutable std::shared_ptr<Output_Stream> m_x_output_stream;
    mutable std::shared_ptr<Output_Stream> m_y_output_stream;
    mutable std::shared_ptr<Output_Stream> m_z_output_stream;
};



}
}
