#pragma once

#include "HAL.h"
#include "common/node/stream/IADC.h"
#include "common/node/stream/IVoltage.h"
#include "common/node/IProcessor.h"

#include "Sample_Accumulator.h"
#include "Basic_Output_Stream.h"

namespace sz
{
namespace ADC_Voltmeter
{
struct Init_Params;
struct Config;
}
}

namespace silk
{
namespace node
{

class ADC_Voltmeter : public IProcessor
{
public:
    ADC_Voltmeter(HAL& hal);

    auto init(rapidjson::Value const& init_params) -> bool;
    auto get_init_params() const -> rapidjson::Document;

    auto set_config(rapidjson::Value const& json) -> bool;
    auto get_config() const -> rapidjson::Document;

    auto send_message(rapidjson::Value const& json) -> rapidjson::Document;

    void set_input_stream_path(size_t idx, q::Path const& path);
    auto get_inputs() const -> std::vector<Input>;
    auto get_outputs() const -> std::vector<Output>;

    void process();

private:
    auto init() -> bool;

    HAL& m_hal;

    std::shared_ptr<sz::ADC_Voltmeter::Init_Params> m_init_params;
    std::shared_ptr<sz::ADC_Voltmeter::Config> m_config;

    Sample_Accumulator<stream::IADC> m_accumulator;

    typedef Basic_Output_Stream<stream::IVoltage> Output_Stream;
    mutable std::shared_ptr<Output_Stream> m_output_stream;
};


}
}
