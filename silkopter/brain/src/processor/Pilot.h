#pragma once

#include "common/node/processor/IPilot.h"
#include "common/node/stream/IAngular_Velocity.h"
#include "common/node/stream/ICardinal_Points.h"
#include "common/node/stream/IFrame.h"
#include "common/node/stream/IBattery_State.h"
#include "common/node/stream/ICommands.h"
#include "Comms.h"
#include "HAL.h"

namespace sz
{
namespace Pilot
{
struct Init_Params;
struct Config;
}
}

namespace silk
{
namespace node
{

class Pilot : public IPilot
{
public:
    Pilot(HAL& hal);

    auto init(rapidjson::Value const& init_params) -> bool;
    auto get_init_params() const -> rapidjson::Document const&;

    auto set_config(rapidjson::Value const& json) -> bool;
    auto get_config() const -> rapidjson::Document;

    auto get_inputs() const -> std::vector<Input>;
    auto get_outputs() const -> std::vector<Output>;

    void process();

private:
    auto init() -> bool;

    HAL& m_hal;

    rapidjson::Document m_init_paramsj;
    std::shared_ptr<sz::Pilot::Init_Params> m_init_params;
    std::shared_ptr<sz::Pilot::Config> m_config;

    q::Clock::duration m_dt = q::Clock::duration(0);

    stream::IAngular_Velocity_wptr m_angular_velocity_stream;
    stream::ICardinal_Points_wptr m_cardinal_points_stream;
    //stream::ILocation_wptr m_location_stream;
    stream::IBattery_State_wptr m_battery_state_stream;
    stream::ICommands_wptr m_commands_stream;

    std::vector<stream::IAngular_Velocity::Sample> m_angular_velocity_samples;
    std::vector<stream::ICardinal_Points::Sample> m_cardinal_points_samples;
    //std::vector<stream::ILocation::Sample> m_location_samples;
    std::vector<stream::IBattery_State::Sample> m_battery_state_samples;
    std::vector<stream::ICommands::Sample> m_commands_samples;

    struct Stream : public stream::IFrame
    {
        auto get_samples() const -> std::vector<Sample> const& { return samples; }
        auto get_rate() const -> uint32_t { return rate; }


        Sample last_sample;
        std::vector<Sample> samples;
        uint32_t rate = 0;
    };
    std::shared_ptr<Stream> m_output_stream;
};



}
}
