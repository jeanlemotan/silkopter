#pragma once

#include "common/node/processor/IProcessor.h"
#include "common/node/stream/ILinear_Acceleration.h"
#include "common/node/stream/ILocation.h"
#include "common/node/stream/IPressure.h"

#include "HAL.h"


namespace sz
{
namespace Comp_Location
{
struct Init_Params;
struct Config;
}
}


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

    auto init(rapidjson::Value const& init_params, rapidjson::Value const& config) -> bool;
    auto get_init_params() -> rapidjson::Document;

    auto set_config(rapidjson::Value const& json) -> bool;
    auto get_config() -> rapidjson::Document;

    auto get_inputs() const -> std::vector<Input>;
    auto get_outputs() const -> std::vector<Output>;

    auto get_name() const -> std::string const&;

    void process();

private:
    auto init() -> bool;

    HAL& m_hal;

    std::shared_ptr<sz::Comp_Location::Init_Params> m_init_params;
    std::shared_ptr<sz::Comp_Location::Config> m_config;

    q::Clock::duration m_dt = q::Clock::duration(0);

    stream::ILocation* m_location_stream = nullptr;
    stream::ILinear_Acceleration* m_linear_acceleration_stream = nullptr;
    stream::IPressure* m_pressure_stream = nullptr;

    std::vector<stream::ILocation::Sample> m_location_samples;
    std::vector<stream::ILinear_Acceleration::Sample> m_linear_acceleration_samples;
    std::vector<stream::IPressure::Sample> m_pressure_samples;

    struct Stream : public stream::ILocation
    {
        auto get_samples() const -> std::vector<Sample> const& { return samples; }
        auto get_rate() const -> uint32_t { return rate; }
        auto get_name() const -> std::string const& { return name; }

        std::string name;
        Sample last_sample;
        std::vector<Sample> samples;
        uint32_t rate = 0;
    };
    mutable Stream m_stream;
};



}
}
}
