#pragma once

#include "common/node/IProcessor.h"
#include "common/node/stream/IFrame.h"
#include "common/node/stream/IAcceleration.h"
#include "common/node/stream/ILinear_Acceleration.h"
#include "HAL.h"
#include "Sample_Accumulator.h"
#include "Basic_Output_Stream.h"

namespace sz
{
namespace Gravity_Filter
{
struct Init_Params;
struct Config;
}
}


namespace silk
{
namespace node
{

class Gravity_Filter : public IProcessor
{
public:
    Gravity_Filter(HAL& hal);

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

    std::shared_ptr<sz::Gravity_Filter::Init_Params> m_init_params;
    std::shared_ptr<sz::Gravity_Filter::Config> m_config;

    Sample_Accumulator<stream::IFrame, stream::IAcceleration> m_accumulator;

    typedef Basic_Output_Stream<stream::ILinear_Acceleration> Output_Stream;
    mutable std::shared_ptr<Output_Stream> m_output_stream;
};



}
}
