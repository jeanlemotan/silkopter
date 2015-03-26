#pragma once

#include "common/node/processor/IController.h"
#include "common/node/stream/IAngular_Velocity.h"
#include "common/node/stream/ITorque.h"
#include "HAL.h"

#include "utils/PID.h"

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

    auto get_inputs() const -> std::vector<Input>;
    auto get_outputs() const -> std::vector<Output>;

    void process();

private:
    auto init() -> bool;

    HAL& m_hal;

    rapidjson::Document m_init_paramsj;
    std::shared_ptr<sz::Rate_Controller::Init_Params> m_init_params;
    std::shared_ptr<sz::Rate_Controller::Config> m_config;

    q::Clock::duration m_dt = q::Clock::duration(0);

    stream::IAngular_Velocity_wptr m_input_stream;
    stream::IAngular_Velocity_wptr m_target_stream;

    std::vector<stream::IAngular_Velocity::Sample> m_input_samples;
    std::vector<stream::IAngular_Velocity::Sample> m_target_samples;

    typedef util::PID<float, math::vec3f, math::vec3f> PID;
    PID m_pid;

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
