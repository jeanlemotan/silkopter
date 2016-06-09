#pragma once

#include "UAV.h"
#include "common/node/ISink.h"
#include "common/stream/IPWM.h"

namespace silk
{
struct Node_Descriptor_Base;
struct PIGPIO_Descriptor;

struct Node_Config_Base;
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

    bool init(std::shared_ptr<Node_Descriptor_Base> descriptor) override;
    std::shared_ptr<Node_Descriptor_Base> get_descriptor() const override;

    bool set_config(std::shared_ptr<Node_Config_Base> config) override;
    std::shared_ptr<Node_Config_Base> get_config() const override;

    auto send_message(rapidjson::Value const& json) -> rapidjson::Document;

    auto start(q::Clock::time_point tp) -> bool override;

    void set_input_stream_path(size_t idx, q::Path const& path);
    auto get_inputs() const -> std::vector<Input>;

    void process();

private:
    auto init() -> bool;

    UAV& m_uav;

    std::shared_ptr<PIGPIO_Descriptor> m_descriptor;
    std::shared_ptr<PIGPIO_Config> m_config;

    struct PWM_Channel
    {
        bool is_servo = false;
        uint32_t rate = 0;
        //sz::PIGPIO::PWM_Config* config = nullptr;
        uint32_t gpio = 0;
        q::Path stream_path;
        std::weak_ptr<stream::IPWM> stream;
    };

    std::vector<PWM_Channel> m_pwm_channels;

    void set_pwm_value(size_t idx, float value);
};

}
}
