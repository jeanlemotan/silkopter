#pragma once

#include "HAL.h"
#include "common/node/bus/IUART.h"

namespace sz
{
namespace UART_Linux
{
struct Init_Params;
struct Config;
}
}


namespace silk
{
namespace node
{
namespace bus
{

class UART_Linux : public IUART
{
public:
    UART_Linux(HAL& hal);
    ~UART_Linux();

    auto init(rapidjson::Value const& init_params, rapidjson::Value const& config) -> bool;
    auto get_init_params() -> rapidjson::Document;

    auto set_config(rapidjson::Value const& json) -> bool;
    auto get_config() -> rapidjson::Document;

    void close();

    auto get_name() const -> std::string const&;

    void lock();
    auto try_lock() -> bool;
    void unlock();

    auto read(uint8_t* data, size_t max_size) -> size_t;
    auto write(uint8_t const* data, size_t size) -> bool;

private:
    auto init() -> bool;

    HAL& m_hal;
    std::shared_ptr<sz::UART_Linux::Init_Params> m_init_params;
    std::shared_ptr<sz::UART_Linux::Config> m_config;

    int m_fd = -1;
    std::recursive_mutex m_mutex;
    std::vector<uint8_t> m_buffer;
};

}
}
}
