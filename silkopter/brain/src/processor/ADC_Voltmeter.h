#pragma once

#include "HAL.h"
#include "common/node/stream/IADC.h"
#include "common/node/stream/IVoltage.h"
#include "common/node/IProcessor.h"

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
    auto get_init_params() const -> rapidjson::Document const&;

    auto set_config(rapidjson::Value const& json) -> bool;
    auto get_config() const -> rapidjson::Document;

    auto send_message(rapidjson::Value const& json) -> rapidjson::Document;

    auto get_stream_inputs() const -> std::vector<Stream_Input>;
    auto get_stream_outputs() const -> std::vector<Stream_Output>;

    void process();

private:
    auto init() -> bool;

    HAL& m_hal;

    rapidjson::Document m_init_paramsj;
    std::shared_ptr<sz::ADC_Voltmeter::Init_Params> m_init_params;
    std::shared_ptr<sz::ADC_Voltmeter::Config> m_config;

    stream::IADC_wptr m_adc_stream;

    typedef Basic_Output_Stream<stream::IVoltage> Output_Stream;
    mutable std::shared_ptr<Output_Stream> m_output_stream;
};


}
}
