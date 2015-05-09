#pragma once

#include "common/node/IProcessor.h"
#include "common/node/stream/IFrame.h"
#include "common/node/stream/IAcceleration.h"
#include "common/node/stream/ILinear_Acceleration.h"
#include "HAL.h"
#include "Sample_Accumulator.h"

namespace sz
{
namespace Gravity_Filter
{
struct Init_Params;
struct Config;
}
}


namespace silk
{
namespace node
{

class Gravity_Filter : public IProcessor
{
public:
    Gravity_Filter(HAL& hal);

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
    std::shared_ptr<sz::Gravity_Filter::Init_Params> m_init_params;
    std::shared_ptr<sz::Gravity_Filter::Config> m_config;

    q::Clock::duration m_dt = q::Clock::duration(0);

    Sample_Accumulator<stream::IFrame, stream::IAcceleration> m_accumulator;

    struct Stream : public stream::ILinear_Acceleration
    {
        auto get_samples() const -> std::vector<Sample> const& { return samples; }
        auto get_rate() const -> uint32_t { return rate; }

        uint32_t rate = 0;
        Sample last_sample;
        std::vector<Sample> samples;
    };
    mutable std::shared_ptr<Stream> m_output_stream;
};



}
}
