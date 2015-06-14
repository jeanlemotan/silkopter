#pragma once

#include "common/node/bus/ISPI.h"

namespace sz
{
namespace SPI_Linux
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

class SPI_Linux : public ISPI
{
public:
    SPI_Linux();
    ~SPI_Linux();

    auto init(rapidjson::Value const& init_params) -> bool;
    auto get_init_params() const -> rapidjson::Document;

    auto set_config(rapidjson::Value const& json) -> bool;
    auto get_config() const -> rapidjson::Document;

    void close();

    void lock();
    auto try_lock() -> bool;
    void unlock();

    auto read(uint8_t* data, size_t size) -> bool;
    auto write(uint8_t const* data, size_t size) -> bool;

    auto read_register(uint8_t reg, uint8_t* data, size_t size) -> bool;
    auto write_register(uint8_t reg, uint8_t const* data, size_t size) -> bool;

private:
    auto init() -> bool;

    auto transfer(uint8_t const* tx_data, uint8_t* rx_data, size_t size) -> bool;


    std::shared_ptr<sz::SPI_Linux::Init_Params> m_init_params;
    std::shared_ptr<sz::SPI_Linux::Config> m_config;

    int m_fd = -1;
    std::recursive_mutex m_mutex;
    std::vector<uint8_t> m_tx_buffer;
    std::vector<uint8_t> m_rx_buffer;
};

}
}
}
