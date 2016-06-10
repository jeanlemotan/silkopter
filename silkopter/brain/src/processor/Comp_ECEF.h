#pragma once
/*
#include "common/node/IProcessor.h"
#include "common/stream/ILinear_Acceleration.h"
#include "common/stream/IPosition.h"
#include "common/stream/IVelocity.h"
#include "common/stream/IPressure.h"
#include "common/stream/IFrame.h"

#include "UAV.h"

#include "Sample_Accumulator.h"
#include "Basic_Output_Stream.h"


namespace silk
{
struct Comp_ECEF_Descriptor;
struct Comp_ECEF_Config;
}


namespace silk
{
namespace node
{

class Comp_ECEF : public IProcessor
{
public:
    Comp_ECEF(UAV& uav);

    bool init(std::shared_ptr<INode_Descriptor> descriptor) override;
    std::shared_ptr<INode_Descriptor> get_descriptor() const override;

    bool set_config(std::shared_ptr<INode_Config> config) override;
    std::shared_ptr<INode_Config> get_config() const override;

    auto send_message(rapidjson::Value const& json) -> rapidjson::Document;

    auto start(q::Clock::time_point tp) -> bool override;

    void set_input_stream_path(size_t idx, q::Path const& path);
    auto get_inputs() const -> std::vector<Input>;
    auto get_outputs() const -> std::vector<Output>;

    void process();

private:
    auto init() -> bool;

    UAV& m_uav;

    std::shared_ptr<Comp_ECEF_Descriptor> m_descriptor;
    std::shared_ptr<Comp_ECEF_Config> m_config;

    q::Clock::duration m_dt = q::Clock::duration(0);

    Sample_Accumulator<stream::IECEF_Position,
                        stream::IECEF_Velocity,
                        stream::IENU_Linear_Acceleration,
                        stream::IPressure> m_accumulator;

    typedef Basic_Output_Stream<stream::IECEF_Position> Position_Output_Stream;
    mutable std::shared_ptr<Position_Output_Stream> m_position_output_stream;

    typedef Basic_Output_Stream<stream::IECEF_Velocity> Velocity_Output_Stream;
    mutable std::shared_ptr<Velocity_Output_Stream> m_velocity_output_stream;

    util::coordinates::ECEF m_last_gps_position;
    math::vec3f m_velocity;

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

*/
