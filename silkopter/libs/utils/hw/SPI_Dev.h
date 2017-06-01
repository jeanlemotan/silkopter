#pragma once

#include "ISPI.h"
#include <mutex>
#include <vector>
#include <string>
#include <linux/spi/spidev.h>

namespace util
{
namespace hw
{

class SPI_Dev : public ISPI
{
public:
    SPI_Dev();
    ~SPI_Dev();

    ts::Result<void> init(std::string const& device, uint32_t speed);

    void lock() override;
    bool try_lock() override;
    void unlock() override;

    bool transfer(void const* tx_data, void* rx_data, size_t size, uint32_t speed = 0) override;
    bool transfer_register(uint8_t reg, void const* tx_data, void* rx_data, size_t size, uint32_t speed = 0) override;

    bool transfers(Transfer const* transfers, size_t transfer_count, uint32_t speed = 0) override;

private:
    bool do_transfer(void const* tx_data, void* rx_data, size_t size, uint32_t speed);

    std::string m_device;
    uint32_t m_speed = 0;

    std::array<spi_ioc_transfer, 256> m_spi_transfers;

    int m_fd = -1;

    mutable std::vector<uint8_t> m_tx_buffer;
    mutable std::vector<uint8_t> m_rx_buffer;

    mutable std::recursive_mutex m_mutex;
};

}
}
