#pragma once

#include "UAV.h"
#include "common/stream/IThrottle.h"
#include "common/stream/IPWM.h"
#include "common/node/IProcessor.h"

#include "Sample_Accumulator.h"
#include "Basic_Output_Stream.h"

namespace sz
{
namespace Throttle_To_PWM
{
struct Init_Params;
struct Config;
}
}

namespace silk
{
namespace node
{

class Throttle_To_PWM : public IProcessor
{
public:
    Throttle_To_PWM(UAV& uav);

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

    std::shared_ptr<sz::Throttle_To_PWM::Init_Params> m_init_params;
    std::shared_ptr<sz::Throttle_To_PWM::Config> m_config;

    std::vector<Sample_Accumulator<stream::IThrottle>> m_accumulators;

    typedef Basic_Output_Stream<stream::IPWM> Output_Stream;
    mutable std::vector<std::shared_ptr<Output_Stream>> m_output_streams;
};


}
}

