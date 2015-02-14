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

    struct Init_Params
    {
        struct PWM
        {
            int gpio = -1;
            size_t frequency = 50;
            size_t range = 1000000 / frequency;
            size_t min = 1000;
            size_t max = 2000;
        };

        q::String name;
        std::array<PWM, MAX_PWM_CHANNELS> pwm_channels;
        std::chrono::microseconds rate = std::chrono::microseconds(5);
    };

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

        PIGPIO* pigpio = nullptr;
        size_t idx = 0;
    };

    std::array<PWM, MAX_PWM_CHANNELS> m_pwm_channels;
};

DECLARE_CLASS_PTR(PIGPIO);

}
}
}
