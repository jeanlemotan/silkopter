#pragma once

#include "common/node/IProcessor.h"
#include "common/stream/IVoltage.h"
#include "common/stream/ICurrent.h"
#include "common/stream/IBattery_State.h"

#include "HAL.h"
#include "utils/Butterworth.h"

#include "Sample_Accumulator.h"
#include "Basic_Output_Stream.h"


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
    auto get_init_params() const -> rapidjson::Document;

    auto set_config(rapidjson::Value const& json) -> bool;
    auto get_config() const -> rapidjson::Document;

    auto send_message(rapidjson::Value const& json) -> rapidjson::Document;

    void set_input_stream_path(size_t idx, q::Path const& path);
    auto get_inputs() const -> std::vector<Input>;
    auto get_outputs() const -> std::vector<Output>;

    auto get_cell_count() const -> boost::optional<uint8_t>;

    void process();

private:
    auto init() -> bool;

    HAL& m_hal;

    std::shared_ptr<sz::LiPo_Battery::Init_Params> m_init_params;
    std::shared_ptr<sz::LiPo_Battery::Config> m_config;

    Sample_Accumulator<stream::IVoltage, stream::ICurrent> m_accumulator;

    auto compute_cell_count() -> boost::optional<uint8_t>;
    boost::optional<uint8_t> m_cell_count;

    util::Butterworth<stream::IVoltage::Value> m_voltage_filter;
    util::Butterworth<stream::ICurrent::Value> m_current_filter;
//    typename Stream_t::FILTER_CHANNEL_TYPE* m_channels[Stream_t::FILTER_CHANNELS] = { nullptr };

    typedef Basic_Output_Stream<stream::IBattery_State> Output_Stream;
    mutable std::shared_ptr<Output_Stream> m_output_stream;

};


}
}
