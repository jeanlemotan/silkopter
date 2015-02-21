#pragma once

#include "common/node/processor/IProcessor.h"
#include "common/node/stream/IAngular_Velocity.h"
#include "common/node/stream/IAcceleration.h"
#include "common/node/stream/IMagnetic_Field.h"
#include "common/node/stream/IReference_Frame.h"
#include "HAL.h"

namespace silk
{
namespace node
{
namespace processor
{

class Multirotor_Pilot : public IProcessor
{
public:
    Multirotor_Pilot(HAL& hal);

    struct Init_Params
    {
        std::string name;
        stream::IAngular_Velocity* angular_velocity_stream = nullptr;
        stream::ILinear_Acceleration* linear_acceleration_stream = nullptr;
        stream::ICardinal_Points* cardinal_points_stream = nullptr;
        stream::ILocation* location_stream = nullptr;
    };

    auto init(rapidjson::Value const& json) -> bool;
    auto init(Init_Params const& params) -> bool;

    auto get_input_stream_count() const -> size_t;
    auto get_input_stream(size_t idx) -> stream::IStream&;

    auto get_output_stream_count() const -> size_t;
    auto get_output_stream(size_t idx) -> stream::IReference_Frame&;

    auto get_name() const -> std::string const&;

    void process();

private:
    auto init() -> bool;

    HAL& m_hal;
    Init_Params m_params;

    q::Clock::duration m_dt = q::Clock::duration(0);

    std::vector<stream::IAngular_Velocity::Sample> m_angular_velocity_samples;
    std::vector<stream::IAcceleration::Sample> m_acceleration_samples;
    std::vector<stream::IMagnetic_Field::Sample> m_magnetic_field_samples;

    math::vec3f m_noisy_front_w;
    math::vec3f m_noisy_right_w;
    math::vec3f m_noisy_up_w;

    struct Stream : public stream::IReference_Frame
    {
        auto get_samples() const -> std::vector<Sample> const& { return samples; }
        auto get_rate() const -> uint32_t { return params->angular_velocity_stream->get_rate(); }
        auto get_name() const -> std::string const& { return name; }

        std::string name;
        Init_Params* params = nullptr;
        Sample last_sample;
        std::vector<Sample> samples;
        uint32_t sample_idx = 0;
    } m_stream;
};



}
}
}
