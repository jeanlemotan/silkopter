#include "BrainStdAfx.h"
#include "bus/UART_BBang.h"

#include "sz_UART_BBang.hpp"

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

UART_BBang::UART_BBang()
    : m_init_params(new sz::UART_BBang::Init_Params())
    , m_config(new sz::UART_BBang::Config())
{
}

UART_BBang::~UART_BBang()
{
    close();
}

auto UART_BBang::init(rapidjson::Value const& init_params) -> bool
{
    QLOG_TOPIC("uart_BBang::init");

    sz::UART_BBang::Init_Params sz;
    autojsoncxx::error::ErrorStack result;
    if (!autojsoncxx::from_value(sz, init_params, result))
    {
        std::ostringstream ss;
        ss << result;
        QLOGE("Cannot deserialize UART_BBang data: {}", ss.str());
        return false;
    }
    *m_init_params = sz;
    return init();
}
auto UART_BBang::init() -> bool
{
    close();

    std::lock_guard<UART_BBang> lg(*this);

#if defined (RASPBERRY_PI)

    int res = gpioSetMode(m_init_params->rx_pin, PI_INPUT);
    if (res != 0)
    {
        QLOGE("can't change bit-banging rx pin to input {}: {}", m_init_params->rx_pin, res);
        return false;
    }

    res = gpioSerialReadOpen(m_init_params->rx_pin, m_init_params->baud, 8);
    if (res != 0)
    {
        QLOGE("can't open bit-banging rx pin {}: {}", m_init_params->rx_pin, res);
        return false;
    }

    res = gpioSerialReadInvert(m_init_params->rx_pin, m_init_params->invert ? PI_BB_SER_INVERT : PI_BB_SER_NORMAL);
    if (res != 0)
    {
        QLOGE("can't open bit-banging invert property on pin {}: {}", m_init_params->rx_pin, res);
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
        int res = gpioSerialReadClose(m_init_params->rx_pin);
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
    int res = gpioSerialRead(m_init_params->rx_pin, data, max_size);
    if (res < 0)
    {
        QLOGE("error reading from bit-banged rx pin {}: {}", m_init_params->rx_pin, res);
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

auto UART_BBang::set_config(rapidjson::Value const& json) -> bool
{
    QLOG_TOPIC("uart_BBang::set_config");

    sz::UART_BBang::Config sz;
    autojsoncxx::error::ErrorStack result;
    if (!autojsoncxx::from_value(sz, json, result))
    {
        std::ostringstream ss;
        ss << result;
        QLOGE("Cannot deserialize UART_BBang config data: {}", ss.str());
        return false;
    }

    *m_config = sz;
    return true;
}
auto UART_BBang::get_config() const -> rapidjson::Document
{
    rapidjson::Document json;
    autojsoncxx::to_document(*m_config, json);
    return std::move(json);
}

auto UART_BBang::get_init_params() const -> rapidjson::Document
{
    rapidjson::Document json;
    autojsoncxx::to_document(*m_init_params, json);
    return std::move(json);
}


}
}
