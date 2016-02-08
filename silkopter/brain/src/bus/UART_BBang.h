#pragma once

#include "common/bus/IUART.h"

namespace sz
{
namespace UART_BBang
{
struct Init_Params;
struct Config;
}
}


namespace silk
{
namespace bus
{

class UART_BBang : public IUART
{
public:
    UART_BBang();
    ~UART_BBang();

    auto init(rapidjson::Value const& init_params) -> bool;
    auto get_init_params() const -> rapidjson::Document;

    auto set_config(rapidjson::Value const& json) -> bool;
    auto get_config() const -> rapidjson::Document;

    void close();

    void lock();
    auto try_lock() -> bool;
    void unlock();

    auto read(uint8_t* data, size_t max_size) -> size_t;
    auto write(uint8_t const* data, size_t size) -> bool;

    void send_break();

private:
    auto init() -> bool;

    std::shared_ptr<sz::UART_BBang::Init_Params> m_init_params;
    std::shared_ptr<sz::UART_BBang::Config> m_config;

    std::recursive_mutex m_mutex;

    bool m_is_initialized = false;
};

}
}
