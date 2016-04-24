#include "BrainStdAfx.h"
#include "bus/UART_BBang.h"
#include "def_lang/Mapper.h"

#ifdef RASPBERRY_PI
extern "C"
{
    #include "hw/pigpio.h"
}
#endif

namespace silk
{
namespace bus
{

UART_BBang::UART_BBang(ts::IDeclaration_Scope const& scope)
{
    std::shared_ptr<const ts::IType> type = scope.find_specialized_symbol_by_path<const ts::IType>("::silk::UART_BBang_Descriptor");
    if (!type)
    {
        QLOGE("Cannot find descriptor type");
    }
    else
    {
        m_descriptor = type->create_value();
    }
}

UART_BBang::~UART_BBang()
{
    close();
}

bool UART_BBang::init(std::shared_ptr<ts::IValue> descriptor)
{
    if (!descriptor)
    {
        QLOGE("Null descriptor!");
        return false;
    }
    uint32_t rx_pin = 0;
    uint32_t baud = 0;
    bool invert = false;

    auto result = ts::mapper::get(*descriptor, "rx_pin", rx_pin) &
                    ts::mapper::get(*descriptor, "baud", baud) &
                    ts::mapper::get(*descriptor, "invert", invert);
    if (result != ts::success)
    {
        QLOGE("{}", result.error().what());
        return false;
    }

    result = m_descriptor->copy_assign(*descriptor);
    if (result != ts::success)
    {
        QLOGE("{}", result.error().what());
        return false;
    }

    return init(rx_pin, baud, invert);
}

std::shared_ptr<const ts::IValue> UART_BBang::get_descriptor() const
{
    return m_descriptor;
}

auto UART_BBang::init(size_t rx_pin, size_t baud, bool invert) -> bool
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
        int res = gpioSerialReadClose(m_rx_pin);
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
    int res = gpioSerialRead(m_rx_pin, data, max_size);
    if (res < 0)
    {
        QLOGE("error reading from bit-banged rx pin {}: {}", m_rx_pin, res);
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
