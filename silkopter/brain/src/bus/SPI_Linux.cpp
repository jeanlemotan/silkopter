#include "BrainStdAfx.h"
#include "bus/SPI_Linux.h"

namespace silk
{
namespace node
{
namespace bus
{

SPI_Linux::SPI_Linux()
{
}

SPI_Linux::~SPI_Linux()
{
    close();
}

auto SPI_Linux::open(q::String const& device, size_t mode) -> bool
{
    close();

    QLOG_TOPIC("bus_spi_pi");

    std::lock_guard<SPI_Linux> lg(*this);

    m_device = device;
    m_fd = ::open(device.c_str(), O_RDWR);
    if (m_fd < 0)
    {
        QLOGE("can't open {}: {}", device, strerror(errno));
        return false;
    }
    return true;
}
void SPI_Linux::close()
{
    QLOG_TOPIC("bus_spi_pi");

    if (m_fd)
    {
        std::lock_guard<SPI_Linux> lg(*this);

        ::close(m_fd);
        m_fd = -1;
    }
}

void SPI_Linux::lock()
{
    m_mutex.lock();
}

auto SPI_Linux::try_lock() -> bool
{
    return m_mutex.try_lock();
}
void SPI_Linux::unlock()
{
    m_mutex.unlock();
}

auto SPI_Linux::read(uint8_t* data, size_t size) -> bool
{
    QLOG_TOPIC("bus_spi_pi");
    QASSERT(m_fd >= 0);

    std::lock_guard<SPI_Linux> lg(*this);

    return false;
}
auto SPI_Linux::write(uint8_t const* data, size_t size) -> bool
{
    QLOG_TOPIC("bus_spi_pi");
    QASSERT(m_fd >= 0);

    std::lock_guard<SPI_Linux> lg(*this);

    return false;
}

auto SPI_Linux::read_register(uint8_t reg, uint8_t* data, size_t size) -> bool
{
    QLOG_TOPIC("bus_spi_pi");
    QASSERT(m_fd >= 0);

    std::lock_guard<SPI_Linux> lg(*this);

    return false;
}
auto SPI_Linux::write_register(uint8_t reg, uint8_t const* data, size_t size) -> bool
{
    QLOG_TOPIC("bus_spi_pi");
    QASSERT(m_fd >= 0);

    std::lock_guard<SPI_Linux> lg(*this);

    return false;
}


}
}
}
