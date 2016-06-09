#pragma once

#include "UAV.h"
#include "common/stream/IADC.h"
#include "common/stream/ICurrent.h"
#include "common/node/IProcessor.h"

#include "Sample_Accumulator.h"
#include "Basic_Output_Stream.h"


namespace silk
{
struct ADC_Ammeter_Descriptor;
struct ADC_Ammeter_Config;
}

namespace silk
{
namespace node
{

class ADC_Ammeter : public IProcessor
{
public:
    ADC_Ammeter(UAV& uav);

    bool init(std::shared_ptr<Node_Descriptor_Base> descriptor) override;
    std::shared_ptr<Node_Descriptor_Base> get_descriptor() const override;

    bool set_config(std::shared_ptr<Node_Config_Base> config) override;
    std::shared_ptr<Node_Config_Base> get_config() const override;

    auto send_message(rapidjson::Value const& json) -> rapidjson::Document;

    auto start(q::Clock::time_point tp) -> bool override;

    void set_input_stream_path(size_t idx, q::Path const& path);
    auto get_inputs() const -> std::vector<Input>;
    auto get_outputs() const -> std::vector<Output>;

    void process();

private:
    auto init() -> bool;

    UAV& m_uav;

    std::shared_ptr<ADC_Ammeter_Descriptor> m_descriptor;
    std::shared_ptr<ADC_Ammeter_Config> m_config;

    Sample_Accumulator<stream::IADC> m_accumulator;

    typedef Basic_Output_Stream<stream::ICurrent> Output_Stream;
    mutable std::shared_ptr<Output_Stream> m_output_stream;
};


}
}

