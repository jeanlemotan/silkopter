#pragma once

#include "UAV.h"
#include "common/node/ISource.h"
#include "common/stream/IADC.h"
#include "common/bus/II2C.h"

#include "Basic_Output_Stream.h"

namespace sz
{
namespace AVRADC
{
struct Init_Params;
struct Config;
}
}



namespace silk
{
namespace node
{

class AVRADC : public ISource
{
public:
    AVRADC(UAV& uav);

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

    UAV& m_uav;
    std::weak_ptr<bus::II2C> m_i2c;

    std::shared_ptr<sz::AVRADC::Init_Params> m_init_params;
    std::shared_ptr<sz::AVRADC::Config> m_config;

    q::Clock::time_point m_last_process_tp = q::Clock::now();
    q::Clock::time_point m_last_reading_tp = q::Clock::now();

    typedef Basic_Output_Stream<stream::IADC> Output_Stream;
    mutable std::array<std::shared_ptr<Output_Stream>, 2> m_adcs;
    std::array<float, 2> m_last_values;

    struct Stats
    {
        q::Clock::time_point last_report_tp = q::Clock::now();
        size_t added = 0;
        size_t bus_failures = 0;

        bool operator==(Stats const& o) const { return added == o.added && bus_failures == o.bus_failures; }
        bool operator!=(Stats const& o) const { return !operator==(o); }
    } m_stats;
};


}
}
