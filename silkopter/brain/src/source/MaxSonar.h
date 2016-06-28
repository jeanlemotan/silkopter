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
struct MaxSonar_Descriptor;
struct MaxSonar_Config;
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

    ts::Result<void> init(hal::INode_Descriptor const& descriptor) override;
    std::shared_ptr<const hal::INode_Descriptor> get_descriptor() const override;

    ts::Result<void> set_config(hal::INode_Config const& config) override;
    std::shared_ptr<const hal::INode_Config> get_config() const override;

    //auto send_message(rapidjson::Value const& json) -> rapidjson::Document;

    ts::Result<void> start(q::Clock::time_point tp) override;

    auto get_outputs() const -> std::vector<Output>;

    void process();

private:
    ts::Result<void> init();

    HAL& m_hal;

    std::weak_ptr<bus::IUART> m_bus;

    std::shared_ptr<hal::MaxSonar_Descriptor> m_descriptor;
    std::shared_ptr<hal::MaxSonar_Config> m_config;

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
