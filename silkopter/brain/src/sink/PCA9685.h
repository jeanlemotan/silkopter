#pragma once

#include "HAL.h"
#include "common/node/ISink.h"
#include "common/stream/IPWM.h"
#include "common/bus/II2C.h"

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
    ~PCA9685();

    static const size_t MAX_PWM_CHANNELS = 8;

    auto init(rapidjson::Value const& init_params) -> bool;
    auto get_init_params() const -> rapidjson::Document;

    auto set_config(rapidjson::Value const& json) -> bool;
    auto get_config() const -> rapidjson::Document;

    auto send_message(rapidjson::Value const& json) -> rapidjson::Document;

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

    HAL& m_hal;

    std::weak_ptr<bus::II2C> m_i2c;

    std::shared_ptr<sz::PCA9685::Init_Params> m_init_params;
    std::shared_ptr<sz::PCA9685::Config> m_config;

    struct PWM_Channel
    {
        sz::PCA9685::PWM_Channel* config = nullptr;
        stream::IPWM_wptr stream;
        struct Last_Data
        {
            int pulse = -1;
        } last_data;
        q::Path stream_path;
    };

    std::vector<PWM_Channel> m_pwm_channels;

    void set_pwm_value(size_t idx, boost::optional<float> value);
};

}
}
