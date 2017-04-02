#pragma once

#include "HAL.h"
#include "common/node/ISource.h"
#include "common/stream/IADC.h"
#include "common/bus/II2C.h"


namespace silk
{
namespace hal
{
struct RC5T619_Descriptor;
struct RC5T619_Config;
}
}



namespace silk
{
namespace node
{

class RC5T619 : public ISource
{
public:
    RC5T619(HAL& hal);

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

    HAL& m_hal;

    std::weak_ptr<bus::II2C_Bus> m_i2c_bus;

    std::shared_ptr<hal::RC5T619_Descriptor> m_descriptor;
    std::shared_ptr<hal::RC5T619_Config> m_config;

    Clock::time_point m_last_tp = Clock::now();
    Clock::duration m_dt;

    size_t m_stage = 0;

    struct Stream : public stream::IADC
    {
        auto get_samples() const -> std::vector<Sample> const& { return samples; }
        auto get_rate() const -> uint32_t { return rate; }

        std::vector<Sample> samples;
        Clock::time_point last_tp = Clock::now();
        Sample last_sample;
        uint32_t rate = 0;
    };
    mutable std::shared_ptr<Stream> m_adc[2];
};


}
}
