#pragma once

#include "common/node/processor/IProcessor.h"
#include "common/node/stream/IAngular_Velocity.h"
#include "common/node/stream/IAcceleration.h"
#include "common/node/stream/IMagnetic_Field.h"
#include "common/node/stream/IReference_Frame.h"
#include "HAL.h"


namespace sz
{
namespace Comp_AHRS
{
class Init_Params;
class Config;
}
}



namespace silk
{
namespace node
{
namespace processor
{

class Comp_AHRS : public IProcessor
{
public:
    Comp_AHRS(HAL& hal);

    auto init(rapidjson::Value const& json) -> bool;
    auto get_init_params() -> boost::optional<rapidjson::Value const&>;

    auto set_config(rapidjson::Value const& json) -> bool;
    auto get_config() -> boost::optional<rapidjson::Value const&>;

    auto get_input_stream_count() const -> size_t;
    auto get_input_stream(size_t idx) -> stream::IStream&;

    auto get_output_stream_count() const -> size_t;
    auto get_output_stream(size_t idx) -> stream::IReference_Frame&;

    auto get_name() const -> std::string const&;

    void process();

private:
    auto init() -> bool;

    HAL& m_hal;

    std::shared_ptr<sz::Comp_AHRS::Init_Params> m_init_params;
    rapidjson::Document m_init_params_json;

    std::shared_ptr<sz::Comp_AHRS::Config> m_config;
    rapidjson::Document m_config_json;

    q::Clock::duration m_dt = q::Clock::duration(0);

    stream::IAngular_Velocity* m_angular_velocity_stream = nullptr;
    stream::IAcceleration* m_acceleration_stream = nullptr;
    stream::IMagnetic_Field* m_magnetic_field_stream = nullptr;

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
        auto get_name() const -> std::string const& { return name; }

        std::string name;
        Sample last_sample;
        std::vector<Sample> samples;
        uint32_t rate = 0;
    } m_stream;
};



}
}
}
