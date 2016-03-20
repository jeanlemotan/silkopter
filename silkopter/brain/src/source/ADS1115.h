#pragma once

#include "UAV.h"
#include "common/node/ISource.h"
#include "common/stream/IADC.h"
#include "common/bus/II2C.h"

#include "Basic_Output_Stream.h"

namespace sz
{
namespace ADS1115
{
struct Init_Params;
struct Config;
}
}



namespace silk
{
namespace node
{

class ADS1115 : public ISource
{
public:
    ADS1115(UAV& uav);

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

    std::shared_ptr<sz::ADS1115::Init_Params> m_init_params;
    std::shared_ptr<sz::ADS1115::Config> m_config;

    q::Clock::time_point m_last_tp = q::Clock::now();

    typedef Basic_Output_Stream<stream::IADC> Output_Stream;
    mutable std::array<std::shared_ptr<Output_Stream>, 4> m_adcs;

    auto set_config_register(bus::II2C& i2c) -> bool;

    struct Config_Register
    {
        uint16_t gain = 0;
        uint16_t mux = 0;
        uint16_t status = 0;
        uint16_t mode = 0;
        uint16_t rate = 0;
        uint16_t comparator = 0;
        uint16_t polarity = 0;
        uint16_t latch = 0;
        uint16_t queue = 0;
    } m_config_register;

    uint8_t m_crt_adc = 0;
};


}
}
