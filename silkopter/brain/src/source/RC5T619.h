#pragma once

#include "HAL.h"
#include "common/node/source/ISource.h"
#include "common/node/stream/IADC_Value.h"
#include "common/node/bus/II2C.h"


namespace sz
{
namespace RC5T619
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

class RC5T619 : public ISource
{
public:
    RC5T619(HAL& hal);

    auto init(rapidjson::Value const& init_params, rapidjson::Value const& config) -> bool;
    auto get_init_params() -> rapidjson::Document;

    auto set_config(rapidjson::Value const& json) -> bool;
    auto get_config() -> rapidjson::Document;

    auto get_name() const -> std::string const&;
    auto get_outputs() const -> std::vector<Output>;

    void process();

private:
    auto init() -> bool;

    HAL& m_hal;
    bus::II2C* m_i2c = nullptr;

    std::shared_ptr<sz::RC5T619::Init_Params> m_init_params;
    std::shared_ptr<sz::RC5T619::Config> m_config;

    q::Clock::time_point m_last_time_point = q::Clock::now();
    q::Clock::duration m_dt;

    size_t m_stage = 0;

    struct ADC_Value : public stream::IADC_Value
    {
        auto get_samples() const -> std::vector<Sample> const& { return samples; }
        auto get_rate() const -> uint32_t { return rate; }
        auto get_name() const -> std::string const& { return name; }

        std::vector<Sample> samples;
        q::Clock::time_point last_time_point = q::Clock::now();
        Sample last_sample;
        uint32_t rate = 0;
        std::string name;
    };
    mutable ADC_Value m_adc[2];
};


}
}
}
