#pragma once

#include "common/bus/ISPI.h"

namespace silk
{
namespace hal
{
struct SPI_Linux_Descriptor;
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

    bool init(hal::IBus_Descriptor const& descriptor) override;
    std::shared_ptr<const hal::IBus_Descriptor> get_descriptor() const override;

    void lock();
    auto try_lock() -> bool;
    void unlock();

    virtual auto transfer(uint8_t const* tx_data, uint8_t* rx_data, size_t size, uint32_t speed = 0) -> bool override;
    virtual auto transfer_register(uint8_t reg, uint8_t const* tx_data, uint8_t* rx_data, size_t size, uint32_t speed = 0) -> bool override;

private:
    bool init(std::string const& dev, uint32_t speed);
    bool open(std::string const& dev, uint32_t speed);
    void close();

    auto do_transfer(uint8_t const* tx_data, uint8_t* rx_data, size_t size, uint32_t speed) -> bool;

    std::shared_ptr<hal::SPI_Linux_Descriptor> m_descriptor;

    int m_fd = -1;
    uint32_t m_speed = 100000;
    std::vector<uint8_t> m_tx_buffer;
    std::vector<uint8_t> m_rx_buffer;
};

}
}
