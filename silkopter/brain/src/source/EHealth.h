#pragma once

#include "HAL.h"
#include "common/node/ISource.h"
#include "common/node/stream/IAcceleration.h"
#include "common/node/bus/IUART.h"


namespace silk
{
namespace node
{

class EHealth : public ISource
{
public:

    EHealth(HAL& hal);
    ~EHealth();

    auto init(rapidjson::Value const& init_params) -> bool;
    auto get_init_params() const -> rapidjson::Document const&;

    auto set_config(rapidjson::Value const& json) -> bool;
    auto get_config() const -> rapidjson::Document;

    auto send_message(rapidjson::Value const& json) -> rapidjson::Document;

    auto get_outputs() const -> std::vector<Output>;

    void process();

private:
    auto init() -> bool;

    HAL& m_hal;

    bus::IUART_wptr m_uart;

    struct Buses
    {
        bus::IUART_ptr uart;
    };

    rapidjson::Document m_init_paramsj;

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
