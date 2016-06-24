#pragma once

#include "HAL.h"
#include "common/stream/IPressure.h"
#include "common/stream/IVelocity.h"
#include "common/node/IProcessor.h"

#include "Sample_Accumulator.h"
#include "Basic_Output_Stream.h"


namespace silk
{
namespace hal
{
struct Pressure_Velocity_Descriptor;
struct Pressure_Velocity_Config;
}
}

namespace silk
{
namespace node
{

class Pressure_Velocity : public IProcessor
{
public:
    Pressure_Velocity(HAL& hal);

    bool init(hal::INode_Descriptor const& descriptor) override;
    std::shared_ptr<const hal::INode_Descriptor> get_descriptor() const override;

    bool set_config(hal::INode_Config const& config) override;
    std::shared_ptr<const hal::INode_Config> get_config() const override;

    //auto send_message(rapidjson::Value const& json) -> rapidjson::Document;

    auto start(q::Clock::time_point tp) -> bool override;

    void set_input_stream_path(size_t idx, q::Path const& path);
    auto get_inputs() const -> std::vector<Input>;
    auto get_outputs() const -> std::vector<Output>;

    void process();

private:
    auto init() -> bool;

    HAL& m_hal;

    std::shared_ptr<hal::Pressure_Velocity_Descriptor> m_descriptor;
    std::shared_ptr<hal::Pressure_Velocity_Config> m_config;

    Sample_Accumulator<stream::IPressure> m_accumulator;

    boost::optional<float> m_last_altitude;

    typedef Basic_Output_Stream<stream::IENU_Velocity> Output_Stream;
    mutable std::shared_ptr<Output_Stream> m_output_stream;
};


}
}

