#pragma once

#include "common/node/IProcessor.h"
#include "common/node/stream/ILocal_Frame.h"
#include "common/node/stream/IAngular_Velocity.h"
#include "HAL.h"


namespace sz
{
namespace Stability_Model
{
struct Init_Params;
struct Config;
}
}



namespace silk
{
namespace node
{

class Stability_Model : public IProcessor
{
public:
    Stability_Model(HAL& hal);

    auto init(rapidjson::Value const& init_params) -> bool;
    auto get_init_params() const -> rapidjson::Document const&;

    auto set_config(rapidjson::Value const& json) -> bool;
    auto get_config() const -> rapidjson::Document;

    auto get_inputs() const -> std::vector<Input>;
    auto get_outputs() const -> std::vector<Output>;

    void process();

private:
    auto init() -> bool;

    HAL& m_hal;

    rapidjson::Document m_init_paramsj;
    std::shared_ptr<sz::Stability_Model::Init_Params> m_init_params;
    std::shared_ptr<sz::Stability_Model::Config> m_config;

    q::Clock::duration m_dt = q::Clock::duration(0);

    stream::ILocal_Frame_wptr m_input_stream;
    stream::ILocal_Frame_wptr m_target_stream;

    std::vector<stream::ILocal_Frame::Sample> m_input_samples;
    std::vector<stream::ILocal_Frame::Sample> m_target_samples;

    struct Stream : public stream::IAngular_Velocity
    {
        auto get_samples() const -> std::vector<Sample> const& { return samples; }
        auto get_rate() const -> uint32_t { return rate; }

        Sample last_sample;
        std::vector<Sample> samples;
        uint32_t rate = 0;
    };
    mutable std::shared_ptr<Stream> m_output_stream;
};



}
}
