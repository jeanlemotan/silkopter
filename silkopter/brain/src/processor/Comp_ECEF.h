#pragma once

#include "common/node/IProcessor.h"
#include "common/node/stream/ILinear_Acceleration.h"
#include "common/node/stream/IECEF.h"
#include "common/node/stream/IPressure.h"

#include "HAL.h"


namespace sz
{
namespace Comp_ECEF
{
struct Init_Params;
struct Config;
}
}


namespace silk
{
namespace node
{

class Comp_ECEF : public IProcessor
{
public:
    Comp_ECEF(HAL& hal);

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
    std::shared_ptr<sz::Comp_ECEF::Init_Params> m_init_params;
    std::shared_ptr<sz::Comp_ECEF::Config> m_config;

    q::Clock::duration m_dt = q::Clock::duration(0);

    stream::IECEF_wptr m_ecef_stream;
    stream::ILinear_Acceleration_wptr m_linear_acceleration_stream;
    stream::IPressure_wptr m_pressure_stream;

    std::vector<stream::IECEF::Sample> m_ecef_samples;
    std::vector<stream::ILinear_Acceleration::Sample> m_linear_acceleration_samples;
    std::vector<stream::IPressure::Sample> m_pressure_samples;

    struct Stream : public stream::IECEF
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

