#pragma once

#include "HAL.h"
#include "common/node/sink/IPWM.h"

namespace silk
{
namespace node
{
namespace sink
{

class PIGPIO : q::util::Noncopyable
{
public:
    PIGPIO(HAL& hal);

    static const size_t MAX_PWM_CHANNELS = 8;

    struct PWM_Params
    {
        int32_t gpio = -1;
        uint32_t frequency = 50;
        uint32_t range = 1000000 / frequency;
        uint32_t min = 1000;
        uint32_t max = 2000;
    };

    struct Init_Params
    {
        std::string name;
        std::vector<PWM_Params> pwm_channels;
        std::chrono::microseconds period = std::chrono::microseconds(5);
    };

    auto init(rapidjson::Value const& json) -> bool;
    auto init(Init_Params const& params) -> bool;

    void process();

    auto get_pwm_channel(size_t idx) -> IPWM*;

private:
    auto init() -> bool;

    HAL& m_hal;
    Init_Params m_params;

    void set_pwm_value(size_t idx, float value);

    struct PWM : public IPWM
    {
        void set_value(float value) { pigpio->set_pwm_value(idx, value); }
        auto get_name() const -> std::string const& { return name; }

        PIGPIO* pigpio = nullptr;
        size_t idx = 0;
        std::string name;
    };

    std::array<PWM, MAX_PWM_CHANNELS> m_pwm_channels;
};

}
}
}
