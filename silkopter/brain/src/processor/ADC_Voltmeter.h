#pragma once

#include "HAL.h"
#include "common/node/stream/IADC_Value.h"
#include "common/node/stream/IVoltage.h"
#include "common/node/processor/ITransform.h"


namespace sz
{
namespace ADC_Voltmeter
{
class Init_Params;
class Config;
}
}

namespace silk
{
namespace node
{
namespace processor
{

class ADC_Voltmeter : public ITransform<stream::IADC_Value, stream::IVoltage>
{
public:
    ADC_Voltmeter(HAL& hal);

    struct Init_Params
    {
        std::string name;
        stream::IADC_Value* input_stream = nullptr;
    };

    auto init(rapidjson::Value const& json) -> bool;
    auto get_init_params() -> boost::optional<rapidjson::Value const&>;

    auto set_config(rapidjson::Value const& json) -> bool;
    auto get_config() -> boost::optional<rapidjson::Value const&>;

    auto get_input_stream_count() const -> size_t;
    auto get_input_stream(size_t idx) -> stream::IADC_Value&;
    auto get_output_stream_count() const -> size_t;
    auto get_output_stream(size_t idx) -> stream::IVoltage&;
    auto get_name() const -> std::string const&;

    void process();

private:
    auto init() -> bool;

    HAL& m_hal;

    std::unique_ptr<sz::ADC_Voltmeter::Init_Params> m_init_params;
    rapidjson::Document m_init_params_json;

    std::unique_ptr<sz::ADC_Voltmeter::Config> m_config;
    rapidjson::Document m_config_json;

    stream::IADC_Value* m_adc_stream = nullptr;

    struct Stream : public stream::IVoltage
    {
        auto get_samples() const -> std::vector<Sample> const& { return samples; }
        auto get_rate() const -> uint32_t { return rate; }
        auto get_name() const -> std::string const& { return name; }

        uint32_t rate = 0;
        std::vector<Sample> samples;
        uint32_t sample_idx = 0;
        std::string name;
    } m_stream;
};


}
}
}
