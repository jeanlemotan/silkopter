#pragma once

#include "HAL.h"
#include "common/node/ISource.h"
#include "common/node/stream/IADC.h"
#include "common/node/bus/II2C.h"


namespace sz
{
namespace ADS1115
{
struct Init_Params;
struct Config;
}
}



namespace silk
{
namespace node
{

class ADS1115 : public ISource
{
public:
    ADS1115(HAL& hal);

    auto init(rapidjson::Value const& init_params) -> bool;
    auto get_init_params() const -> rapidjson::Document const&;

    auto set_config(rapidjson::Value const& json) -> bool;
    auto get_config() const -> rapidjson::Document;

    auto send_message(rapidjson::Value const& json) -> rapidjson::Document;

    auto get_stream_outputs() const -> std::vector<Stream_Output>;

    void process();

private:
    auto init() -> bool;

    HAL& m_hal;
    std::weak_ptr<bus::II2C> m_i2c;

    rapidjson::Document m_init_paramsj;
    std::shared_ptr<sz::ADS1115::Init_Params> m_init_params;
    std::shared_ptr<sz::ADS1115::Config> m_config;

    q::Clock::time_point m_last_tp = q::Clock::now();
    q::Clock::duration m_dt;

    struct Stream : public stream::IADC
    {
        auto get_samples() const -> std::vector<Sample> const& { return samples; }
        auto get_rate() const -> uint32_t { return rate; }

        std::vector<Sample> samples;
        q::Clock::time_point last_tp = q::Clock::now();
        Sample last_sample;
        uint32_t rate = 0;
    };
    mutable std::shared_ptr<Stream> m_adc[4];
};


}
}
