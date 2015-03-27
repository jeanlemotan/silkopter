#pragma once

#include "HAL.h"
#include "common/node/stream/IFactor.h"
#include "common/node/IGenerator.h"


namespace sz
{
namespace Factor_Generator
{
struct Init_Params;
struct Config;
}
}

namespace silk
{
namespace node
{

class Factor_Generator : public IGenerator
{
public:
    Factor_Generator(HAL& hal);

    auto init(rapidjson::Value const& init_params) -> bool;
    auto get_init_params() const -> rapidjson::Document const&;

    auto set_config(rapidjson::Value const& json) -> bool;
    auto get_config() const -> rapidjson::Document;

    auto send_message(rapidjson::Value const& json) -> rapidjson::Document;

    auto get_inputs() const -> std::vector<Input>;
    auto get_outputs() const -> std::vector<Output>;

    void process();

private:
    auto init() -> bool;

    HAL& m_hal;

    rapidjson::Document m_init_paramsj;
    std::shared_ptr<sz::Factor_Generator::Init_Params> m_init_params;
    std::shared_ptr<sz::Factor_Generator::Config> m_config;

    q::Clock::time_point m_last_tp;
    q::Clock::duration m_dt;
    float m_period = 0;
    std::uniform_real_distribution<float> m_rnd_distribution;
    std::mt19937 m_rnd_engine;

    struct Stream : public stream::IFactor
    {
        auto get_samples() const -> std::vector<Sample> const& { return samples; }
        auto get_rate() const -> uint32_t { return rate; }

        uint32_t rate = 0;
        std::vector<Sample> samples;
        uint32_t sample_idx = 0;
    };
    mutable std::shared_ptr<Stream> m_output_stream;
};


}
}

