#pragma once

#include "HAL.h"
#include "common/node/ISource.h"
#include "common/stream/IADC.h"
#include "common/bus/II2C.h"

#include "Basic_Output_Stream.h"


namespace silk
{
namespace hal
{
struct AVRADC_Descriptor;
struct AVRADC_Config;
}
}



namespace silk
{
namespace node
{

class AVRADC : public ISource
{
public:
    AVRADC(HAL& hal);

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
    std::weak_ptr<bus::II2C> m_i2c;

    std::shared_ptr<hal::AVRADC_Descriptor> m_descriptor;
    std::shared_ptr<hal::AVRADC_Config> m_config;

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
