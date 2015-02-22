#pragma once

#include "HAL.h"
#include "common/node/sink/IPWM.h"

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

    struct PWM_Params
    {
        stream::IPWM_Value* stream = nullptr;
        uint32_t gpio = 0;
        uint32_t rate = 50;
        uint32_t range = 1000000 / rate;
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

    auto get_name() const -> std::string const&;
    auto get_input_stream_count() const -> size_t;
    auto get_input_stream(size_t idx) -> stream::IPWM_Value&;

    void process();

private:
    auto init() -> bool;

    HAL& m_hal;
    Init_Params m_params;

    void set_pwm_value(size_t idx, float value);
};

}
}
}
