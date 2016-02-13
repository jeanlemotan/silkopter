#pragma once

#include "HAL.h"
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
    AVRADC(HAL& hal);

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
    std::weak_ptr<bus::II2C> m_i2c;

    std::shared_ptr<sz::AVRADC::Init_Params> m_init_params;
    std::shared_ptr<sz::AVRADC::Config> m_config;

    q::Clock::time_point m_last_tp = q::Clock::now();

    typedef Basic_Output_Stream<stream::IADC> Output_Stream;
    mutable std::array<std::shared_ptr<Output_Stream>, 2> m_adcs;

    uint8_t m_crt_adc = 0;
};


}
}
