#pragma once

#include "common/node/IProcessor.h"
#include "common/stream/IFrame.h"
#include "common/stream/IAcceleration.h"
#include "common/stream/ILinear_Acceleration.h"
#include "UAV.h"
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
    Gravity_Filter(UAV& uav);

    auto init(rapidjson::Value const& init_params) -> bool;
    auto get_init_params() const -> rapidjson::Document;

    auto set_config(rapidjson::Value const& json) -> bool;
    auto get_config() const -> rapidjson::Document;

    auto send_message(rapidjson::Value const& json) -> rapidjson::Document;

    auto start(q::Clock::time_point tp) -> bool override;

    void set_input_stream_path(size_t idx, q::Path const& path);
    auto get_inputs() const -> std::vector<Input>;
    auto get_outputs() const -> std::vector<Output>;

    void process();

private:
    auto init() -> bool;

    UAV& m_uav;

    std::shared_ptr<sz::Gravity_Filter::Init_Params> m_init_params;
    std::shared_ptr<sz::Gravity_Filter::Config> m_config;

    Sample_Accumulator<stream::IUAV_Frame, stream::IAcceleration> m_accumulator;

    typedef Basic_Output_Stream<stream::ILinear_Acceleration> Output_Stream;
    mutable std::shared_ptr<Output_Stream> m_output_stream;
};



}
}
