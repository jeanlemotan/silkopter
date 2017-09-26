#include "SPI_Dev.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>


namespace util
{
namespace hw
{


SPI_Dev::SPI_Dev()
{
    memset(m_spi_transfers.data(), 0, sizeof(spi_ioc_transfer) * m_spi_transfers.size());
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
        return ts::Error("Can't open '" + device + "': " + strerror(errno));
    }

    int bits = 8;
    int ret = ioctl(m_fd, SPI_IOC_WR_BITS_PER_WORD, &bits);
    if (ret == -1)
    {
        ::close(m_fd);
        m_fd = -1;
        return ts::Error("Can't set bits per word");
    }

    ret = ioctl(m_fd, SPI_IOC_WR_MAX_SPEED_HZ, &speed);
    if (ret == -1)
    {
        ::close(m_fd);
        m_fd = -1;
        return ts::Error("Can't set speed");
    }

    int actual_speed = 0;
    ret = ioctl(m_fd, SPI_IOC_RD_MAX_SPEED_HZ, &actual_speed);
    if (ret == -1)
    {
        ::close(m_fd);
        m_fd = -1;
        return ts::Error("Can't set speed");
    }
    QLOGI("Requested speed: {}, actual speed: {}", speed, actual_speed);

    m_device = device;
    m_speed = speed;
    return ts::success;
}

bool SPI_Dev::do_transfer(void const* tx_data, void* rx_data, size_t size, uint32_t speed)
{
    QASSERT(m_fd >= 0 && size > 0);
    if (m_fd < 0 || size == 0)
    {
        return false;
    }

    m_spi_transfers[0].tx_buf = (unsigned long)tx_data;
    m_spi_transfers[0].rx_buf = (unsigned long)rx_data;
    m_spi_transfers[0].len = size;
    m_spi_transfers[0].speed_hz = speed ? speed : m_speed;
    m_spi_transfers[0].bits_per_word = 8;
    m_spi_transfers[0].delay_usecs = 0;
    m_spi_transfers[0].cs_change = 0;

    int status = ioctl(m_fd, SPI_IOC_MESSAGE(1), m_spi_transfers.data());
    if (status < 0)
    {
        QLOGW("transfer failed: {}", strerror(errno));
        return false;
    }

    return true;
}

bool SPI_Dev::transfer(void const* tx_data, void* rx_data, size_t size, uint32_t speed)
{
    QLOG_TOPIC("SPI_Dev::transfer");

    if (m_is_used.exchange(true) == true)
    {
        QLOGE("SPI bus in use");
        return false;
    }

    bool res = do_transfer(tx_data, rx_data, size, speed);

    m_is_used = false;

    return res;
}

bool SPI_Dev::transfers(Transfer const* transfers, size_t transfer_count, uint32_t speed)
{
    if (m_is_used.exchange(true) == true)
    {
        QLOGE("SPI bus in use");
        return false;
    }

    if (transfer_count > 256)
    {
        for (size_t i = 0; i < transfer_count; i++)
        {
            if (!do_transfer(transfers[i].tx_data, transfers[i].rx_data, transfers[i].size, speed))
            {
                m_is_used = false;
                return false;
            }
        }
    }
    else
    {
        QASSERT(m_fd >= 0);
        if (m_fd < 0)
        {
            m_is_used = false;
            return false;
        }

        for (size_t i = 0; i < transfer_count; i++)
        {
            m_spi_transfers[i].tx_buf = (unsigned long)transfers[i].tx_data;
            m_spi_transfers[i].rx_buf = (unsigned long)transfers[i].rx_data;
            m_spi_transfers[i].len = transfers[i].size;
            m_spi_transfers[i].speed_hz = speed ? speed : m_speed;
            m_spi_transfers[i].bits_per_word = 8;
            m_spi_transfers[i].delay_usecs = std::chrono::duration_cast<std::chrono::microseconds>(transfers[i].delay).count();
            m_spi_transfers[i].cs_change = i + 1 < transfer_count ? 1 : 0;
        }

        int status = ioctl(m_fd, SPI_IOC_MESSAGE(transfer_count), m_spi_transfers.data());
        if (status < 0)
        {
            QLOGW("transfer failed: {}", strerror(errno));
            m_is_used = false;
            return false;
        }
    }

    m_is_used = false;
    return true;
}

bool SPI_Dev::transfer_register(uint8_t reg, void const* tx_data, void* rx_data, size_t size, uint32_t speed)
{
    QLOG_TOPIC("SPI_Dev::transfer_register");

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
