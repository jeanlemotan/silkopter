#pragma once

#include "common/node/processor/IProcessor.h"
#include "common/node/stream/IReference_Frame.h"
#include "common/node/stream/IAcceleration.h"
#include "common/node/stream/ILinear_Acceleration.h"
#include "HAL.h"

namespace silk
{
namespace node
{
namespace processor
{

class Inertial : public IProcessor
{
public:
    Inertial(HAL& hal);

    struct Init_Params
    {
        std::string name;
        stream::IReference_Frame* reference_frame_stream = nullptr;
        stream::IAcceleration* acceleration_stream = nullptr;
    };

    auto init(rapidjson::Value const& json) -> bool;
    auto init(Init_Params const& params) -> bool;

    auto get_input_stream_count() const -> size_t;
    auto get_input_stream(size_t idx) -> stream::IStream&;

    auto get_output_stream_count() const -> size_t;
    auto get_output_stream(size_t idx) -> stream::ILinear_Acceleration&;

    auto get_name() const -> std::string const&;

    void process();

private:
    auto init() -> bool;

    HAL& m_hal;
    Init_Params m_params;

    q::Clock::duration m_dt = q::Clock::duration(0);

    std::vector<stream::IReference_Frame::Sample> m_reference_frame_samples;
    std::vector<stream::IAcceleration::Sample> m_acceleration_samples;

    struct Stream : public stream::ILinear_Acceleration
    {
        auto get_samples() const -> std::vector<Sample> const& { return samples; }
        auto get_rate() const -> uint32_t { return params->reference_frame_stream->get_rate(); }
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
