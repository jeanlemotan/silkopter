#include "SPI_Dev.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>

#include <linux/spi/spidev.h>

namespace util
{
namespace hw
{

SPI_Dev::SPI_Dev()
{
}

SPI_Dev::~SPI_Dev()
{
    if (m_fd >= 0)
    {
        ::close(m_fd);
        m_fd = -1;
    }
}

ts::Result<void> SPI_Dev::init(std::string const& device, uint32_t speed)
{
    QLOG_TOPIC("SPI_Dev::init");

    if (speed == 0)
    {
        return ts::Error("Invalid speed " + std::to_string(speed));
    }

    QASSERT(m_fd < 0);
    m_fd = ::open(device.c_str(), O_RDWR);
    if (m_fd < 0)
    {
        return ts::Error("Can't open " + device + ": " + strerror(errno));
    }

    m_device = device;
    m_speed = speed;
    return ts::success;
}

bool SPI_Dev::do_transfer(void const* tx_data, void* rx_data, size_t size, uint32_t speed) const
{
    QASSERT(m_fd >= 0 && size > 0);
    if (m_fd < 0 || size == 0)
    {
        return false;
    }

    spi_ioc_transfer spi_transfer;
    memset(&spi_transfer, 0, sizeof(spi_ioc_transfer));

    spi_transfer.tx_buf = (unsigned long)tx_data;
    spi_transfer.rx_buf = (unsigned long)rx_data;
    spi_transfer.len = size;
    spi_transfer.speed_hz = speed ? speed : m_speed;
    spi_transfer.bits_per_word = 8;
    spi_transfer.delay_usecs = 0;

    int status = ioctl(m_fd, SPI_IOC_MESSAGE(1), &spi_transfer);
    if (status < 0)
    {
        QLOGW("transfer failed: {}", strerror(errno));
        return false;
    }

    return true;
}

bool SPI_Dev::transfer(void const* tx_data, void* rx_data, size_t size, uint32_t speed) const
{
    QLOG_TOPIC("SPI_Dev::transfer");
    return do_transfer(tx_data, rx_data, size, speed);
}

bool SPI_Dev::transfer_register(uint8_t reg, void const* tx_data, void* rx_data, size_t size, uint32_t speed) const
{
    QLOG_TOPIC("SPI_Dev::transfer_register");

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
