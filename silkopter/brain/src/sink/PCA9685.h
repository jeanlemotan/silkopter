#pragma once

#include "HAL.h"
#include "common/node/ISink.h"
#include "common/node/stream/IPWM.h"
#include "common/node/bus/II2C.h"

namespace sz
{
namespace PCA9685
{
struct Init_Params;
struct Config;
struct PWM_Channel;
}
}


namespace silk
{
namespace node
{

class PCA9685 : public ISink
{
public:
    PCA9685(HAL& hal);

    static const size_t MAX_PWM_CHANNELS = 8;

    auto init(rapidjson::Value const& init_params) -> bool;
    auto get_init_params() const -> rapidjson::Document const&;

    auto set_config(rapidjson::Value const& json) -> bool;
    auto get_config() const -> rapidjson::Document;

    auto send_message(rapidjson::Value const& json) -> rapidjson::Document;

    auto get_stream_inputs() const -> std::vector<Stream_Input>;

    void process();

private:
    auto init() -> bool;

    HAL& m_hal;

    std::weak_ptr<bus::II2C> m_i2c;

    rapidjson::Document m_init_paramsj;
    std::shared_ptr<sz::PCA9685::Init_Params> m_init_params;
    std::shared_ptr<sz::PCA9685::Config> m_config;

    struct PWM_Channel
    {
        sz::PCA9685::PWM_Channel* config = nullptr;
        stream::IPWM_wptr stream;
    };

    std::vector<PWM_Channel> m_pwm_channels;

    void set_pwm_value(size_t idx, float value);
};

}
}
