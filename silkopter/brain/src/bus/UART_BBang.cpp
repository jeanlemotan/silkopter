#include "BrainStdAfx.h"
#include "bus/UART_BBang.h"

#ifdef RASPBERRY_PI
extern "C"
{
    #include "hw/pigpio.h"
}
#endif

#include "uav.def.h"

namespace silk
{
namespace bus
{

UART_BBang::UART_BBang()
    : m_descriptor(new uav::UART_BBang_Descriptor())
{
}

UART_BBang::~UART_BBang()
{
    close();
}

bool UART_BBang::init(uav::IBus_Descriptor const& descriptor)
{
    auto specialized = dynamic_cast<uav::UART_BBang_Descriptor const*>(&descriptor);
    if (!specialized)
    {
        QLOGE("Wrong descriptor type");
        return false;
    }
    *m_descriptor = *specialized;

    if (!init(specialized->get_rx_pin(), specialized->get_baud(), specialized->get_invert()))
    {
        return false;
    }

    return true;
}

std::shared_ptr<const uav::IBus_Descriptor> UART_BBang::get_descriptor() const
{
    return m_descriptor;
}

auto UART_BBang::init(uint32_t rx_pin, uint32_t baud, bool invert) -> bool
{
    close();

    std::lock_guard<UART_BBang> lg(*this);

#if defined (RASPBERRY_PI)

    int res = gpioSetMode(rx_pin, PI_INPUT);
    if (res != 0)
    {
        QLOGE("can't change bit-banging rx pin to input {}: {}", rx_pin, res);
        return false;
    }

    res = gpioSerialReadOpen(rx_pin, baud, 8);
    if (res != 0)
    {
        QLOGE("can't open bit-banging rx pin {}: {}", rx_pin, res);
        return false;
    }

    res = gpioSerialReadInvert(rx_pin, invert ? PI_BB_SER_INVERT : PI_BB_SER_NORMAL);
    if (res != 0)
    {
        QLOGE("can't open bit-banging invert property on pin {}: {}", rx_pin, res);
        return false;
    }
#endif

    m_is_initialized = true;

    return true;
}

void UART_BBang::close()
{
    QLOG_TOPIC("uart_BBang::clone");

#if defined (RASPBERRY_PI)

    if (m_is_initialized)
    {
        int res = gpioSerialReadClose(m_descriptor->get_rx_pin());
        QASSERT(res == 0);
    }

#endif

    m_is_initialized = false;
}

void UART_BBang::lock()
{
    m_mutex.lock();
}

auto UART_BBang::try_lock() -> bool
{
    return m_mutex.try_lock();
}
void UART_BBang::unlock()
{
    m_mutex.unlock();
}

auto UART_BBang::read(uint8_t* data, size_t max_size) -> size_t
{
    QLOG_TOPIC("uart_BBang::read");

    std::lock_guard<UART_BBang> lg(*this);

#if defined (RASPBERRY_PI)
    int res = gpioSerialRead(m_descriptor->get_rx_pin(), data, max_size);
    if (res < 0)
    {
        QLOGE("error reading from bit-banged rx pin {}: {}", m_descriptor->get_rx_pin(), res);
        return 0;
    }
    return res;
#else
    return 0;
#endif
}
auto UART_BBang::write(uint8_t const*, size_t) -> bool
{
    QLOG_TOPIC("uart_BBang::write");
    QLOGE("not supported");
    return false;
}

void UART_BBang::send_break()
{
    QLOG_TOPIC("uart_bbang::send_break");
    QLOGE("not supported");
}

}
}
