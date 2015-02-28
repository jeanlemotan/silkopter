#include "BrainStdAfx.h"
#include "bus/UART_Linux.h"

#include "sz_UART_Linux.hpp"

#include <termios.h>

namespace silk
{
namespace node
{
namespace bus
{

UART_Linux::UART_Linux(HAL& hal)
    : m_hal(hal)
    , m_init_params(new sz::UART_Linux::Init_Params())
    , m_config(new sz::UART_Linux::Config())
{
}

UART_Linux::~UART_Linux()
{
    close();
}

auto UART_Linux::init(rapidjson::Value const& init_params, rapidjson::Value const& config) -> bool
{
    QLOG_TOPIC("uart_linux::init");

    sz::UART_Linux::Init_Params sz;
    autojsoncxx::error::ErrorStack result;
    if (!autojsoncxx::from_value(sz, init_params, result))
    {
        std::ostringstream ss;
        ss << result;
        QLOGE("Cannot deserialize UART_Linux data: {}", ss.str());
        return false;
    }
    *m_init_params = sz;
    return init() && set_config(config);
}
auto UART_Linux::init() -> bool
{
    close();

    QLOG_TOPIC("bus_uart_linux::init");

    int b = -1;
    switch (m_init_params->baud)
    {
    case 9600: b = B9600; break;
    case 19200: b = B19200; break;
    case 38400: b = B38400; break;
    case 57600: b = B57600; break;
    case 115200: b = B115200; break;
    case 230400: b = B230400; break;
    }

    if (b < 0)
    {
        QLOGE("Invalid baud requested: {}", m_init_params->baud);
        return false;
    }

    std::lock_guard<UART_Linux> lg(*this);

    m_fd = ::open(m_init_params->dev.c_str(), O_RDWR | O_NOCTTY | O_NDELAY | O_NONBLOCK);
    if (m_fd < 0)
    {
        QLOGE("can't open {}: {}", m_init_params->dev, strerror(errno));
        return false;
    }

    struct termios options;
    tcgetattr(m_fd, &options);
    cfsetispeed(&options, b);						// Set baud rate
    cfsetospeed(&options, b);

    cfmakeraw(&options);

    tcflush(m_fd, TCIFLUSH);
    tcsetattr(m_fd, TCSANOW, &options);

    if (!m_init_params->name.empty())
    {
        if (!m_hal.get_buses().add(*this))
        {
            return false;
        }
    }

    return true;
}

auto UART_Linux::get_name() const -> std::string const&
{
    return m_init_params->name;
}

void UART_Linux::close()
{
    QLOG_TOPIC("bus_uart_linux::clone");

    if (m_fd)
    {
        std::lock_guard<UART_Linux> lg(*this);

        ::close(m_fd);
        m_fd = -1;
    }
}

void UART_Linux::lock()
{
    m_mutex.lock();
}

auto UART_Linux::try_lock() -> bool
{
    return m_mutex.try_lock();
}
void UART_Linux::unlock()
{
    m_mutex.unlock();
}

auto UART_Linux::read(uint8_t* data, size_t max_size) -> size_t
{
    QLOG_TOPIC("bus_uart_linux::read");
    QASSERT(m_fd >= 0);

    std::lock_guard<UART_Linux> lg(*this);

    auto res = ::read(m_fd, data, max_size);
    if (res < 0)
    {
//        QLOGE("error reading from {}: {}", m_params.dev, strerror(errno));
        return 0;
    }
    return res;
}
auto UART_Linux::write(uint8_t const* data, size_t size) -> bool
{
    QLOG_TOPIC("bus_uart_linux::write");
    QASSERT(m_fd >= 0);

    std::lock_guard<UART_Linux> lg(*this);

    auto res = ::write(m_fd, data, size);
    if (res < 0)
    {
        QLOGE("error writing to {}: {}", m_init_params->dev, strerror(errno));
        return false;
    }
    return static_cast<size_t>(res) == size;
}

auto UART_Linux::set_config(rapidjson::Value const& json) -> bool
{
    sz::UART_Linux::Config sz;
    autojsoncxx::error::ErrorStack result;
    if (!autojsoncxx::from_value(sz, json, result))
    {
        std::ostringstream ss;
        ss << result;
        QLOGE("Cannot deserialize UART_Linux config data: {}", ss.str());
        return false;
    }

    *m_config = sz;
    return true;
}
auto UART_Linux::get_config() -> rapidjson::Document
{
    rapidjson::Document json;
    autojsoncxx::to_document(*m_config, json);
    return std::move(json);
}

auto UART_Linux::get_init_params() -> rapidjson::Document
{
    rapidjson::Document json;
    autojsoncxx::to_document(*m_init_params, json);
    return std::move(json);
}


}
}
}
