#pragma once

#include "ISPI.h"
#include <atomic>
#include <vector>

namespace util
{
namespace hw
{

class SPI_PIGPIO : public ISPI
{
public:
    SPI_PIGPIO();
    ~SPI_PIGPIO();

    ts::Result<void> init(uint32_t port, uint32_t channel, uint32_t speed, uint32_t mode);

    bool transfer(void const* tx_data, void* rx_data, size_t size, uint32_t speed = 0) override;
    bool transfer_register(uint8_t reg, void const* tx_data, void* rx_data, size_t size, uint32_t speed = 0) override;

    bool transfers(Transfer const* transfers, size_t transfer_count, uint32_t speed = 0) override;

private:
    bool do_transfer(void const* tx_data, void* rx_data, size_t size, uint32_t speed);

    uint32_t m_channel = 0;
    uint32_t m_speed = 0;
    uint32_t m_mode = 0;
    int m_fd = -1;

    mutable std::vector<uint8_t> m_tx_buffer;
    mutable std::vector<uint8_t> m_rx_buffer;

    mutable std::atomic_bool m_is_used = { false };
};

}
}
