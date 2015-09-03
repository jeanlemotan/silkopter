#pragma once

#include "common/bus/ISPI.h"

namespace sz
{
namespace SPI_RPI
{
struct Init_Params;
struct Config;
}
}


namespace silk
{
namespace bus
{

class SPI_RPI : public ISPI
{
public:
    SPI_RPI();
    ~SPI_RPI();

    auto init(rapidjson::Value const& init_params) -> bool;
    auto get_init_params() const -> rapidjson::Document;

    auto set_config(rapidjson::Value const& json) -> bool;
    auto get_config() const -> rapidjson::Document;

    void lock();
    auto try_lock() -> bool;
    void unlock();

    virtual auto transfer(uint8_t const* tx_data, uint8_t* rx_data, size_t size, size_t speed = 0) -> bool override;
    virtual auto transfer_register(uint8_t reg, uint8_t const* tx_data, uint8_t* rx_data, size_t size, size_t speed = 0) -> bool override;

private:
    auto init() -> bool;

    auto get_divider(uint32_t speed) const -> uint32_t;

    auto do_transfer(uint8_t const* tx_data, uint8_t* rx_data, size_t size, size_t speed) -> bool;


    std::shared_ptr<sz::SPI_RPI::Init_Params> m_init_params;
    std::shared_ptr<sz::SPI_RPI::Config> m_config;

    uint32_t m_clock_divider = 0;

    std::vector<uint8_t> m_tx_buffer;
    std::vector<uint8_t> m_rx_buffer;
    //std::vector<uint8_t> m_dummy_buffer;

    static std::mutex s_mutex;
};

}
}
