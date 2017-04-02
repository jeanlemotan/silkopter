#pragma once

#include "HAL.h"
#include "common/node/ISink.h"
#include "common/stream/IPWM.h"
#include "common/bus/II2C.h"


namespace silk
{
namespace hal
{
struct PCA9685_Descriptor;
struct PCA9685_Config;
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
    ~PCA9685();

    static const size_t MAX_PWM_CHANNELS = 8;

    ts::Result<void> init(hal::INode_Descriptor const& descriptor) override;
    std::shared_ptr<const hal::INode_Descriptor> get_descriptor() const override;

    ts::Result<void> set_config(hal::INode_Config const& config) override;
    std::shared_ptr<const hal::INode_Config> get_config() const override;

    ts::Result<std::shared_ptr<messages::INode_Message>> send_message(messages::INode_Message const& message) override;

    ts::Result<void> start(Clock::time_point tp) override;

    ts::Result<void> set_input_stream_path(size_t idx, std::string const& path);
    auto get_inputs() const -> std::vector<Input>;

    void process();

private:
    ts::Result<void> init();

    static std::mutex s_pwm_enabled_mutex;
    static size_t s_pwm_enabled_count;
    bool m_has_enabled_all_pwm = false;

    auto set_all_pwm_enabled(bool val) -> bool;

    auto restart(util::hw::II2C& i2c) -> bool;

    HAL& m_hal;

    std::weak_ptr<bus::II2C_Bus> m_i2c_bus;

    std::shared_ptr<hal::PCA9685_Descriptor> m_descriptor;
    std::shared_ptr<hal::PCA9685_Config> m_config;

    struct PWM_Channel
    {
        //sz::PCA9685::PWM_Channel* config = nullptr;
        std::weak_ptr<stream::IPWM> stream;
        struct Last_Data
        {
            int pulse = -1;
        } last_data;
        std::string stream_path;
    };

    std::vector<PWM_Channel> m_pwm_channels;

    void set_pwm_value(util::hw::II2C& i2c, size_t idx, boost::optional<float> value);
    std::array<uint8_t, 16 * 4> m_i2c_data;
    size_t m_i2c_data_first = 0;
    size_t m_i2c_data_last = 0;
};

}
}
