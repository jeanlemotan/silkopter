#pragma once

#include "HAL.h"
#include "common/node/stream/IADC_Value.h"
#include "common/node/stream/ICurrent.h"
#include "common/node/processor/ITransform.h"

namespace silk
{
namespace node
{
namespace processor
{

class ADC_Ammeter : public ITransform<stream::IADC_Value, stream::ICurrent>
{
public:
    ADC_Ammeter(HAL& hal);

    struct Init_Params
    {
        std::string name;
        stream::IADC_Value* input_stream = nullptr;
    };

    auto init(rapidjson::Value const& json) -> bool;
    auto init(Init_Params const& params) -> bool;

    auto set_config(rapidjson::Value const& json) -> bool;
    auto get_config() -> boost::optional<rapidjson::Value const&>;

    auto get_input_stream_count() const -> size_t;
    auto get_input_stream(size_t idx) -> stream::IADC_Value&;
    auto get_output_stream_count() const -> size_t;
    auto get_output_stream(size_t idx) -> stream::ICurrent&;
    auto get_name() const -> std::string const&;

    void process();

private:
    auto init() -> bool;

    HAL& m_hal;
    Init_Params m_params;

    struct Stream : public stream::ICurrent
    {
        auto get_samples() const -> std::vector<Sample> const& { return samples; }
        auto get_rate() const -> uint32_t { return params->input_stream->get_rate(); }
        auto get_name() const -> std::string const& { return name; }

        Init_Params* params = nullptr;
        std::vector<Sample> samples;
        uint32_t sample_idx = 0;
        std::string name;
    } m_stream;
};


}
}
}
