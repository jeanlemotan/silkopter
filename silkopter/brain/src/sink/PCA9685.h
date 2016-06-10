#pragma once

#include "UAV.h"
#include "common/node/ISink.h"
#include "common/stream/IPWM.h"
#include "common/bus/II2C.h"


namespace silk
{
struct PCA9685_Descriptor;
struct PCA9685_Config;
}


namespace silk
{
namespace node
{

class PCA9685 : public ISink
{
public:
    PCA9685(UAV& uav);
    ~PCA9685();

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

    static std::mutex s_pwm_enabled_mutex;
    static size_t s_pwm_enabled_count;
    bool m_has_enabled_all_pwm = false;

    auto set_all_pwm_enabled(bool val) -> bool;

    auto restart(bus::II2C& i2c) -> bool;

    UAV& m_uav;

    std::weak_ptr<bus::II2C> m_i2c;

    std::shared_ptr<PCA9685_Descriptor> m_descriptor;
    std::shared_ptr<PCA9685_Config> m_config;

    struct PWM_Channel
    {
        //sz::PCA9685::PWM_Channel* config = nullptr;
        std::weak_ptr<stream::IPWM> stream;
        struct Last_Data
        {
            int pulse = -1;
        } last_data;
        q::Path stream_path;
    };

    std::vector<PWM_Channel> m_pwm_channels;

    void set_pwm_value(bus::II2C& i2c, size_t idx, boost::optional<float> value);
    std::array<uint8_t, 16 * 4> m_i2c_data;
    size_t m_i2c_data_first = 0;
    size_t m_i2c_data_last = 0;
};

}
}
