#include "UART_BB.h"

#ifdef RASPBERRY_PI
extern "C"
{
    #include "utils/hw/pigpio.h"
}
#endif

namespace util
{
namespace hw
{

UART_BB::UART_BB()
{
}

UART_BB::~UART_BB()
{
    close();
}

ts::Result<void> UART_BB::init(uint32_t rx_gpio, uint32_t baud, bool inverted_signalling)
{
    close();

    std::lock_guard<UART_BB> lg(*this);

#if defined (RASPBERRY_PI)
    int res = gpioSetMode(rx_gpio, PI_INPUT);
    if (res != 0)
    {
        return ts::Error("can't change bit-banging rx gpio " + std::to_string(rx_gpio) + " to input: " + std::to_string(res));
    }

    res = gpioSerialReadOpen(rx_gpio, baud, 8);
    if (res != 0)
    {
        return ts::Error("can't open bit-banging rx gpio " + std::to_string(rx_gpio) + ": " + std::to_string(res));
    }

    res = gpioSerialReadInvert(rx_gpio, inverted_signalling ? PI_BB_SER_INVERT : PI_BB_SER_NORMAL);
    if (res != 0)
    {
        return ts::Error("can't change bit-banging rx gpio " + std::to_string(rx_gpio) + " invert property: " + std::to_string(res));
    }
#endif

    m_rx_gpio = rx_gpio;
    m_is_initialized = true;

    return ts::success;
}

void UART_BB::close()
{
    QLOG_TOPIC("UART_BB::clone");

#if defined (RASPBERRY_PI)
    if (m_is_initialized)
    {
        int res = gpioSerialReadClose(m_rx_gpio);
        QASSERT(res == 0);
    }
#endif

    m_is_initialized = false;
}

void UART_BB::lock()
{
    m_mutex.lock();
}

bool UART_BB::try_lock()
{
    return m_mutex.try_lock();
}
void UART_BB::unlock()
{
    m_mutex.unlock();
}

size_t UART_BB::read(uint8_t* data, size_t max_size)
{
    QLOG_TOPIC("UART_BB::read");

    std::lock_guard<UART_BB> lg(*this);

#if defined (RASPBERRY_PI)
    int res = gpioSerialRead(m_rx_gpio, data, max_size);
    if (res < 0)
    {
        QLOGE("error reading from bit-banged rx gpio {}: {}", m_rx_gpio, res);
        return 0;
    }
    return res;
#else
    return 0;
#endif
}
bool UART_BB::write(uint8_t const*, size_t)
{
    QLOG_TOPIC("UART_BB::write");
    QLOGE("not supported");
    return false;
}

void UART_BB::send_break()
{
    QLOG_TOPIC("UART_BB::send_break");
    QLOGE("not supported");
}

}
}
