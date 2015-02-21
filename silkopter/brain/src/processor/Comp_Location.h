#pragma once

#include "common/node/processor/IProcessor.h"
#include "common/node/stream/ILinear_Acceleration.h"
#include "common/node/stream/ILocation.h"
#include "common/node/stream/IPressure.h"

#include "HAL.h"

namespace silk
{
namespace node
{
namespace processor
{

class Comp_Location : public IProcessor
{
public:
    Comp_Location(HAL& hal);

    struct Init_Params
    {
        std::string name;
        stream::ILocation* location_stream = nullptr;
        stream::ILinear_Acceleration* linear_acceleration_stream = nullptr;
        stream::IPressure* pressure_stream = nullptr;
    };

    auto init(rapidjson::Value const& json) -> bool;
    auto init(Init_Params const& params) -> bool;

    auto get_input_stream_count() const -> size_t;
    auto get_input_stream(size_t idx) -> stream::IStream&;
    auto get_output_stream_count() const -> size_t;
    auto get_output_stream(size_t idx) -> stream::ILocation&;

    auto get_name() const -> std::string const&;

    void process();

private:
    auto init() -> bool;

    HAL& m_hal;
    Init_Params m_params;

    q::Clock::duration m_dt = q::Clock::duration(0);

    std::vector<stream::ILocation::Sample> m_location_samples;
    std::vector<stream::ILinear_Acceleration::Sample> m_linear_acceleration_samples;
    std::vector<stream::IPressure::Sample> m_pressure_samples;

    struct Stream : public stream::ILocation
    {
        auto get_samples() const -> std::vector<Sample> const& { return samples; }
        auto get_rate() const -> uint32_t { return params->location_stream->get_rate(); }
        auto get_name() const -> std::string const& { return name; }

        std::string name;
        Init_Params* params = nullptr;
        Sample last_sample;
        std::vector<Sample> samples;
    } m_stream;
};



}
}
}
