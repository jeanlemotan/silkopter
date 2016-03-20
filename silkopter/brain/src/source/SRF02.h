#pragma once

#include "UAV.h"
#include "common/node/ISource.h"
#include "common/stream/IDistance.h"
#include "common/bus/II2C.h"

#include "Basic_Output_Stream.h"

namespace sz
{
namespace SRF02
{
struct Init_Params;
struct Config;
}
}


namespace silk
{
namespace node
{

class SRF02 : public ISource
{
public:
    SRF02(UAV& uav);

    auto init(rapidjson::Value const& init_params) -> bool;
    auto get_init_params() const -> rapidjson::Document;

    auto set_config(rapidjson::Value const& json) -> bool;
    auto get_config() const -> rapidjson::Document;

    auto send_message(rapidjson::Value const& json) -> rapidjson::Document;

    auto start(q::Clock::time_point tp) -> bool override;

    auto get_outputs() const -> std::vector<Output>;

    void process();

private:
    auto init() -> bool;

    void trigger(bus::II2C& bus);

    UAV& m_uav;

    std::weak_ptr<bus::II2C> m_bus;

    std::shared_ptr<sz::SRF02::Init_Params> m_init_params;
    std::shared_ptr<sz::SRF02::Config> m_config;

    typedef Basic_Output_Stream<stream::IDistance> Output_Stream;
    mutable std::shared_ptr<Output_Stream> m_output_stream;
    q::Clock::time_point m_last_trigger_tp;

};

}
}
