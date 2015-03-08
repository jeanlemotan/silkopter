#pragma once

#include "HAL.h"
#include "common/node/ISink.h"
#include "common/node/stream/IPWM_Value.h"

namespace sz
{
namespace PIGPIO
{
struct Init_Params;
struct Config;
}
}


namespace silk
{
namespace node
{

class PIGPIO : public ISink
{
public:
    PIGPIO(HAL& hal);

    static const size_t MAX_PWM_CHANNELS = 8;

    auto init(rapidjson::Value const& init_params) -> bool;
    auto get_init_params() const -> rapidjson::Document;

    auto set_config(rapidjson::Value const& json) -> bool;
    auto get_config() const -> rapidjson::Document;

    auto get_inputs() const -> std::vector<Input>;

    void process();

private:
    auto init() -> bool;

    HAL& m_hal;

    std::shared_ptr<sz::PIGPIO::Init_Params> m_init_params;
    std::shared_ptr<sz::PIGPIO::Config> m_config;

    struct PWM_Channel
    {
        stream::IPWM_Value_wptr stream;
        uint32_t gpio = 0;
    };

    std::vector<PWM_Channel> m_pwm_channels;

    void set_pwm_value(size_t idx, float value);
};

}
}
