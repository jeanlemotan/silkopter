#pragma once

#include "common/node/IController.h"
#include "common/node/stream/IVelocity.h"
#include "common/node/stream/IFrame.h"
#include "common/node/stream/IForce.h"
#include "HAL.h"


namespace sz
{
namespace Velocity_Controller
{
struct Init_Params;
struct Config;
}
}



namespace silk
{
namespace node
{

class Velocity_Controller : public IController
{
public:
    Velocity_Controller(HAL& hal);

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
    std::shared_ptr<sz::Velocity_Controller::Init_Params> m_init_params;
    std::shared_ptr<sz::Velocity_Controller::Config> m_config;

    q::Clock::duration m_dt = q::Clock::duration(0);

    stream::IVelocity_wptr m_input_stream;
    stream::IVelocity_wptr m_target_stream;

    std::vector<stream::IVelocity::Sample> m_input_samples;
    std::vector<stream::IVelocity::Sample> m_target_samples;

    struct Frame : public stream::IFrame
    {
        auto get_samples() const -> std::vector<Sample> const& { return samples; }
        auto get_rate() const -> uint32_t { return rate; }

        Sample last_sample;
        std::vector<Sample> samples;
        uint32_t rate = 0;
    };
    mutable std::shared_ptr<Frame> m_frame_stream;
    struct Force : public stream::IForce
    {
        auto get_samples() const -> std::vector<Sample> const& { return samples; }
        auto get_rate() const -> uint32_t { return rate; }

        Sample last_sample;
        std::vector<Sample> samples;
        uint32_t rate = 0;
    };
    mutable std::shared_ptr<Force> m_force_stream;
};



}
}
