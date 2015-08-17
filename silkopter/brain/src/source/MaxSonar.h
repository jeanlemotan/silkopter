#pragma once

#include "HAL.h"
#include "common/node/ISource.h"
#include "common/node/stream/IDistance.h"
#include "common/node/bus/IUART.h"

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

};

}
}
