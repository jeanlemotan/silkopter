#pragma once

#include "common/bus/ISPI.h"
#include "utils/hw/SPI_Dev.h"

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

    ts::Result<void> init(hal::IBus_Descriptor const& descriptor) override;
    std::shared_ptr<const hal::IBus_Descriptor> get_descriptor() const override;

    void lock();
    auto try_lock() -> bool;
    void unlock();

    virtual auto transfer(uint8_t const* tx_data, uint8_t* rx_data, size_t size, uint32_t speed = 0) -> bool override;
    virtual auto transfer_register(uint8_t reg, uint8_t const* tx_data, uint8_t* rx_data, size_t size, uint32_t speed = 0) -> bool override;

private:
    ts::Result<void> init(std::string const& dev, uint32_t speed);

    std::shared_ptr<hal::SPI_Linux_Descriptor> m_descriptor;
    util::hw::SPI_Dev m_spi_dev;
};

}
}
