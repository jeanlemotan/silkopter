#pragma once

#include "HAL.h"
#include "common/node/ISource.h"
#include "common/stream/IDistance.h"
#include "common/bus/IUART.h"

#include "Basic_Output_Stream.h"

namespace sz
{
namespace MaxSonar
{
struct Init_Params;
struct Config;
}
}


namespace silk
{
namespace node
{

class MaxSonar : public ISource
{
public:
    MaxSonar(HAL& hal);

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

    HAL& m_hal;

    std::weak_ptr<bus::IUART> m_bus;

    std::shared_ptr<sz::MaxSonar::Init_Params> m_init_params;
    std::shared_ptr<sz::MaxSonar::Config> m_config;

    typedef Basic_Output_Stream<stream::IDistance> Output_Stream;
    mutable std::shared_ptr<Output_Stream> m_output_stream;

    std::deque<uint8_t> m_read_data;

    float m_last_distance = 0;
    q::Clock::time_point m_last_reading_tp = q::Clock::now();

    struct Stats
    {
        q::Clock::time_point last_report_tp = q::Clock::now();
        size_t added = 0;

        bool operator==(Stats const& o) const { return added == o.added; }
        bool operator!=(Stats const& o) const { return !operator==(o); }
    } m_stats;
};

}
}
