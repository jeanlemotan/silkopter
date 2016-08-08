#include "BrainStdAfx.h"
#include "bus/SPI_BCM.h"

#ifdef RASPBERRY_PI

extern "C"
{
    #include "utils/hw/bcm2835.h"
}

#endif

#include "hal.def.h"

///////////////////////////////////////////////////////////////////

namespace silk
{
namespace bus
{

std::mutex SPI_BCM::s_mutex;


SPI_BCM::SPI_BCM()
    : m_descriptor(new hal::SPI_BCM_Descriptor())
{
}

SPI_BCM::~SPI_BCM()
{
}

ts::Result<void> SPI_BCM::init(hal::IBus_Descriptor const& descriptor)
{
    auto specialized = dynamic_cast<hal::SPI_BCM_Descriptor const*>(&descriptor);
    if (!specialized)
    {
        return make_error("Wrong descriptor type");
    }
    *m_descriptor = *specialized;

    return open(specialized->get_dev(), specialized->get_speed(), specialized->get_mode());
}

std::shared_ptr<const hal::IBus_Descriptor> SPI_BCM::get_descriptor() const
{
    return m_descriptor;
}

ts::Result<void> SPI_BCM::open(uint32_t dev, uint32_t speed, uint32_t mode)
{
    if (dev > 1)
    {
        return make_error("Only SPI devices 0 & 1 are allowed");
    }
    if (mode > 3)
    {
        return make_error("Only SPI modes 0 to 3 are allowed");
    }

    m_dev = dev;
    m_speed = speed;
    m_mode = mode;

#ifdef RASPBERRY_PI

    bcm2835_spi_begin();
    bcm2835_spi_setBitOrder(BCM2835_SPI_BIT_ORDER_MSBFIRST);      // The default
    bcm2835_spi_setChipSelectPolarity(dev, LEVEL_LOW);      // the default

#endif

    return ts::success;
}

void SPI_BCM::lock()
{
    s_mutex.lock();
}

auto SPI_BCM::try_lock() -> bool
{
    return s_mutex.try_lock();
}

void SPI_BCM::unlock()
{
    s_mutex.unlock();
}

auto SPI_BCM::get_divider(uint32_t speed) const -> uint32_t
{
#ifdef RASPBERRY_PI
    if (speed > 125000000) return BCM2835_SPI_CLOCK_DIVIDER_2;
    else if (speed > 62000000) return BCM2835_SPI_CLOCK_DIVIDER_4;
    else if (speed > 31200000) return BCM2835_SPI_CLOCK_DIVIDER_8;
    else if (speed > 15500000) return BCM2835_SPI_CLOCK_DIVIDER_16;
    else if (speed > 7800000) return BCM2835_SPI_CLOCK_DIVIDER_32;
    else if (speed > 3900000) return BCM2835_SPI_CLOCK_DIVIDER_64;
    else if (speed > 1900000) return BCM2835_SPI_CLOCK_DIVIDER_128;
    else if (speed > 970000) return BCM2835_SPI_CLOCK_DIVIDER_256;
    else if (speed > 480000) return BCM2835_SPI_CLOCK_DIVIDER_512;
    else if (speed > 240000) return BCM2835_SPI_CLOCK_DIVIDER_1024;
    else if (speed > 120000) return BCM2835_SPI_CLOCK_DIVIDER_2048;
    else if (speed > 60000) return BCM2835_SPI_CLOCK_DIVIDER_4096;
    else if (speed > 30000) return BCM2835_SPI_CLOCK_DIVIDER_8192;
    else if (speed > 15000) return BCM2835_SPI_CLOCK_DIVIDER_16384;
    else if (speed > 7500) return BCM2835_SPI_CLOCK_DIVIDER_32768;
    else return BCM2835_SPI_CLOCK_DIVIDER_65536;
#else
    return 0;
#endif
}

auto SPI_BCM::do_transfer(uint8_t const* tx_data, uint8_t* rx_data, size_t size, uint32_t speed) -> bool
{
    QASSERT(size > 0);
    if (size == 0)
    {
        return false;
    }

#ifdef RASPBERRY_PI

    uint32_t divider = get_divider(speed ? speed : m_speed);

    bcm2835_spi_chipSelect(m_dev);
    bcm2835_spi_setDataMode(m_mode);
    bcm2835_spi_setClockDivider(divider);

    if (!tx_data)
    {
        tx_data = rx_data;
    }
    if (!rx_data)
    {
        rx_data = const_cast<uint8_t*>(tx_data);
    }

    bcm2835_spi_transfernb(reinterpret_cast<char*>(const_cast<uint8_t*>(tx_data)), reinterpret_cast<char*>(rx_data), size);

#endif

    return true;
}

auto SPI_BCM::transfer(uint8_t const* tx_data, uint8_t* rx_data, size_t size, uint32_t speed) -> bool
{
    QLOG_TOPIC("spi_bcm::transfer");
    return do_transfer(tx_data, rx_data, size, speed);
}

auto SPI_BCM::transfer_register(uint8_t reg, uint8_t const* tx_data, uint8_t* rx_data, size_t size, uint32_t speed) -> bool
{
    QLOG_TOPIC("spi_bcm::transfer_register");

    m_tx_buffer.resize(size + 1);
    m_tx_buffer[0] = reg;
    std::copy(tx_data, tx_data + size, m_tx_buffer.begin() + 1);

    m_rx_buffer.resize(size + 1);
    if (!do_transfer(m_tx_buffer.data(), m_rx_buffer.data(), size + 1, speed))
    {
        return false;
    }

    std::copy(m_rx_buffer.begin() + 1, m_rx_buffer.end(), rx_data);
    return true;
}


}
}


