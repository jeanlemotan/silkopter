#include "BrainStdAfx.h"
#include "bus/UART_Linux.h"

#include <termios.h>

namespace silk
{
namespace bus
{

UART_Linux::UART_Linux(q::String const& name)
    : m_name(name)
{
}

UART_Linux::~UART_Linux()
{
    close();
}

auto UART_Linux::get_name() const -> q::String const&
{
    return m_name;
}

auto UART_Linux::open(q::String const& device, size_t baud) -> bool
{
    close();

    QLOG_TOPIC("bus_uart_pi");

    int b = -1;
    switch (baud)
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
        QLOGE("Invalid baud requested: {}", baud);
        return false;
    }

    std::lock_guard<UART_Linux> lg(*this);

    m_device = device;
    m_fd = ::open(device.c_str(), O_RDWR | O_NOCTTY | O_NDELAY);
    if (m_fd < 0)
    {
        QLOGE("can't open {}: {}", device, strerror(errno));
        return false;
    }

    struct termios options;
    tcgetattr(m_fd, &options);
    cfsetispeed(&options, b);						// Set baud rate
    cfsetospeed(&options, b);

    cfmakeraw(&options);

    tcflush(m_fd, TCIFLUSH);
    tcsetattr(m_fd, TCSANOW, &options);

    return true;
}
void UART_Linux::close()
{
    QLOG_TOPIC("bus_uart_pi");

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
    QLOG_TOPIC("bus_uart_pi");
    QASSERT(m_fd >= 0);

    std::lock_guard<UART_Linux> lg(*this);

    return ::read(m_fd, data, max_size);
}
auto UART_Linux::write(uint8_t const* data, size_t size) -> bool
{
    QLOG_TOPIC("bus_uart_pi");
    QASSERT(m_fd >= 0);

    std::lock_guard<UART_Linux> lg(*this);

    return ::write(m_fd, data, size) == size;
}


}
}
