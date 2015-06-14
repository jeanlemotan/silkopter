#pragma once

#include "HAL.h"
#include "common/node/stream/IADC.h"
#include "common/node/stream/ICurrent.h"
#include "common/node/IProcessor.h"

#include "Sample_Accumulator.h"
#include "Basic_Output_Stream.h"

namespace sz
{
namespace ADC_Ammeter
{
struct Init_Params;
struct Config;
}
}

namespace silk
{
namespace node
{

class ADC_Ammeter : public IProcessor
{
public:
    ADC_Ammeter(HAL& hal);

    auto init(rapidjson::Value const& init_params) -> bool;
    auto get_init_params() const -> rapidjson::Document;

    auto set_config(rapidjson::Value const& json) -> bool;
    auto get_config() const -> rapidjson::Document;

    auto send_message(rapidjson::Value const& json) -> rapidjson::Document;

    void set_stream_input_path(size_t idx, q::Path const& path);
    auto get_stream_inputs() const -> std::vector<Stream_Input>;
    auto get_stream_outputs() const -> std::vector<Stream_Output>;

    void process();

private:
    auto init() -> bool;

    HAL& m_hal;

    std::shared_ptr<sz::ADC_Ammeter::Init_Params> m_init_params;
    std::shared_ptr<sz::ADC_Ammeter::Config> m_config;

    Sample_Accumulator<stream::IADC> m_accumulator;

    typedef Basic_Output_Stream<stream::ICurrent> Output_Stream;
    mutable std::shared_ptr<Output_Stream> m_output_stream;
};


}
}

