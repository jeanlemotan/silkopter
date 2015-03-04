#pragma once

#include "HAL.h"
#include "common/node/source/ISource.h"
#include "common/node/stream/IDistance.h"
#include "common/node/bus/II2C.h"


namespace sz
{
namespace SRF02
{
struct Init_Params;
struct Config;
}
}


namespace silk
{
namespace node
{
namespace source
{

class SRF02 : public ISource
{
public:
    SRF02(HAL& hal);

    auto init(rapidjson::Value const& init_params) -> bool;
    auto get_init_params() -> rapidjson::Document;

    auto set_config(rapidjson::Value const& json) -> bool;
    auto get_config() -> rapidjson::Document;

    auto get_outputs() const -> std::vector<Output>;

    void process();

private:
    auto init() -> bool;

    HAL& m_hal;

    std::weak_ptr<bus::II2C> m_i2c;

    std::shared_ptr<sz::SRF02::Init_Params> m_init_params;
    std::shared_ptr<sz::SRF02::Config> m_config;

    struct Stream : public stream::IDistance
    {
        auto get_samples() const -> std::vector<Sample> const& { return samples; }
        auto get_rate() const -> uint32_t { return rate; }

        uint32_t rate = 0;
        std::vector<Sample> samples;
        Sample last_sample;
        q::Clock::duration dt;
        q::Clock::time_point last_time_point;
    };
    mutable std::shared_ptr<Stream> m_stream;

    int m_state = 0;
};

}
}
}
