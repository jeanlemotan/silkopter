#pragma once

#include "common/node/IProcessor.h"
#include "common/node/stream/IAngular_Velocity.h"
#include "common/node/stream/IAcceleration.h"
#include "common/node/stream/IMagnetic_Field.h"
#include "common/node/stream/IReference_Frame.h"
#include "HAL.h"


namespace sz
{
namespace Comp_AHRS
{
struct Init_Params;
struct Config;
}
}



namespace silk
{
namespace node
{

class Comp_AHRS : public IProcessor
{
public:
    Comp_AHRS(HAL& hal);

    auto init(rapidjson::Value const& init_params) -> bool;
    auto get_init_params() const -> rapidjson::Document;

    auto set_config(rapidjson::Value const& json) -> bool;
    auto get_config() const -> rapidjson::Document;

    auto get_inputs() const -> std::vector<Input>;
    auto get_outputs() const -> std::vector<Output>;

    void process();

private:
    auto init() -> bool;

    HAL& m_hal;

    std::shared_ptr<sz::Comp_AHRS::Init_Params> m_init_params;
    std::shared_ptr<sz::Comp_AHRS::Config> m_config;

    q::Clock::duration m_dt = q::Clock::duration(0);

    stream::IAngular_Velocity_wptr m_angular_velocity_stream;
    stream::IAcceleration_wptr m_acceleration_stream;
    stream::IMagnetic_Field_wptr m_magnetic_field_stream;

    std::vector<stream::IAngular_Velocity::Sample> m_angular_velocity_samples;
    std::vector<stream::IAcceleration::Sample> m_acceleration_samples;
    std::vector<stream::IMagnetic_Field::Sample> m_magnetic_field_samples;

    math::vec3f m_noisy_front_w;
    math::vec3f m_noisy_right_w;
    math::vec3f m_noisy_up_w;

    struct Stream : public stream::IReference_Frame
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
