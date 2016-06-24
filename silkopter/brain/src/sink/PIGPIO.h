#pragma once

#include "HAL.h"
#include "common/node/ISink.h"
#include "common/stream/IPWM.h"

namespace silk
{
namespace hal
{
struct PIGPIO_Descriptor;
struct PIGPIO_Config;
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
    ~PIGPIO();

    static const size_t MAX_PWM_CHANNELS = 8;

    bool init(hal::INode_Descriptor const& descriptor) override;
    std::shared_ptr<const hal::INode_Descriptor> get_descriptor() const override;

    bool set_config(hal::INode_Config const& config) override;
    std::shared_ptr<const hal::INode_Config> get_config() const override;

    //auto send_message(rapidjson::Value const& json) -> rapidjson::Document;

    auto start(q::Clock::time_point tp) -> bool override;

    void set_input_stream_path(size_t idx, q::Path const& path);
    auto get_inputs() const -> std::vector<Input>;

    void process();

private:
    auto init() -> bool;

    HAL& m_hal;

    std::shared_ptr<hal::PIGPIO_Descriptor> m_descriptor;
    std::shared_ptr<hal::PIGPIO_Config> m_config;

    struct Channel;

    std::vector<std::unique_ptr<Channel>> m_channels;

    void set_pwm_value(size_t idx, float value);
};

}
}
