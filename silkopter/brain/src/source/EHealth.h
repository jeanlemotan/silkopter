#pragma once

#include "HAL.h"
#include "common/node/ISource.h"
#include "common/stream/IAcceleration.h"
#include "common/bus/IUART.h"


namespace silk
{
namespace node
{

class EHealth : public ISource
{
public:

    EHealth(HAL& hal);
    ~EHealth();

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

    std::weak_ptr<bus::IUART> m_uart;

    struct Buses
    {
        std::shared_ptr<bus::IUART> uart;
    };

    std::array<char, 1024> m_temp_buffer;
    std::string m_buffer;

    q::Clock::time_point m_last_tp;

    struct Stream : public stream::IAcceleration
    {
        auto get_samples() const -> std::vector<Sample> const& { return samples; }
        auto get_rate() const -> uint32_t { return rate; }

        uint32_t rate = 0;
        std::vector<Sample> samples;
        Sample last_sample;
    };
    mutable std::shared_ptr<Stream> m_stream;
};


}
}
