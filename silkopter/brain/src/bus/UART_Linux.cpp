#include "BrainStdAfx.h"
#include "bus/UART_Linux.h"

#include <termios.h>

#include "uav.def.h"

namespace silk
{
namespace bus
{

UART_Linux::UART_Linux()
    : m_descriptor(new uav::UART_Linux_Descriptor())
{
}

UART_Linux::~UART_Linux()
{
    close();
}

bool UART_Linux::init(uav::IBus_Descriptor const& descriptor)
{
    auto specialized = dynamic_cast<uav::UART_Linux_Descriptor const*>(&descriptor);
    if (!specialized)
    {
        QLOGE("Wrong descriptor type");
        return false;
    }
    *m_descriptor = *specialized;

    int baud_id = -1;
    switch (m_descriptor->get_baud())
    {
    case uav::UART_Linux_Descriptor::baud_t::_9600: baud_id = B9600; break;
    case uav::UART_Linux_Descriptor::baud_t::_19200: baud_id = B19200; break;
    case uav::UART_Linux_Descriptor::baud_t::_38400: baud_id = B38400; break;
    case uav::UART_Linux_Descriptor::baud_t::_57600: baud_id = B57600; break;
    case uav::UART_Linux_Descriptor::baud_t::_115200: baud_id = B115200; break;
    case uav::UART_Linux_Descriptor::baud_t::_230400: baud_id = B230400; break;
    }
    if (baud_id < 0)
    {
        QLOGE("Invalid baud requested");
        return false;
    }

    if (!init(m_descriptor->get_dev(), baud_id))
    {
        return false;
    }

    return true;
}

std::shared_ptr<const uav::IBus_Descriptor> UART_Linux::get_descriptor() const
{
    return m_descriptor;
}

auto UART_Linux::init(std::string const& dev, int baud_id) -> bool
{
    close();

    std::lock_guard<UART_Linux> lg(*this);

    m_fd = ::open(dev.c_str(), O_RDWR | O_NOCTTY | O_NDELAY | O_NONBLOCK);
    if (m_fd < 0)
    {
        QLOGE("can't open {}: {}", dev, strerror(errno));
        return false;
    }

    m_dev = dev;

    struct termios options;
    tcgetattr(m_fd, &options);
    cfsetispeed(&options, baud_id);						// Set baud rate
    cfsetospeed(&options, baud_id);

    cfmakeraw(&options);

    tcflush(m_fd, TCIFLUSH);
    tcsetattr(m_fd, TCSANOW, &options);

    return true;
}

void UART_Linux::close()
{
    QLOG_TOPIC("uart_linux::clone");

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
    QLOG_TOPIC("uart_linux::read");
    QASSERT(m_fd >= 0);

    std::lock_guard<UART_Linux> lg(*this);

    auto res = ::read(m_fd, data, max_size);
    if (res < 0)
    {
        if (errno != EWOULDBLOCK && errno != EAGAIN)
        {
            QLOGE("error reading from {}: {}", m_dev, strerror(errno));
        }
        return 0;
    }
    return res;
}
auto UART_Linux::write(uint8_t const* data, size_t size) -> bool
{
    QLOG_TOPIC("uart_linux::write");
    QASSERT(m_fd >= 0);

    std::lock_guard<UART_Linux> lg(*this);

    auto res = ::write(m_fd, data, size);
    if (res < 0)
    {
        QLOGE("error writing to {}: {}", m_dev, strerror(errno));
        return false;
    }
    return static_cast<size_t>(res) == size;
}

void UART_Linux::send_break()
{
    tcsendbreak(m_fd, 1);
}

}
}
