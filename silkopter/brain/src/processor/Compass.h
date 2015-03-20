#pragma once

#include "common/node/IProcessor.h"
#include "common/node/stream/IFrame.h"
#include "common/node/stream/IMagnetic_Field.h"
#include "common/node/stream/ICardinal_Points.h"
#include "HAL.h"

namespace silk
{
namespace node
{

class Compass : public IProcessor
{
public:
    Compass(HAL& hal);

    struct Init_Params
    {
        std::string name;
        stream::IFrame* frame_stream = nullptr;
        stream::IMagnetic_Field* magnetic_field_stream = nullptr;
    };

    auto get_init_params() const -> rapidjson::Document const&;

    auto set_config(rapidjson::Value const& json) -> bool;
    auto get_config() const -> rapidjson::Document;

    auto get_inputs() const -> std::vector<Input>;
    auto get_outputs() const -> std::vector<Output>;

    void process();

private:
    auto init() -> bool;

    HAL& m_hal;
    Init_Params m_params;

    q::Clock::duration m_dt = q::Clock::duration(0);

    std::vector<stream::IFrame::Sample> m_frame_samples;
    std::vector<stream::IMagnetic_Field::Sample> m_magnetic_field_samples;

    struct Stream : public stream::ICardinal_Points
    {
        auto get_samples() const -> std::vector<Sample> const& { return samples; }
        auto get_rate() const -> uint32_t { return params->frame_stream->get_rate(); }

        Init_Params* params = nullptr;
        Sample last_sample;
        std::vector<Sample> samples;
    } m_stream;
};



}
}
