#include "UART_Dev.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>

namespace util
{
namespace hw
{

UART_Dev::UART_Dev()
{
}

UART_Dev::~UART_Dev()
{
    close();
}

ts::Result<void> UART_Dev::init(std::string const& device, Baud baud)
{
    int baud_id = -1;
    switch (baud)
    {
    case Baud::_9600: baud_id = B9600; break;
    case Baud::_19200: baud_id = B19200; break;
    case Baud::_38400: baud_id = B38400; break;
    case Baud::_57600: baud_id = B57600; break;
    case Baud::_115200: baud_id = B115200; break;
    case Baud::_230400: baud_id = B230400; break;
    }
    if (baud_id < 0)
    {
        return ts::Error("Invalid baud requested");
    }

    m_device = device;
    m_fd = ::open(device.c_str(), O_RDWR | O_NOCTTY);
    if (m_fd < 0)
    {
        return ts::Error("Can't open " + device + ": " + strerror(errno));
    }

    struct termios options;
    tcgetattr(m_fd, &options);
    cfsetispeed(&options, baud_id);						// Set baud rate
    cfsetospeed(&options, baud_id);

    cfmakeraw(&options);

//    options.c_iflag &= ~IGNBRK;         // disable break processing
//    options.c_lflag = 0;                // no signaling chars, no echo, no canonical processing
//    options.c_oflag = 0;                // no remapping, no delays
    options.c_cc[VMIN]  = 0;            // read doesn't block
    options.c_cc[VTIME] = 0;            // 0 seconds read timeout

    tcflush(m_fd, TCIFLUSH);
    tcsetattr(m_fd, TCSANOW, &options);

    return ts::success;
}

void UART_Dev::close()
{
    QLOG_TOPIC("UART_Dev::clone");

    if (m_fd)
    {
        ::close(m_fd);
        m_fd = -1;
    }
}

size_t UART_Dev::read(uint8_t* data, size_t max_size)
{
    QLOG_TOPIC("UART_Dev::read");
    QASSERT(m_fd >= 0);

    if (m_is_used.exchange(true) == true)
    {
        QLOGE("SPI bus in use");
        return false;
    }

    int res = ::read(m_fd, data, max_size);
    if (res < 0)
    {
        if (errno != EWOULDBLOCK && errno != EAGAIN)
        {
            QLOGE("error reading from {}: {}", m_device, strerror(errno));
        }
        m_is_used = false;
        return 0;
    }
    m_is_used = false;
    return res;
}
bool UART_Dev::write(uint8_t const* data, size_t size)
{
    QLOG_TOPIC("UART_Dev::write");
    QASSERT(m_fd >= 0);

    if (m_is_used.exchange(true) == true)
    {
        QLOGE("SPI bus in use");
        return false;
    }

    int res = ::write(m_fd, data, size);
    if (res < 0)
    {
        QLOGE("error writing to {}: {}", m_device, strerror(errno));
        m_is_used = false;
        return false;
    }
    m_is_used = false;
    return static_cast<size_t>(res) == size;
}

void UART_Dev::send_break()
{
    if (m_is_used.exchange(true) == true)
    {
        QLOGE("SPI bus in use");
        return;
    }

    tcsendbreak(m_fd, 1);

    m_is_used = false;
}

}
}
