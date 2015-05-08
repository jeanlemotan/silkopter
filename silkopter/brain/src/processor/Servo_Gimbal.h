#pragma once

#include "common/node/IProcessor.h"
#include "common/node/stream/IFrame.h"
#include "common/node/stream/IFloat.h"
#include "common/node/stream/IPWM.h"
#include "HAL.h"


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
    auto get_init_params() const -> rapidjson::Document const&;

    auto set_config(rapidjson::Value const& json) -> bool;
    auto get_config() const -> rapidjson::Document;

    auto send_message(rapidjson::Value const& json) -> rapidjson::Document;

    auto get_stream_inputs() const -> std::vector<Stream_Input>;
    auto get_stream_outputs() const -> std::vector<Stream_Output>;

    void process();

private:
    auto init() -> bool;

    HAL& m_hal;

    rapidjson::Document m_init_paramsj;
    std::shared_ptr<sz::Servo_Gimbal::Init_Params> m_init_params;
    std::shared_ptr<sz::Servo_Gimbal::Config> m_config;

    q::Clock::duration m_dt = q::Clock::duration(0);

    stream::IFrame_wptr m_frame_stream;
    stream::IFrame_wptr m_target_stream;

    std::vector<stream::IFrame::Sample> m_frame_samples;
    std::vector<stream::IFrame::Sample> m_target_frame_samples;

    struct Stream : public stream::IPWM
    {
        auto get_samples() const -> std::vector<Sample> const& { return samples; }
        auto get_rate() const -> uint32_t { return rate; }

        Sample last_sample;
        std::vector<Sample> samples;
        uint32_t rate = 0;
    };
    mutable std::shared_ptr<Stream> m_x_output_stream;
    mutable std::shared_ptr<Stream> m_y_output_stream;
    mutable std::shared_ptr<Stream> m_z_output_stream;
};



}
}
