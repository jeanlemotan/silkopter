#include "BrainStdAfx.h"
#include "bus/SPI_Linux.h"

#include "sz_hal_buses.hpp"

namespace silk
{
namespace node
{
namespace bus
{

SPI_Linux::SPI_Linux(HAL& hal)
    : m_hal(hal)
{
}

SPI_Linux::~SPI_Linux()
{
    close();
}

auto SPI_Linux::init(rapidjson::Value const& json) -> bool
{
    sz::SPI_Linux sz;
    autojsoncxx::error::ErrorStack result;
    if (!autojsoncxx::from_value(sz, json, result))
    {
        std::ostringstream ss;
        ss << result;
        QLOGE("Cannot deserialize SPI_Linux data: {}", ss.str());
        return false;
    }
    Init_Params params;
    params.name = sz.name;
    params.dev = sz.dev;
    params.mode = sz.mode;
    return init(params);
}
auto SPI_Linux::init(Init_Params const& params) -> bool
{
    close();

    QLOG_TOPIC("bus_spi_linux::init");

    std::lock_guard<SPI_Linux> lg(*this);

    m_params = params;
    m_fd = ::open(params.dev.c_str(), O_RDWR);
    if (m_fd < 0)
    {
        QLOGE("can't open {}: {}", params.dev, strerror(errno));
        return false;
    }

    if (!m_params.name.empty())
    {
        if (!m_hal.get_buses().add(*this))
        {
            return false;
        }
    }

    return true;
}

auto SPI_Linux::get_name() const -> std::string const&
{
    return m_params.name;
}

void SPI_Linux::close()
{
    QLOG_TOPIC("bus_spi_linux::close");

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
    QLOG_TOPIC("bus_spi_linux::read");
    QASSERT(m_fd >= 0);

    std::lock_guard<SPI_Linux> lg(*this);

    return false;
}
auto SPI_Linux::write(uint8_t const* data, size_t size) -> bool
{
    QLOG_TOPIC("bus_spi_linux::write");
    QASSERT(m_fd >= 0);

    std::lock_guard<SPI_Linux> lg(*this);

    return false;
}

auto SPI_Linux::read_register(uint8_t reg, uint8_t* data, size_t size) -> bool
{
    QLOG_TOPIC("bus_spi_linux::read_register");
    QASSERT(m_fd >= 0);

    std::lock_guard<SPI_Linux> lg(*this);

    return false;
}
auto SPI_Linux::write_register(uint8_t reg, uint8_t const* data, size_t size) -> bool
{
    QLOG_TOPIC("bus_spi_linux::write_register");
    QASSERT(m_fd >= 0);

    std::lock_guard<SPI_Linux> lg(*this);

    return false;
}


}
}
}
