#include "BrainStdAfx.h"
#include "bus/UART_Linux.h"

#include "sz_hal_buses.hpp"

#include <termios.h>

namespace silk
{
namespace node
{
namespace bus
{

UART_Linux::UART_Linux(HAL& hal)
    : m_hal(hal)
{
}

UART_Linux::~UART_Linux()
{
    close();
}

auto UART_Linux::init(rapidjson::Value const& json) -> bool
{
    sz::UART_Linux sz;
    autojsoncxx::error::ErrorStack result;
    if (!autojsoncxx::from_value(sz, json, result))
    {
        std::ostringstream ss;
        ss << result;
        QLOGE("Cannot deserialize UART_Linux data: {}", ss.str());
        return false;
    }
    Init_Params params;
    params.name = sz.name;
    params.dev = sz.dev;
    params.baud = sz.baud;
    return init(params);
}
auto UART_Linux::init(Init_Params const& params) -> bool
{
    close();

    QLOG_TOPIC("bus_uart_linux::init");

    m_params = params;

    int b = -1;
    switch (params.baud)
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
        QLOGE("Invalid baud requested: {}", params.baud);
        return false;
    }

    std::lock_guard<UART_Linux> lg(*this);

    m_fd = ::open(params.dev.c_str(), O_RDWR | O_NOCTTY | O_NDELAY | O_NONBLOCK);
    if (m_fd < 0)
    {
        QLOGE("can't open {}: {}", params.dev, strerror(errno));
        return false;
    }

    struct termios options;
    tcgetattr(m_fd, &options);
    cfsetispeed(&options, b);						// Set baud rate
    cfsetospeed(&options, b);

    cfmakeraw(&options);

    tcflush(m_fd, TCIFLUSH);
    tcsetattr(m_fd, TCSANOW, &options);

    if (!m_params.name.empty())
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
    return m_params.name;
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

    int res = ::read(m_fd, data, max_size);
    if (res < 0)
    {
//        QLOGE("error reading {}: {}", m_params.dev, strerror(errno));
        return 0;
    }
    return res;
}
auto UART_Linux::write(uint8_t const* data, size_t size) -> bool
{
    QLOG_TOPIC("bus_uart_linux::write");
    QASSERT(m_fd >= 0);

    std::lock_guard<UART_Linux> lg(*this);

    return ::write(m_fd, data, size) == size;
}


}
}
}
