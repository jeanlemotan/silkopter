#include "SPI_PIGPIO.h"

#ifdef RASPBERRY_PI
extern "C"
{
    #include "utils/hw/pigpio.h"
}
#endif

namespace util
{
namespace hw
{

SPI_PIGPIO::SPI_PIGPIO()
{
}

SPI_PIGPIO::~SPI_PIGPIO()
{
    if (m_fd >= 0)
    {
        spiClose(m_fd);
    }
}

ts::Result<void> SPI_PIGPIO::init(uint32_t port, uint32_t channel, uint32_t speed, uint32_t mode)
{
    if (m_fd >= 0)
    {
        spiClose(m_fd);
        m_fd = -1;
    }

    if (port > 1)
    {
        return ts::Error("Only SPI ports 0 (main) & 1 (aux) are allowed");
    }
    if (port == 0 && channel > 1)
    {
        return ts::Error("For port 0, only SPI channels 0 & 1 are allowed");
    }
    if (port == 1 && channel > 2)
    {
        return ts::Error("For port 1, only SPI channels 0 & 1 & 2 are allowed");
    }
    if (mode > 3)
    {
        return ts::Error("Only SPI modes 0 to 3 are allowed");
    }

    m_channel = channel;
    m_speed = speed;
    m_mode = mode;

#ifdef RASPBERRY_PI

    uint32_t flags = 0;
    if (port == 1)
    {
        flags |= 1 << 8;
    }
    flags |= mode & 0x3;

    m_fd = spiOpen(channel, speed, flags);
    if (m_fd < 0)
    {
        return ts::Error("Error opening SPI port " + std::to_string(port) +
                         ", channel " + std::to_string(channel) +
                         ", speed " + std::to_string(speed) +
                         ", mode " + std::to_string(mode) +
                         ": "  + std::to_string(m_fd));
    }

#endif

    return ts::success;
}

bool SPI_PIGPIO::do_transfer(void const* tx_data, void* rx_data, size_t size, uint32_t speed)
{
    QASSERT(size > 0);
    if (size == 0)
    {
        return false;
    }

#ifdef RASPBERRY_PI
    int result = 0;
    if (tx_data && rx_data)
    {
        result = spiXfer(m_fd, (char*)tx_data, (char*)rx_data, size);
    }
    else if (tx_data)
    {
        result = spiWrite(m_fd, (char*)tx_data, size);
    }
    else if (rx_data)
    {
        result = spiRead(m_fd, (char*)rx_data, size);
    }

    if (result < 0)
    {
        return false;
    }

#endif

    return true;
}

bool SPI_PIGPIO::transfer(void const* tx_data, void* rx_data, size_t size, uint32_t speed)
{
    QLOG_TOPIC("spi_PIGPIO::transfer");

    if (m_is_used.exchange(true) == true)
    {
        QLOGE("SPI bus in use");
        return false;
    }

    bool res = do_transfer(tx_data, rx_data, size, speed);

    m_is_used = false;

    return res;
}

bool SPI_PIGPIO::transfers(Transfer const* transfers, size_t transfer_count, uint32_t speed)
{
    if (m_is_used.exchange(true) == true)
    {
        QLOGE("SPI bus in use");
        return false;
    }

    for (size_t i = 0; i < transfer_count; i++)
    {
        if (!do_transfer(transfers[i].tx_data, transfers[i].rx_data, transfers[i].size, speed))
        {
            m_is_used = false;
            return false;
        }
    }
    m_is_used = false;
    return true;
}

bool SPI_PIGPIO::transfer_register(uint8_t reg, void const* tx_data, void* rx_data, size_t size, uint32_t speed)
{
    QLOG_TOPIC("spi_PIGPIO::transfer_register");

    if (m_is_used.exchange(true) == true)
    {
        QLOGE("SPI bus in use");
        return false;
    }

    m_tx_buffer.resize(size + 1);
    m_tx_buffer[0] = reg;
    std::copy(reinterpret_cast<uint8_t const*>(tx_data), reinterpret_cast<uint8_t const*>(tx_data) + size, m_tx_buffer.begin() + 1);

    m_rx_buffer.resize(size + 1);
    if (!do_transfer(m_tx_buffer.data(), m_rx_buffer.data(), size + 1, speed))
    {
        m_is_used = false;
        return false;
    }

    std::copy(m_rx_buffer.begin() + 1, m_rx_buffer.end(), reinterpret_cast<uint8_t*>(rx_data));

    m_is_used = false;
    return true;
}

}
}
