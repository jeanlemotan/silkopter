#pragma once

#include "UAV.h"
#include "common/node/ISource.h"
#include "common/stream/IDistance.h"
#include "common/bus/IUART.h"

#include "Basic_Output_Stream.h"


namespace silk
{
struct SRF01_Descriptor;
struct SRF01_Config;
}


namespace silk
{
namespace node
{

class SRF01 : public ISource
{
public:
    SRF01(UAV& uav);

    bool init(std::shared_ptr<Node_Descriptor_Base> descriptor) override;
    std::shared_ptr<Node_Descriptor_Base> get_descriptor() const override;

    bool set_config(std::shared_ptr<Node_Config_Base> config) override;
    std::shared_ptr<Node_Config_Base> get_config() const override;

    auto send_message(rapidjson::Value const& json) -> rapidjson::Document;

    auto start(q::Clock::time_point tp) -> bool override;

    auto get_outputs() const -> std::vector<Output>;

    void process();

private:
    auto init() -> bool;

    auto send_command(bus::IUART& bus, uint8_t command) -> bool;
    auto read_response(bus::IUART& bus, uint8_t sent_command, uint8_t* response, size_t size) -> bool;
    auto read_response_u8(bus::IUART& bus, uint8_t sent_command, uint8_t& response) -> bool;
    auto read_response_u16(bus::IUART& bus, uint8_t sent_command, uint16_t& response) -> bool;

    void trigger(bus::IUART& bus);

    UAV& m_uav;

    std::weak_ptr<bus::IUART> m_bus;

    std::shared_ptr<SRF01_Descriptor> m_descriptor;
    std::shared_ptr<SRF01_Config> m_config;

    typedef Basic_Output_Stream<stream::IDistance> Output_Stream;
    mutable std::shared_ptr<Output_Stream> m_output_stream;
    q::Clock::time_point m_last_trigger_tp;

    std::deque<uint8_t> m_read_data;

};

}
}
