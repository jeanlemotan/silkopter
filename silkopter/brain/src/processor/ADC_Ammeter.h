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
    typedef ITransform<stream::IADC_Value, stream::ICurrent> Base;

    ADC_Ammeter(HAL& hal);

    struct Init_Params
    {
        std::string name;
        stream::IADC_Value* source_stream = nullptr;
    };

    auto init(rapidjson::Value const& json) -> bool;
    auto init(Init_Params const& params) -> bool;

    auto get_input_stream() -> stream::IADC_Value&;
    auto get_output_stream() -> stream::ICurrent&;
    auto get_name() const -> std::string const&;

    void process();

private:
    auto init() -> bool;

    HAL& m_hal;
    Init_Params m_params;

    struct Stream : public stream::ICurrent
    {
        auto get_samples() const -> std::vector<Sample> const& { return samples; }
        auto get_rate() const -> uint32_t { return source_stream->get_rate(); }
        auto get_name() const -> std::string const& { return name; }

        stream::IADC_Value* source_stream = nullptr;
        std::vector<Sample> samples;
        uint32_t sample_idx = 0;
        std::string name;
    } m_stream;
};


}
}
}
