#pragma once

#include "HAL.h"
#include "common/node/stream/IThrottle.h"
#include "common/node/stream/IPWM.h"
#include "common/node/IProcessor.h"


namespace sz
{
namespace Throttle_PWM
{
struct Init_Params;
struct Config;
}
}

namespace silk
{
namespace node
{

class Throttle_PWM : public IProcessor
{
public:
    Throttle_PWM(HAL& hal);

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
    std::shared_ptr<sz::Throttle_PWM::Init_Params> m_init_params;
    std::shared_ptr<sz::Throttle_PWM::Config> m_config;

    IThrottle_wptr m_input_stream;

    struct Stream : public IPWM
    {
        auto get_samples() const -> std::vector<Sample> const& { return samples; }
        auto get_rate() const -> uint32_t { return rate; }

        uint32_t rate = 0;
        std::vector<Sample> samples;
    };
    mutable std::shared_ptr<Stream> m_output_stream;
};


}
}

