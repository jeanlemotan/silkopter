#pragma once

#include "common/node/processor/IMultirotor.h"
#include "common/node/stream/IAngular_Velocity.h"
#include "common/node/stream/ILinear_Acceleration.h"
#include "common/node/stream/ICardinal_Points.h"
#include "common/node/stream/IReference_Frame.h"
#include "common/node/stream/ILocation.h"
#include "common/node/stream/IBattery_State.h"
#include "Comms.h"
#include "HAL.h"

namespace silk
{
namespace node
{
namespace processor
{

class Multirotor : public IMultirotor
{
public:
    Multirotor(HAL& hal, Comms& comms);

    struct Init_Params
    {
        std::string name;
        stream::IReference_Frame* reference_frame_stream = nullptr;
        stream::IAngular_Velocity* angular_velocity_stream = nullptr;
        stream::ILinear_Acceleration* linear_acceleration_stream = nullptr;
        stream::ICardinal_Points* cardinal_points_stream = nullptr;
        stream::ILocation* location_stream = nullptr;
        stream::IBattery_State* battery_state_stream = nullptr;
    };

    auto get_init_params() -> rapidjson::Document;

    auto set_config(rapidjson::Value const& json) -> bool;
    auto get_config() -> rapidjson::Document;

    auto get_input_stream_count() const -> size_t;
    auto get_input_stream(size_t idx) -> stream::IStream&;

    auto get_output_stream_count() const -> size_t;
    auto get_output_stream(size_t idx) -> stream::IReference_Frame&;

    auto get_name() const -> std::string const&;

    void process();

private:
    auto init() -> bool;

    HAL& m_hal;
    Comms& m_comms;

    Init_Params m_params;

    q::Clock::duration m_dt = q::Clock::duration(0);

    std::vector<stream::IReference_Frame::Sample> m_reference_frame_samples;
    std::vector<stream::IAngular_Velocity::Sample> m_angular_velocity_samples;
    std::vector<stream::ILinear_Acceleration::Sample> m_acceleration_samples;
    std::vector<stream::ICardinal_Points::Sample> m_cardinal_points_samples;
    std::vector<stream::ILocation::Sample> m_location_samples;
    std::vector<stream::IBattery_State::Sample> m_battery_state_samples;

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
