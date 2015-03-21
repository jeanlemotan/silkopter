#pragma once

#include "common/node/processor/IProcessor.h"
#include "common/node/stream/IVoltage_Stream.h"
#include "common/node/stream/ICurrent_Stream.h"
#include "common/node/stream/IBattery_State_Stream.h"

#include "HAL.h"
#include "utils/Butterworth.h"

namespace sz
{
namespace LiPo_Battery
{
struct Init_Params;
struct Config;
}
}


namespace silk
{
namespace node
{

class LiPo_Battery : public IProcessor
{
public:
    LiPo_Battery(HAL& hal);

    auto init(rapidjson::Value const& init_params) -> bool;
    auto get_init_params() const -> rapidjson::Document const&;

    auto set_config(rapidjson::Value const& json) -> bool;
    auto get_config() const -> rapidjson::Document;

    auto get_inputs() const -> std::vector<Input>;
    auto get_outputs() const -> std::vector<Output>;

    auto get_cell_count() const -> boost::optional<uint8_t>;

    void process();

private:
    auto init() -> bool;

    HAL& m_hal;

    rapidjson::Document m_init_paramsj;
    std::shared_ptr<sz::LiPo_Battery::Init_Params> m_init_params;
    std::shared_ptr<sz::LiPo_Battery::Config> m_config;

    IVoltage_Stream_wptr m_voltage_stream;
    ICurrent_Stream_wptr m_current_stream;

    std::vector<IVoltage_Stream::Sample> m_voltage_samples;
    std::vector<ICurrent_Stream::Sample> m_current_samples;
    q::Clock::duration m_dt = q::Clock::duration(0);

    auto compute_cell_count() -> boost::optional<uint8_t>;
    boost::optional<uint8_t> m_cell_count;

    util::Butterworth<IVoltage_Stream::Value> m_voltage_filter;
    util::Butterworth<ICurrent_Stream::Value> m_current_filter;
//    typename Stream_t::FILTER_CHANNEL_TYPE* m_channels[Stream_t::FILTER_CHANNELS] = { nullptr };

    struct Stream : public IBattery_State_Stream
    {
        auto get_samples() const -> std::vector<Sample> const& { return samples; }
        auto get_rate() const -> uint32_t { return rate; }

        uint32_t rate = 0;
        Sample last_sample;
        std::vector<Sample> samples;
    };
    mutable std::shared_ptr<Stream> m_output_stream;
};


}
}
