#pragma once

#include "common/node/processor/IAHRS.h"
#include "HAL.h"

namespace silk
{
namespace node
{
namespace processor
{

class AHRS_Complimentary : public IAHRS
{
public:
    AHRS_Complimentary(HAL& hal);

    struct Init_Params
    {
        std::string name;
        stream::IAngular_Velocity* angular_velocity_stream = nullptr;
        stream::IAcceleration* acceleration_stream = nullptr;
        stream::IMagnetic_Field* magnetic_field_stream = nullptr;
    };

    auto init(rapidjson::Value const& json) -> bool;
    auto init(Init_Params const& params) -> bool;

    auto get_input_stream_count() const -> size_t;
    auto get_input_stream(size_t idx) -> stream::IStream&;
    auto get_input_angular_velocity_stream() -> stream::IAngular_Velocity&;
    auto get_input_acceleration_stream() -> stream::IAcceleration&;
    auto get_input_magnetic_field_stream() -> stream::IMagnetic_Field&;

    auto get_output_stream_count() const -> size_t;
    auto get_output_stream(size_t idx) -> stream::IStream&;
    auto get_output_reference_frame_stream() -> stream::IReference_Frame&;

    auto get_name() const -> std::string const&;

    void process();

private:
    auto init() -> bool;

    HAL& m_hal;
    Init_Params m_params;

    q::Clock::duration m_dt = q::Clock::duration(0);

    struct Stream : public stream::IReference_Frame
    {
        auto get_samples() const -> std::vector<Sample> const& { return samples; }
        auto get_rate() const -> uint32_t { return params->angular_velocity_stream->get_rate(); }
        auto get_name() const -> std::string const& { return name; }

        std::string name;
        Init_Params* params = nullptr;
        std::vector<Sample> samples;
        uint32_t sample_idx = 0;
    } m_stream;
};



}
}
}
