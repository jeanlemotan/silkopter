#pragma once

#include "UAV.h"
#include "common/node/ISink.h"
#include "common/stream/IPWM.h"

namespace silk
{
struct INode_Descriptor;
struct PIGPIO_Descriptor;

struct INode_Config;
struct PIGPIO_Config;
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

    bool init(std::shared_ptr<INode_Descriptor> descriptor) override;
    std::shared_ptr<INode_Descriptor> get_descriptor() const override;

    bool set_config(std::shared_ptr<INode_Config> config) override;
    std::shared_ptr<INode_Config> get_config() const override;

    //auto send_message(rapidjson::Value const& json) -> rapidjson::Document;

    auto start(q::Clock::time_point tp) -> bool override;

    void set_input_stream_path(size_t idx, q::Path const& path);
    auto get_inputs() const -> std::vector<Input>;

    void process();

private:
    auto init() -> bool;

    UAV& m_uav;

    std::shared_ptr<PIGPIO_Descriptor> m_descriptor;
    std::shared_ptr<PIGPIO_Config> m_config;

    struct Channel;

    std::vector<std::unique_ptr<Channel>> m_channels;

    void set_pwm_value(size_t idx, float value);
};

}
}
