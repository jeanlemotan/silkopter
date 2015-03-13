#include "BrainStdAfx.h"
#include "bus/SPI_Linux.h"

#include "sz_SPI_Linux.hpp"

namespace silk
{
namespace node
{
namespace bus
{

SPI_Linux::SPI_Linux()
    : m_init_params(new sz::SPI_Linux::Init_Params())
    , m_config(new sz::SPI_Linux::Config())
{
}

SPI_Linux::~SPI_Linux()
{
    close();
}

auto SPI_Linux::init(rapidjson::Value const& init_params) -> bool
{
    QLOG_TOPIC("spi_linux::init");

    sz::SPI_Linux::Init_Params sz;
    autojsoncxx::error::ErrorStack result;
    if (!autojsoncxx::from_value(sz, init_params, result))
    {
        std::ostringstream ss;
        ss << result;
        QLOGE("Cannot deserialize SPI_Linux data: {}", ss.str());
        return false;
    }
    jsonutil::clone_value(m_init_paramsj, init_params, m_init_paramsj.GetAllocator());
    *m_init_params = sz;
    return init();
}
auto SPI_Linux::init() -> bool
{
    close();

    QLOG_TOPIC("bus_spi_linux::init");

    std::lock_guard<SPI_Linux> lg(*this);

    m_fd = ::open(m_init_params->dev.c_str(), O_RDWR);
    if (m_fd < 0)
    {
        QLOGE("can't open {}: {}", m_init_params->dev, strerror(errno));
        return false;
    }

    return true;
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

auto SPI_Linux::set_config(rapidjson::Value const& json) -> bool
{
    sz::SPI_Linux::Config sz;
    autojsoncxx::error::ErrorStack result;
    if (!autojsoncxx::from_value(sz, json, result))
    {
        std::ostringstream ss;
        ss << result;
        QLOGE("Cannot deserialize SPI_Linux config data: {}", ss.str());
        return false;
    }

    *m_config = sz;
    return true;
}
auto SPI_Linux::get_config() const -> rapidjson::Document
{
    rapidjson::Document json;
    autojsoncxx::to_document(*m_config, json);
    return std::move(json);
}

auto SPI_Linux::get_init_params() const -> rapidjson::Document const&
{
    return m_init_paramsj;
}


}
}
}
