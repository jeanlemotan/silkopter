#pragma once

#include "common/node/IProcessor.h"
#include "common/stream/ITorque.h"
#include "common/stream/IFloat.h"
#include "common/stream/IThrottle.h"

#include "HAL.h"

#include "Sample_Accumulator.h"

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
    auto get_init_params() const -> rapidjson::Document;

    auto set_config(rapidjson::Value const& json) -> bool;
    auto get_config() const -> rapidjson::Document;

    auto send_message(rapidjson::Value const& json) -> rapidjson::Document;

    void set_input_stream_path(size_t idx, q::Path const& path);
    auto get_inputs() const -> std::vector<Input>;
    auto get_outputs() const -> std::vector<Output>;

    auto get_cell_count() const -> boost::optional<uint8_t>;

    void process();

private:
    auto init() -> bool;

    void compute_throttles(config::Multi const& config, stream::IFloat::Value const& collective_thrust, stream::ITorque::Value const& torque);


    HAL& m_hal;

    std::shared_ptr<sz::Motor_Mixer::Init_Params> m_init_params;
    std::shared_ptr<sz::Motor_Mixer::Config> m_config;

    Sample_Accumulator<stream::ITorque, stream::IFloat> m_accumulator;

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
