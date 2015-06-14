#include "BrainStdAfx.h"
#include "bus/SPI_Linux.h"

#include <linux/spi/spidev.h>
#define READ_FLAG   0x80

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
    *m_init_params = sz;
    return init();
}
auto SPI_Linux::init() -> bool
{
    close();

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
    QLOG_TOPIC("spi_linux::close");

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

auto SPI_Linux::transfer(uint8_t const* tx_data, uint8_t* rx_data, size_t size) -> bool
{
    QLOG_TOPIC("spi_linux::transfer");
    QASSERT(m_fd >= 0 && size > 0);

    spi_ioc_transfer spi_transfer;
    memset(&spi_transfer, 0, sizeof(spi_ioc_transfer));

    m_tx_buffer.resize(size);
    spi_transfer.tx_buf = (unsigned long)tx_data;
    spi_transfer.rx_buf = (unsigned long)rx_data;
    spi_transfer.len = size;
    spi_transfer.speed_hz = m_init_params->speed;
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

auto SPI_Linux::read(uint8_t* data, size_t size) -> bool
{
    QLOG_TOPIC("spi_linux::read");

    std::lock_guard<SPI_Linux> lg(*this);
    return transfer(nullptr, data, size);
}
auto SPI_Linux::write(uint8_t const* data, size_t size) -> bool
{
    QLOG_TOPIC("spi_linux::write");

    std::lock_guard<SPI_Linux> lg(*this);
    return transfer(data, nullptr, size);
}

auto SPI_Linux::read_register(uint8_t reg, uint8_t* data, size_t size) -> bool
{
    QLOG_TOPIC("spi_linux::read_register");
    QASSERT(m_fd >= 0);

    std::lock_guard<SPI_Linux> lg(*this);

    m_tx_buffer.clear();
    m_tx_buffer.resize(size + 1);
    m_tx_buffer[0] = reg | READ_FLAG;

    m_rx_buffer.resize(size + 1);
    if (!transfer(m_tx_buffer.data(), m_rx_buffer.data(), size + 1))
    {
        return false;
    }

    std::copy(m_rx_buffer.begin() + 1, m_rx_buffer.end(), data);
    return true;
}
auto SPI_Linux::write_register(uint8_t reg, uint8_t const* data, size_t size) -> bool
{
    QLOG_TOPIC("spi_linux::write_register");
    QASSERT(m_fd >= 0);

    std::lock_guard<SPI_Linux> lg(*this);

    m_tx_buffer.clear();
    m_tx_buffer.resize(size + 1);
    m_tx_buffer[0] = reg;
    std::copy(data, data + size, m_tx_buffer.begin() + 1);

    m_rx_buffer.resize(size + 1);
    return transfer(m_tx_buffer.data(), m_rx_buffer.data(), size + 1);
}

auto SPI_Linux::set_config(rapidjson::Value const& json) -> bool
{
    QLOG_TOPIC("spi_linux::set_config");

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

auto SPI_Linux::get_init_params() const -> rapidjson::Document
{
    rapidjson::Document json;
    autojsoncxx::to_document(*m_init_params, json);
    return std::move(json);
}


}
}
}
