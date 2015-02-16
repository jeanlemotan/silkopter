#pragma once

#include "HAL.h"
#include "common/node/processor/IBattery.h"

namespace silk
{
namespace node
{
namespace processor
{

class Lipo_Battery : public IBattery
{
public:
    Lipo_Battery(HAL& hal);

    struct Init_Params
    {
        std::string name;
        stream::IVoltage* voltage_stream = nullptr;
        stream::ICurrent* current_stream = nullptr;
        float capacity_ah = 0;
    };

    auto init(rapidjson::Value const& json) -> bool;
    auto init(Init_Params const& params) -> bool;

    auto get_input_stream_count() const -> size_t;
    auto get_input_stream(size_t idx) -> stream::IStream&;

    auto get_output_stream_count() const -> size_t;
    auto get_output_stream(size_t idx) -> stream::IStream&;

    auto get_input_voltage_stream() -> stream::IVoltage&;
    auto get_input_current_stream() -> stream::ICurrent&;
    auto get_output_battery_state_stream() -> stream::IBattery_State&;

    auto get_name() const -> std::string const&;

    void process();

private:
    auto init() -> bool;

    HAL& m_hal;
    Init_Params m_params;

    struct Stream : public stream::IBattery_State
    {
        auto get_samples() const -> std::vector<Sample> const& { return samples; }
        auto get_rate() const -> uint32_t { return params->current_stream->get_rate(); }
        auto get_name() const -> std::string const& { return name; }

        Init_Params const* params = nullptr;
        std::vector<Sample> samples;
        uint32_t sample_idx = 0;
        std::string name;
    } m_stream;
};


}
}
}
