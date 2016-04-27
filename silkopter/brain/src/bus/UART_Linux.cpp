#include "BrainStdAfx.h"
#include "bus/UART_Linux.h"
#include "def_lang/Mapper.h"
#include "def_lang/Type_System.h"

#include <termios.h>

namespace silk
{
namespace bus
{

UART_Linux::UART_Linux(ts::Type_System const& ts)
{
    m_descriptor = ts.create_value("::silk::UART_Linux_Descriptor");
    if (!m_descriptor)
    {
        QLOGE("Cannot create descriptor value");
    }
}

UART_Linux::~UART_Linux()
{
    close();
}

bool UART_Linux::init(std::shared_ptr<ts::IValue> descriptor)
{
    if (!descriptor || descriptor->get_type() != m_descriptor->get_type())
    {
        QLOGE("Bad descriptor!");
        return false;
    }
    std::string dev;
    uint32_t baud = 0;

    auto result = ts::mapper::get(*descriptor, "dev", dev) &
                    ts::mapper::get(*descriptor, "baud", baud);
    if (result != ts::success)
    {
        QLOGE("{}", result.error().what());
        return false;
    }

    if (!init(dev, baud))
    {
        return false;
    }

    result = m_descriptor->copy_assign(*descriptor);
    QASSERT(result == ts::success);
    return true;
}

std::shared_ptr<const ts::IValue> UART_Linux::get_descriptor() const
{
    return m_descriptor;
}

auto UART_Linux::init(std::string const& dev, uint32_t baud) -> bool
{
    close();

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

    m_fd = ::open(dev.c_str(), O_RDWR | O_NOCTTY | O_NDELAY | O_NONBLOCK);
    if (m_fd < 0)
    {
        QLOGE("can't open {}: {}", dev, strerror(errno));
        return false;
    }

    m_dev = dev;

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
