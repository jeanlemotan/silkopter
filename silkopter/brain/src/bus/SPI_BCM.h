#pragma once

#include "common/bus/ISPI.h"

namespace silk
{
namespace hal
{
struct SPI_BCM_Descriptor;
}
}

namespace silk
{
namespace bus
{

class SPI_BCM : public ISPI
{
public:
    SPI_BCM();
    ~SPI_BCM();

    bool init(hal::IBus_Descriptor const& descriptor) override;
    std::shared_ptr<const hal::IBus_Descriptor> get_descriptor() const override;

    void lock();
    auto try_lock() -> bool;
    void unlock();

    virtual auto transfer(uint8_t const* tx_data, uint8_t* rx_data, size_t size, uint32_t speed = 0) -> bool override;
    virtual auto transfer_register(uint8_t reg, uint8_t const* tx_data, uint8_t* rx_data, size_t size, uint32_t speed = 0) -> bool override;

private:
    bool open(uint32_t dev, uint32_t speed, uint32_t mode);

    auto get_divider(uint32_t speed) const -> uint32_t;

    auto do_transfer(uint8_t const* tx_data, uint8_t* rx_data, size_t size, uint32_t speed) -> bool;

    std::shared_ptr<hal::SPI_BCM_Descriptor> m_descriptor;
    uint32_t m_dev = 0;
    uint32_t m_speed = 0;
    uint32_t m_mode = 0;

    uint32_t m_clock_divider = 0;

    std::vector<uint8_t> m_tx_buffer;
    std::vector<uint8_t> m_rx_buffer;
    //std::vector<uint8_t> m_dummy_buffer;

    static std::mutex s_mutex;
};

}
}
