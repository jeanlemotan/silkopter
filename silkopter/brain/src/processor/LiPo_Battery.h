#pragma once

#include "common/node/processor/IProcessor.h"
#include "common/node/stream/IVoltage.h"
#include "common/node/stream/ICurrent.h"
#include "common/node/stream/IBattery_State.h"

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

    stream::IVoltage_wptr m_voltage_stream;
    stream::ICurrent_wptr m_current_stream;

    std::vector<stream::IVoltage::Sample> m_voltage_samples;
    std::vector<stream::ICurrent::Sample> m_current_samples;
    q::Clock::duration m_dt = q::Clock::duration(0);

    auto compute_cell_count() -> boost::optional<uint8_t>;
    boost::optional<uint8_t> m_cell_count;

    util::Butterworth<stream::IVoltage::Value> m_voltage_filter;
    util::Butterworth<stream::ICurrent::Value> m_current_filter;
//    typename Stream_t::FILTER_CHANNEL_TYPE* m_channels[Stream_t::FILTER_CHANNELS] = { nullptr };

    struct Stream : public stream::IBattery_State
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
