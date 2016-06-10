#pragma once

#include "UAV.h"
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

    EHealth(UAV& uav);
    ~EHealth();

    bool init(std::shared_ptr<INode_Descriptor> descriptor) override;
    std::shared_ptr<INode_Descriptor> get_descriptor() const override;

    bool set_config(std::shared_ptr<INode_Config> config) override;
    std::shared_ptr<INode_Config> get_config() const override;

    //auto send_message(rapidjson::Value const& json) -> rapidjson::Document;

    auto start(q::Clock::time_point tp) -> bool override;

    auto get_outputs() const -> std::vector<Output>;

    void process();

private:
    auto init() -> bool;

    UAV& m_uav;

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
