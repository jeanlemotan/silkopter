#pragma once

#include "common/node/IController.h"
#include "common/node/stream/IAngular_Velocity.h"
#include "common/node/stream/ITorque.h"
#include "HAL.h"

#include "utils/PID.h"
#include "Sample_Accumulator.h"

namespace sz
{
namespace Rate_Controller
{
struct Init_Params;
struct Config;
}
}



namespace silk
{
namespace node
{

class Rate_Controller : public IController
{
public:
    Rate_Controller(HAL& hal);

    auto init(rapidjson::Value const& init_params) -> bool;
    auto get_init_params() const -> rapidjson::Document const&;

    auto set_config(rapidjson::Value const& json) -> bool;
    auto get_config() const -> rapidjson::Document;

    auto send_message(rapidjson::Value const& json) -> rapidjson::Document;

    auto get_stream_inputs() const -> std::vector<Stream_Input>;
    auto get_stream_outputs() const -> std::vector<Stream_Output>;

    void process();

private:
    auto init() -> bool;

    math::vec3f compute_feedforward(config::Multi& config, stream::IAngular_Velocity::Value const& input, stream::IAngular_Velocity::Value const& target);
    math::vec3f compute_feedback(stream::IAngular_Velocity::Value const& input, stream::IAngular_Velocity::Value const& target);


    HAL& m_hal;

    rapidjson::Document m_init_paramsj;
    std::shared_ptr<sz::Rate_Controller::Init_Params> m_init_params;
    std::shared_ptr<sz::Rate_Controller::Config> m_config;

    Sample_Accumulator<stream::IAngular_Velocity, stream::IAngular_Velocity> m_accumulator;

    typedef util::PID<float, float, float> PID;
    PID m_x_pid;
    PID m_y_pid;
    PID m_z_pid;

    struct Stream : public stream::ITorque
    {
        auto get_samples() const -> std::vector<Sample> const& { return samples; }
        auto get_rate() const -> uint32_t { return rate; }

        Sample last_sample;
        std::vector<Sample> samples;
        uint32_t rate = 0;
    };
    mutable std::shared_ptr<Stream> m_output_stream;
};



}
}
