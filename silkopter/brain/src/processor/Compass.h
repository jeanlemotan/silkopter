#pragma once

#include "common/node/processor/IProcessor.h"
#include "common/node/stream/IReference_Frame.h"
#include "common/node/stream/IMagnetic_Field.h"
#include "common/node/stream/ICardinal_Points.h"
#include "HAL.h"

namespace silk
{
namespace node
{
namespace processor
{

class Compass : public IProcessor
{
public:
    Compass(HAL& hal);

    struct Init_Params
    {
        std::string name;
        stream::IReference_Frame* reference_frame_stream = nullptr;
        stream::IMagnetic_Field* magnetic_field_stream = nullptr;
    };

    auto get_init_params() -> rapidjson::Document;

    auto set_config(rapidjson::Value const& json) -> bool;
    auto get_config() -> rapidjson::Document;

    auto get_input_stream_count() const -> size_t;
    auto get_input_stream(size_t idx) -> stream::IStream&;

    auto get_output_stream_count() const -> size_t;
    auto get_output_stream(size_t idx) -> stream::ICardinal_Points&;

    auto get_name() const -> std::string const&;

    void process();

private:
    auto init() -> bool;

    HAL& m_hal;
    Init_Params m_params;

    q::Clock::duration m_dt = q::Clock::duration(0);

    std::vector<stream::IReference_Frame::Sample> m_reference_frame_samples;
    std::vector<stream::IMagnetic_Field::Sample> m_magnetic_field_samples;

    struct Stream : public stream::ICardinal_Points
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
