#pragma once

#include "HAL.h"
#include "common/node/processor/IBattery.h"
#include "DspFilters/Butterworth.h"


namespace silk
{
namespace node
{
namespace processor
{

class LiPo_Battery : public IBattery
{
public:
    LiPo_Battery(HAL& hal);

    struct Init_Params
    {
        std::string name;
        stream::IVoltage* voltage_stream = nullptr;
        stream::ICurrent* current_stream = nullptr;
        float full_charge = 0; //When full, Ah
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

    auto get_cell_count() const -> boost::optional<uint8_t>;

    auto get_name() const -> std::string const&;

    void process();

private:
    auto init() -> bool;

    HAL& m_hal;
    Init_Params m_params;

    std::vector<stream::IVoltage::Sample> m_input_voltage_samples;
    std::vector<stream::ICurrent::Sample> m_input_current_samples;
    q::Clock::duration m_dt = q::Clock::duration(0);

    auto compute_cell_count() -> boost::optional<uint8_t>;
    boost::optional<uint8_t> m_cell_count;

    Dsp::SimpleFilter <Dsp::Butterworth::LowPass<1>, stream::IVoltage::FILTER_CHANNELS> m_voltage_filter;
    Dsp::SimpleFilter <Dsp::Butterworth::LowPass<1>, stream::ICurrent::FILTER_CHANNELS> m_current_filter;
//    typename Stream_t::FILTER_CHANNEL_TYPE* m_channels[Stream_t::FILTER_CHANNELS] = { nullptr };

    struct Stream : public stream::IBattery_State
    {
        auto get_samples() const -> std::vector<Sample> const& { return samples; }
        auto get_rate() const -> uint32_t { return params->current_stream->get_rate(); }
        auto get_name() const -> std::string const& { return name; }

        Init_Params const* params = nullptr;
        Sample last_sample;
        std::vector<Sample> samples;
        std::string name;
    } m_stream;
};


}
}
}
