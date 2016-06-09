#pragma once

#include "UAV.h"
#include "common/node/ISource.h"
#include "common/stream/IDistance.h"
#include "common/bus/IUART.h"

#include "Basic_Output_Stream.h"


namespace silk
{
struct MaxSonar_Descriptor;
struct MaxSonar_Config;
}


namespace silk
{
namespace node
{

class MaxSonar : public ISource
{
public:
    MaxSonar(UAV& uav);

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

    UAV& m_uav;

    std::weak_ptr<bus::IUART> m_bus;

    std::shared_ptr<MaxSonar_Descriptor> m_descriptor;
    std::shared_ptr<MaxSonar_Config> m_config;

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
