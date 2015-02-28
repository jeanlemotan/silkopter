#pragma once

#include "HAL.h"
#include "common/node/sink/IPWM.h"

namespace sz
{
namespace PIGPIO
{
class Init_Params;
class Config;
}
}


namespace silk
{
namespace node
{
namespace sink
{

class PIGPIO : public ISink
{
public:
    PIGPIO(HAL& hal);

    static const size_t MAX_PWM_CHANNELS = 8;

    auto init(rapidjson::Value const& json) -> bool;
    auto get_init_params() -> rapidjson::Document;

    auto set_config(rapidjson::Value const& json) -> bool;
    auto get_config() -> rapidjson::Document;

    auto get_name() const -> std::string const&;
    auto get_input_stream_count() const -> size_t;
    auto get_input_stream(size_t idx) -> stream::IPWM_Value&;

    void process();

private:
    auto init() -> bool;

    HAL& m_hal;

    std::shared_ptr<sz::PIGPIO::Init_Params> m_init_params;
    std::shared_ptr<sz::PIGPIO::Config> m_config;

    struct PWM_Channel
    {
        stream::IPWM_Value* stream = nullptr;
        uint32_t gpio = 0;
    };

    std::vector<PWM_Channel> m_pwm_channels;

    void set_pwm_value(size_t idx, float value);
};

}
}
}
