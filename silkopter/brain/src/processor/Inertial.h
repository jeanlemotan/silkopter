#pragma once

#include "common/node/processor/IProcessor.h"
#include "common/node/stream/IReference_Frame.h"
#include "common/node/stream/IAcceleration.h"
#include "common/node/stream/ILinear_Acceleration.h"
#include "HAL.h"

namespace sz
{
namespace Inertial
{
struct Init_Params;
struct Config;
}
}


namespace silk
{
namespace node
{
namespace processor
{

class Inertial : public IProcessor
{
public:
    Inertial(HAL& hal);

    auto init(rapidjson::Value const& init_params) -> bool;
    auto get_init_params() -> rapidjson::Document;

    auto set_config(rapidjson::Value const& json) -> bool;
    auto get_config() -> rapidjson::Document;

    auto get_inputs() const -> std::vector<Input>;
    auto get_outputs() const -> std::vector<Output>;

    void process();

private:
    auto init() -> bool;

    HAL& m_hal;

    std::shared_ptr<sz::Inertial::Init_Params> m_init_params;
    std::shared_ptr<sz::Inertial::Config> m_config;

    q::Clock::duration m_dt = q::Clock::duration(0);

    stream::IReference_Frame_wptr m_reference_frame_stream;
    stream::IAcceleration_wptr m_acceleration_stream;

    std::vector<stream::IReference_Frame::Sample> m_reference_frame_samples;
    std::vector<stream::IAcceleration::Sample> m_acceleration_samples;

    struct Stream : public stream::ILinear_Acceleration
    {
        auto get_samples() const -> std::vector<Sample> const& { return samples; }
        auto get_rate() const -> uint32_t { return rate; }

        uint32_t rate = 0;
        Sample last_sample;
        std::vector<Sample> samples;
    };
    mutable std::shared_ptr<Stream> m_stream;
};



}
}
}
