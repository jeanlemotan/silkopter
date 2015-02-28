#pragma once

#include "HAL.h"
#include "common/node/source/ISource.h"
#include "common/node/stream/IDistance.h"
#include "common/node/bus/II2C.h"


namespace sz
{
namespace SRF02
{
class Init_Params;
class Config;
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

    auto init(rapidjson::Value const& json) -> bool;
    auto get_init_params() -> boost::optional<rapidjson::Value const&>;

    auto set_config(rapidjson::Value const& json) -> bool;
    auto get_config() -> boost::optional<rapidjson::Value const&>;

    auto get_name() const -> std::string const&;
    auto get_output_stream_count() const -> size_t;
    auto get_output_stream(size_t idx) -> stream::IStream&;

    void process();

private:
    auto init() -> bool;

    HAL& m_hal;

    bus::II2C* m_i2c = nullptr;

    std::shared_ptr<sz::SRF02::Init_Params> m_init_params;
    rapidjson::Document m_init_params_json;

    std::shared_ptr<sz::SRF02::Config> m_config;
    rapidjson::Document m_config_json;

    struct Stream : public stream::IDistance
    {
        auto get_samples() const -> std::vector<Sample> const& { return samples; }
        auto get_rate() const -> uint32_t { return rate; }
        auto get_name() const -> std::string const& { return name; }

        uint32_t rate = 0;
        std::vector<Sample> samples;
        Sample last_sample;
        q::Clock::duration dt;
        q::Clock::time_point last_time_point;
        std::string name;
    } m_stream;

    int m_state = 0;
};

}
}
}
