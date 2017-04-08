#include "SPI_BCM.h"

#ifdef RASPBERRY_PI
extern "C"
{
    #include "utils/hw/bcm2835.h"
}
#endif

namespace util
{
namespace hw
{

SPI_BCM::SPI_BCM()
{
}

SPI_BCM::~SPI_BCM()
{
}

ts::Result<void> SPI_BCM::init(uint32_t device, uint32_t speed, uint32_t mode)
{
    if (device > 1)
    {
        return ts::Error("Only SPI devices 0 & 1 are allowed");
    }
    if (mode > 3)
    {
        return ts::Error("Only SPI modes 0 to 3 are allowed");
    }

    m_device = device;
    m_speed = speed;
    m_mode = mode;

#ifdef RASPBERRY_PI
    bcm2835_spi_begin();
    bcm2835_spi_setBitOrder(BCM2835_SPI_BIT_ORDER_MSBFIRST);      // The default
    bcm2835_spi_setChipSelectPolarity(m_device, LEVEL_LOW);      // the default
#endif

    return ts::success;
}

void SPI_BCM::lock()
{
    m_mutex.lock();
}

bool SPI_BCM::try_lock()
{
    return m_mutex.try_lock();
}
void SPI_BCM::unlock()
{
    m_mutex.unlock();
}

uint32_t SPI_BCM::get_divider(uint32_t speed) const
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

bool SPI_BCM::do_transfer(void const* tx_data, void* rx_data, size_t size, uint32_t speed)
{
    QASSERT(size > 0);
    if (size == 0)
    {
        return false;
    }

    std::lock_guard<SPI_BCM> lg(*this);

#ifdef RASPBERRY_PI
    uint32_t divider = get_divider(speed ? speed : m_speed);

    bcm2835_spi_chipSelect(m_device);
    bcm2835_spi_setDataMode(m_mode);
    bcm2835_spi_setClockDivider(divider);

    if (!tx_data)
    {
        tx_data = rx_data;
    }
    if (!rx_data)
    {
        rx_data = const_cast<void*>(tx_data);
    }

    bcm2835_spi_transfernb(reinterpret_cast<char*>(const_cast<void*>(tx_data)), reinterpret_cast<char*>(rx_data), size);
#endif

    return true;
}

bool SPI_BCM::transfer(void const* tx_data, void* rx_data, size_t size, uint32_t speed)
{
    QLOG_TOPIC("spi_bcm::transfer");
    return do_transfer(tx_data, rx_data, size, speed);
}

bool SPI_BCM::transfer_register(uint8_t reg, void const* tx_data, void* rx_data, size_t size, uint32_t speed)
{
    QLOG_TOPIC("spi_bcm::transfer_register");

    std::lock_guard<SPI_BCM> lg(*this);

    m_tx_buffer.resize(size + 1);
    m_tx_buffer[0] = reg;
    std::copy(reinterpret_cast<uint8_t const*>(tx_data), reinterpret_cast<uint8_t const*>(tx_data) + size, m_tx_buffer.begin() + 1);

    m_rx_buffer.resize(size + 1);
    if (!do_transfer(m_tx_buffer.data(), m_rx_buffer.data(), size + 1, speed))
    {
        return false;
    }

    std::copy(m_rx_buffer.begin() + 1, m_rx_buffer.end(), reinterpret_cast<uint8_t*>(rx_data));
    return true;
}

}
}
