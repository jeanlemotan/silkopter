#pragma once

#include "HAL.h"
#include "common/node/source/IADC.h"
#include "common/node/source/IAmmeter.h"
#include "common/node/bus/II2C.h"

namespace silk
{
namespace node
{
namespace source
{

class RC5T619 : q::util::Noncopyable
{
public:
    RC5T619(HAL& hal);

    struct Init_Params
    {
        std::string name;
        bus::IBus* bus = nullptr;
        uint32_t adc0_rate = 50;
        uint32_t adc1_rate_ratio = 10;
    };

    auto init(rapidjson::Value const& json) -> bool;
    auto init(Init_Params const& params) -> bool;

    void process();

    auto get_adc0() -> source::IADC&;
    auto get_adc1() -> source::IADC&;

private:
    auto init() -> bool;

    HAL& m_hal;
    bus::II2C* m_i2c = nullptr;

    Init_Params m_params;

    q::Clock::time_point m_last_time_point = q::Clock::now();
    q::Clock::duration m_dt;

    size_t m_stage = 0;

    struct ADC : public source::IADC
    {
        struct Stream : public stream::IADC_Value
        {
            auto get_samples() const -> std::vector<Sample> const& { return samples; }
            auto get_rate() const -> uint32_t { return rate; }
            auto get_name() const -> std::string const& { return name; }

            std::vector<Sample> samples;
            q::Clock::time_point last_time_point = q::Clock::now();
            uint32_t sample_idx = 0;
            uint32_t rate = 0;
            std::string name;
        } stream;

        auto get_stream() -> stream::IADC_Value& { return stream; }
        auto get_name() const -> std::string const& { return name; }

        std::string name;
    } m_adc[2];
};


}
}
}
