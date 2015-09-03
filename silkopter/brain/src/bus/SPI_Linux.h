#pragma once

#include "common/bus/ISPI.h"

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

    virtual auto transfer(uint8_t const* tx_data, uint8_t* rx_data, size_t size, size_t speed = 0) -> bool override;
    virtual auto transfer_register(uint8_t reg, uint8_t const* tx_data, uint8_t* rx_data, size_t size, size_t speed = 0) -> bool override;

private:
    auto init() -> bool;

    auto open() -> bool;

    auto do_transfer(uint8_t const* tx_data, uint8_t* rx_data, size_t size, size_t speed) -> bool;


    std::shared_ptr<sz::SPI_Linux::Init_Params> m_init_params;
    std::shared_ptr<sz::SPI_Linux::Config> m_config;

    int m_fd = -1;
    std::vector<uint8_t> m_tx_buffer;
    std::vector<uint8_t> m_rx_buffer;
};

}
}
