#pragma once

#include "common/node/processor/IAHRS.h"

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
        stream::IAngular_Velocity* angular_velocity_stream = nullptr;
        stream::IAcceleration* acceleration_stream = nullptr;
        stream::IMagnetic_Field* magnetic_field_stream = nullptr;
    };

    auto init(rapidjson::Value& json) -> bool;
    auto init(Init_Params const& params) -> bool;

    auto get_input_stream_count() const -> size_t;
    auto get_input_stream(size_t idx) -> stream::IStream&;
    auto get_input_angular_velocity_stream(size_t idx) -> stream::IAngular_Velocity&;
    auto get_input_acceleration_stream(size_t idx) -> stream::IAcceleration&;
    auto get_input_magnetic_field_stream(size_t idx) -> stream::IMagnetic_Field&;

    auto get_output_stream_count() const -> size_t;
    auto get_output_stream(size_t idx) -> stream::IReference_Frame&;
    auto get_name() const -> std::string const&;

    void process();

private:
    auto init() -> bool;

    HAL& m_hal;
    Init_Params m_params;

    struct Stream : public stream::IReference_Frame
    {
        auto get_samples() const -> std::vector<Sample> const& { return samples; }
        auto get_rate() const -> uint32_t { return rate; }
        auto get_name() const -> std::string const& { return name; }

        uint32_t rate = 0;
        std::vector<Sample> samples;
        uint32_t sample_idx = 0;
        std::string name;
    } m_stream;
};



}
}
}
