#pragma once

#include "HAL.h"
#include "common/node/ISource.h"
#include "common/stream/IDistance.h"
#include "common/bus/II2C.h"

#include "Basic_Output_Stream.h"


namespace silk
{
namespace hal
{
struct SRF02_Descriptor;
struct SRF02_Config;
}
}


namespace silk
{
namespace node
{

class SRF02 : public ISource
{
public:
    SRF02(HAL& hal);

    ts::Result<void> init(hal::INode_Descriptor const& descriptor) override;
    std::shared_ptr<const hal::INode_Descriptor> get_descriptor() const override;

    ts::Result<void> set_config(hal::INode_Config const& config) override;
    std::shared_ptr<const hal::INode_Config> get_config() const override;

    ts::Result<std::shared_ptr<messages::INode_Message>> send_message(messages::INode_Message const& message) override;

    ts::Result<void> start(Clock::time_point tp) override;

    auto get_outputs() const -> std::vector<Output>;

    void process();

private:
    ts::Result<void> init();

    void trigger(util::hw::II2C& i2c);

    HAL& m_hal;

    std::weak_ptr<bus::II2C_Bus> m_i2c_bus;

    std::shared_ptr<hal::SRF02_Descriptor> m_descriptor;
    std::shared_ptr<hal::SRF02_Config> m_config;

    typedef Basic_Output_Stream<stream::IDistance> Output_Stream;
    mutable std::shared_ptr<Output_Stream> m_output_stream;
    Clock::time_point m_last_trigger_tp;

};

}
}
