#include "BrainStdAfx.h"
#include "buses/SPI_Pi.h"

namespace silk
{
namespace buses
{

SPI_Pi::SPI_Pi(q::String const& name)
    : m_name(name)
{
}

SPI_Pi::~SPI_Pi()
{
    close();
}

auto SPI_Pi::get_name() const -> q::String const&
{
    return m_name;
}

auto SPI_Pi::open(q::String const& device, size_t mode) -> bool
{
    close();

    QLOG_TOPIC("bus_spi_pi");

    std::lock_guard<SPI_Pi> lg(*this);

    m_device = device;
    m_fd = ::open(device.c_str(), O_RDWR);
    if (m_fd < 0)
    {
        QLOGE("can't open {}: {}", device, strerror(errno));
        return false;
    }
    return true;
}
void SPI_Pi::close()
{
    QLOG_TOPIC("bus_spi_pi");

    if (m_fd)
    {
        std::lock_guard<SPI_Pi> lg(*this);

        ::close(m_fd);
        m_fd = -1;
    }
}

void SPI_Pi::lock()
{
    m_mutex.lock();
}

auto SPI_Pi::try_lock() -> bool
{
    return m_mutex.try_lock();
}
void SPI_Pi::unlock()
{
    m_mutex.unlock();
}

auto SPI_Pi::read(uint8_t* data, size_t size) -> bool
{
    QLOG_TOPIC("bus_spi_pi");
    QASSERT(m_fd >= 0);

    std::lock_guard<SPI_Pi> lg(*this);

    return false;
}
auto SPI_Pi::write(uint8_t const* data, size_t size) -> bool
{
    QLOG_TOPIC("bus_spi_pi");
    QASSERT(m_fd >= 0);

    std::lock_guard<SPI_Pi> lg(*this);

    return false;
}

auto SPI_Pi::read_register(uint8_t reg, uint8_t* data, size_t size) -> bool
{
    QLOG_TOPIC("bus_spi_pi");
    QASSERT(m_fd >= 0);

    std::lock_guard<SPI_Pi> lg(*this);

    return false;
}
auto SPI_Pi::write_register(uint8_t reg, uint8_t const* data, size_t size) -> bool
{
    QLOG_TOPIC("bus_spi_pi");
    QASSERT(m_fd >= 0);

    std::lock_guard<SPI_Pi> lg(*this);

    return false;
}


}
}
