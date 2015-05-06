#pragma once

#include "common/node/IProcessor.h"
#include "common/node/stream/ITorque.h"
#include "common/node/stream/IForce.h"
#include "common/node/stream/IThrottle.h"

#include "HAL.h"

namespace sz
{
namespace Motor_Mixer
{
struct Init_Params;
struct Config;
}
}


namespace silk
{
namespace node
{

class Motor_Mixer : public IProcessor
{
public:
    Motor_Mixer(HAL& hal);

    auto init(rapidjson::Value const& init_params) -> bool;
    auto get_init_params() const -> rapidjson::Document const&;

    auto set_config(rapidjson::Value const& json) -> bool;
    auto get_config() const -> rapidjson::Document;

    auto send_message(rapidjson::Value const& json) -> rapidjson::Document;

    auto get_stream_inputs() const -> std::vector<Stream_Input>;
    auto get_stream_outputs() const -> std::vector<Stream_Output>;

    auto get_cell_count() const -> boost::optional<uint8_t>;

    void process();

private:
    auto init() -> bool;

    void compute_throttles(config::Multi const& config, stream::IForce::Value const& collective_thrust, stream::ITorque::Value const& torque);


    HAL& m_hal;

    rapidjson::Document m_init_paramsj;
    std::shared_ptr<sz::Motor_Mixer::Init_Params> m_init_params;
    std::shared_ptr<sz::Motor_Mixer::Config> m_config;

    stream::ITorque_wptr m_torque_stream;
    stream::IForce_wptr m_force_stream;

    std::vector<stream::ITorque::Sample> m_torque_samples;
    std::vector<stream::IForce::Sample> m_force_samples;

    struct Stream : public stream::IThrottle
    {
        auto get_samples() const -> std::vector<Sample> const& { return samples; }
        auto get_rate() const -> uint32_t { return rate; }

        uint32_t rate = 0;
        Sample last_sample;
        std::vector<Sample> samples;

        struct Config
        {
            math::vec3f position;
            math::vec3f max_torque;
            math::vec3f torque_vector;
        } config;

        float throttle = 0;
        float thrust = 0;
        math::vec3f torque;
    };

    mutable std::vector<std::shared_ptr<Stream>> m_outputs;
};


}
}
