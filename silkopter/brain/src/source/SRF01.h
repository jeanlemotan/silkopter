#pragma once

#include "HAL.h"
#include "common/node/ISource.h"
#include "common/node/stream/IDistance.h"
#include "common/node/bus/IUART.h"

#include "Basic_Output_Stream.h"

namespace sz
{
namespace SRF01
{
struct Init_Params;
struct Config;
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

    auto init(rapidjson::Value const& init_params) -> bool;
    auto get_init_params() const -> rapidjson::Document;

    auto set_config(rapidjson::Value const& json) -> bool;
    auto get_config() const -> rapidjson::Document;

    auto send_message(rapidjson::Value const& json) -> rapidjson::Document;

    auto get_outputs() const -> std::vector<Output>;

    void process();

private:
    auto init() -> bool;

    void trigger(bus::IUART& bus);

    HAL& m_hal;

    std::weak_ptr<bus::IUART> m_bus;

    std::shared_ptr<sz::SRF01::Init_Params> m_init_params;
    std::shared_ptr<sz::SRF01::Config> m_config;

    typedef Basic_Output_Stream<stream::IDistance> Output_Stream;
    mutable std::shared_ptr<Output_Stream> m_output_stream;
    q::Clock::time_point m_last_trigger_tp;

};

}
}
