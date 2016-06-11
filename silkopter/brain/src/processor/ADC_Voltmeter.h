#pragma once

#include "UAV.h"
#include "common/stream/IADC.h"
#include "common/stream/IVoltage.h"
#include "common/node/IProcessor.h"

#include "Sample_Accumulator.h"
#include "Basic_Output_Stream.h"


namespace silk
{
namespace uav
{
struct ADC_Voltmeter_Descriptor;
struct ADC_Voltmeter_Config;
}
}

namespace silk
{
namespace node
{

class ADC_Voltmeter : public IProcessor
{
public:
    ADC_Voltmeter(UAV& uav);

    bool init(std::shared_ptr<uav::INode_Descriptor> descriptor) override;
    std::shared_ptr<uav::INode_Descriptor> get_descriptor() const override;

    bool set_config(std::shared_ptr<uav::INode_Config> config) override;
    std::shared_ptr<uav::INode_Config> get_config() const override;

    //auto send_message(rapidjson::Value const& json) -> rapidjson::Document;

    auto start(q::Clock::time_point tp) -> bool override;

    void set_input_stream_path(size_t idx, q::Path const& path);
    auto get_inputs() const -> std::vector<Input>;
    auto get_outputs() const -> std::vector<Output>;

    void process();

private:
    auto init() -> bool;

    UAV& m_uav;

    std::shared_ptr<uav::ADC_Voltmeter_Descriptor> m_descriptor;
    std::shared_ptr<uav::ADC_Voltmeter_Config> m_config;

    Sample_Accumulator<stream::IADC> m_accumulator;

    typedef Basic_Output_Stream<stream::IVoltage> Output_Stream;
    mutable std::shared_ptr<Output_Stream> m_output_stream;
};


}
}
