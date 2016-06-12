#pragma once

#include "UAV.h"
#include "common/node/ISink.h"
#include "common/stream/IPWM.h"

namespace silk
{
namespace uav
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
    PIGPIO(UAV& uav);
    ~PIGPIO();

    static const size_t MAX_PWM_CHANNELS = 8;

    bool init(uav::INode_Descriptor const& descriptor) override;
    std::shared_ptr<const uav::INode_Descriptor> get_descriptor() const override;

    bool set_config(uav::INode_Config const& config) override;
    std::shared_ptr<const uav::INode_Config> get_config() const override;

    //auto send_message(rapidjson::Value const& json) -> rapidjson::Document;

    auto start(q::Clock::time_point tp) -> bool override;

    void set_input_stream_path(size_t idx, q::Path const& path);
    auto get_inputs() const -> std::vector<Input>;

    void process();

private:
    auto init() -> bool;

    UAV& m_uav;

    std::shared_ptr<uav::PIGPIO_Descriptor> m_descriptor;
    std::shared_ptr<uav::PIGPIO_Config> m_config;

    struct Channel;

    std::vector<std::unique_ptr<Channel>> m_channels;

    void set_pwm_value(size_t idx, float value);
};

}
}
