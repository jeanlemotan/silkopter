#pragma once

#include "UAV.h"
#include "common/stream/IThrottle.h"
#include "common/stream/IPWM.h"
#include "common/node/IProcessor.h"

#include "Sample_Accumulator.h"
#include "Basic_Output_Stream.h"


namespace silk
{
struct Throttle_To_PWM_Descriptor;
struct Throttle_To_PWM_Config;
}

namespace silk
{
namespace node
{

class Throttle_To_PWM : public IProcessor
{
public:
    Throttle_To_PWM(UAV& uav);

    bool init(std::shared_ptr<INode_Descriptor> descriptor) override;
    std::shared_ptr<INode_Descriptor> get_descriptor() const override;

    bool set_config(std::shared_ptr<INode_Config> config) override;
    std::shared_ptr<INode_Config> get_config() const override;

    //auto send_message(rapidjson::Value const& json) -> rapidjson::Document;

    auto start(q::Clock::time_point tp) -> bool override;

    void set_input_stream_path(size_t idx, q::Path const& path);
    auto get_inputs() const -> std::vector<Input>;
    auto get_outputs() const -> std::vector<Output>;

    void process();

private:
    auto init() -> bool;

    UAV& m_uav;

    std::shared_ptr<Throttle_To_PWM_Descriptor> m_descriptor;
    std::shared_ptr<Throttle_To_PWM_Config> m_config;

    std::vector<Sample_Accumulator<stream::IThrottle>> m_accumulators;

    typedef Basic_Output_Stream<stream::IPWM> Output_Stream;
    mutable std::vector<std::shared_ptr<Output_Stream>> m_output_streams;
};


}
}

