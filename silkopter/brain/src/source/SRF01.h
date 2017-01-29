#pragma once

#include "HAL.h"
#include "common/node/ISource.h"
#include "common/stream/IDistance.h"
#include "common/bus/IUART.h"

#include "Basic_Output_Stream.h"


namespace silk
{
namespace hal
{
struct SRF01_Descriptor;
struct SRF01_Config;
}
}


namespace silk
{
namespace node
{

class SRF01 : public ISource
{
public:
    SRF01(HAL& hal);

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

    auto send_command(bus::IUART& bus, uint8_t command) -> bool;
    auto read_response(bus::IUART& bus, uint8_t sent_command, uint8_t* response, size_t size) -> bool;
    auto read_response_u8(bus::IUART& bus, uint8_t sent_command, uint8_t& response) -> bool;
    auto read_response_u16(bus::IUART& bus, uint8_t sent_command, uint16_t& response) -> bool;

    void trigger(bus::IUART& bus);

    HAL& m_hal;

    std::weak_ptr<bus::IUART> m_bus;

    std::shared_ptr<hal::SRF01_Descriptor> m_descriptor;
    std::shared_ptr<hal::SRF01_Config> m_config;

    typedef Basic_Output_Stream<stream::IDistance> Output_Stream;
    mutable std::shared_ptr<Output_Stream> m_output_stream;
    Clock::time_point m_last_trigger_tp;

    std::deque<uint8_t> m_read_data;

};

}
}
