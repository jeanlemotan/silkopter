#include "BrainStdAfx.h"
#include "bus/SPI_Linux.h"

#include "hal.def.h"

namespace silk
{
namespace bus
{

SPI_Linux::SPI_Linux()
    : m_descriptor(new hal::SPI_Linux_Descriptor())
{
}

SPI_Linux::~SPI_Linux()
{
}

ts::Result<void> SPI_Linux::init(hal::IBus_Descriptor const& descriptor)
{
    auto specialized = dynamic_cast<hal::SPI_Linux_Descriptor const*>(&descriptor);
    if (!specialized)
    {
        return make_error("Wrong descriptor type");
    }
    *m_descriptor = *specialized;

    return init(m_descriptor->get_dev(), m_descriptor->get_speed());
}

std::shared_ptr<const hal::IBus_Descriptor> SPI_Linux::get_descriptor() const
{
    return m_descriptor;
}

ts::Result<void> SPI_Linux::init(std::string const& dev, uint32_t speed)
{
    QLOG_TOPIC("spi_linux::init");

    return m_spi_dev.init(dev, speed);
}

void SPI_Linux::lock()
{
}

auto SPI_Linux::try_lock() -> bool
{
    return true;
}

void SPI_Linux::unlock()
{
}

bool SPI_Linux::transfer(uint8_t const* tx_data, uint8_t* rx_data, size_t size, uint32_t speed)
{
    QLOG_TOPIC("SPI_Linux::transfer");
    return m_spi_dev.transfer(tx_data, rx_data, size, speed);
}

bool SPI_Linux::transfer_register(uint8_t reg, uint8_t const* tx_data, uint8_t* rx_data, size_t size, uint32_t speed)
{
    QLOG_TOPIC("SPI_Linux::transfer_register");
    return m_spi_dev.transfer_register(reg, tx_data, rx_data, size, speed);
}

}
}
