#pragma once

#include "common/node/IProcessor.h"
#include "common/node/stream/ILinear_Acceleration.h"
#include "common/node/stream/IPosition.h"
#include "common/node/stream/IPressure.h"
#include "common/node/stream/IFrame.h"

#include "HAL.h"

#include "Sample_Accumulator.h"
#include "Basic_Output_Stream.h"


namespace sz
{
namespace Comp_ECEF_Position
{
struct Init_Params;
struct Config;
}
}


namespace silk
{
namespace node
{

class Comp_ECEF_Position : public IProcessor
{
public:
    Comp_ECEF_Position(HAL& hal);

    auto init(rapidjson::Value const& init_params) -> bool;
    auto get_init_params() const -> rapidjson::Document;

    auto set_config(rapidjson::Value const& json) -> bool;
    auto get_config() const -> rapidjson::Document;

    auto send_message(rapidjson::Value const& json) -> rapidjson::Document;

    void set_input_stream_path(size_t idx, q::Path const& path);
    auto get_inputs() const -> std::vector<Input>;
    auto get_outputs() const -> std::vector<Output>;

    void process();

private:
    auto init() -> bool;

    HAL& m_hal;

    std::shared_ptr<sz::Comp_ECEF_Position::Init_Params> m_init_params;
    std::shared_ptr<sz::Comp_ECEF_Position::Config> m_config;

    q::Clock::duration m_dt = q::Clock::duration(0);

    Sample_Accumulator<stream::IECEF_Position, stream::IENU_Linear_Acceleration, stream::IPressure> m_accumulator;

    typedef Basic_Output_Stream<stream::IECEF_Position> Output_Stream;
    mutable std::shared_ptr<Output_Stream> m_output_stream;

    util::coordinates::ECEF m_velocity;

//    struct ENU_Frame_Stream : public stream::IENU_Frame
//    {
//        auto get_samples() const -> std::vector<Sample> const& { return samples; }
//        auto get_rate() const -> uint32_t { return rate; }

//        Sample last_sample;
//        std::vector<Sample> samples;
//        uint32_t rate = 0;
//    };
//    mutable std::shared_ptr<ENU_Frame_Stream> m_enu_frame_output_stream;
};



}
}

