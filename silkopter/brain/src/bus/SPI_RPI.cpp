#include "BrainStdAfx.h"
#include "bus/SPI_RPI.h"

#ifdef RASPBERRY_PI

extern "C"
{
    #include "pigpio.h"
    #include "hw/bcm2835.h"
}
#define READ_FLAG   0x80

#include "sz_SPI_RPI.hpp"

///////////////////////////////////////////////////////////////////

//auto initialize_pigpio() -> bool
//{
//    static bool initialized = false;
//    if (initialized)
//    {
//        return true;
//    }

//    if (gpioGetMode(27) == PI_NOT_INITIALISED)
//    {
//        QLOGI("Initializing pigpio");
//        gpioCfgInterfaces(PI_DISABLE_FIFO_IF | PI_DISABLE_SOCK_IF);
//        if (gpioInitialise() < 0)
//        {
//            QLOGE("PIGPIO library initialization failed");
//            return false;
//        }
//    }
//    initialized = true;
//    return true;
//}

auto initialize_bcm() -> bool
{
    static bool initialized = false;
    if (initialized)
    {
        return true;
    }

    QLOGI("Initializing bcm2835");
    if (!bcm2835_init())
    {
        QLOGE("bcm 2835 library initialization failed");
        return false;
    }

    initialized = true;
    return true;
}

auto shutdown_bcm() -> bool
{
    QLOGI("Shutting down bcm2835");
    bcm2835_spi_end();
    bcm2835_i2c_end();
    bcm2835_close();
    return true;
}

///////////////////////////////////////////////////////////////////

namespace silk
{
namespace bus
{

std::mutex SPI_RPI::s_mutex;


SPI_RPI::SPI_RPI()
    : m_init_params(new sz::SPI_RPI::Init_Params())
    , m_config(new sz::SPI_RPI::Config())
{
}

SPI_RPI::~SPI_RPI()
{
}

auto SPI_RPI::init(rapidjson::Value const& init_params) -> bool
{
    QLOG_TOPIC("SPI_RPI::init");

    sz::SPI_RPI::Init_Params sz;
    autojsoncxx::error::ErrorStack result;
    if (!autojsoncxx::from_value(sz, init_params, result))
    {
        std::ostringstream ss;
        ss << result;
        QLOGE("Cannot deserialize SPI_RPI data: {}", ss.str());
        return false;
    }
    *m_init_params = sz;
    return init();
}
auto SPI_RPI::init() -> bool
{
    if (m_init_params->dev > 1)
    {
        QLOGE("Only SPI devices 0 & 1 are allowed");
        return false;
    }
    if (m_init_params->mode > 3)
    {
        QLOGE("Only SPI modes 0 to 3 are allowed");
        return false;
    }

    auto ok = initialize_bcm();
    if (!ok)
    {
        return false;
    }
    bcm2835_spi_begin();
    bcm2835_spi_setBitOrder(BCM2835_SPI_BIT_ORDER_MSBFIRST);      // The default
    bcm2835_spi_setChipSelectPolarity(m_init_params->dev, LOW);      // the default

    return true;
}

void SPI_RPI::lock()
{
    s_mutex.lock();
}

auto SPI_RPI::try_lock() -> bool
{
    return s_mutex.try_lock();
}

void SPI_RPI::unlock()
{
    s_mutex.unlock();
}

auto SPI_RPI::get_divider(uint32_t speed) const -> uint32_t
{
    if (speed > 125000000) return BCM2835_SPI_CLOCK_DIVIDER_2;
    else if (speed > 62000000) return BCM2835_SPI_CLOCK_DIVIDER_4;
    else if (speed > 31200000) return BCM2835_SPI_CLOCK_DIVIDER_8;
    else if (speed > 15500000) return BCM2835_SPI_CLOCK_DIVIDER_16;
    else if (speed > 7800000) return BCM2835_SPI_CLOCK_DIVIDER_32;
    else if (speed > 3900000) return BCM2835_SPI_CLOCK_DIVIDER_64;
    else if (speed > 1900000) return BCM2835_SPI_CLOCK_DIVIDER_128;
    else if (speed > 970000) return BCM2835_SPI_CLOCK_DIVIDER_256;
    else if (speed > 480000) return BCM2835_SPI_CLOCK_DIVIDER_512;
    else if (speed > 240000) return BCM2835_SPI_CLOCK_DIVIDER_1024;
    else if (speed > 120000) return BCM2835_SPI_CLOCK_DIVIDER_2048;
    else if (speed > 60000) return BCM2835_SPI_CLOCK_DIVIDER_4096;
    else if (speed > 30000) return BCM2835_SPI_CLOCK_DIVIDER_8192;
    else if (speed > 15000) return BCM2835_SPI_CLOCK_DIVIDER_16384;
    else if (speed > 7500) return BCM2835_SPI_CLOCK_DIVIDER_32768;
    else return BCM2835_SPI_CLOCK_DIVIDER_65536;
}

auto SPI_RPI::transfer(uint8_t const* tx_data, uint8_t* rx_data, size_t size, size_t speed) -> bool
{
    QASSERT(size > 0);
    if (size == 0)
    {
        return false;
    }

    uint32_t divider = get_divider(speed ? speed : m_init_params->speed);

    bcm2835_spi_setDataMode(m_init_params->mode);
    bcm2835_spi_setClockDivider(divider);
    bcm2835_spi_chipSelect(m_init_params->dev);

    if (!tx_data)
    {
        tx_data = rx_data;
    }
    if (!rx_data)
    {
        rx_data = const_cast<uint8_t*>(tx_data);
    }

    bcm2835_spi_transfernb(reinterpret_cast<char*>(const_cast<uint8_t*>(tx_data)), reinterpret_cast<char*>(rx_data), size);

    return true;
}

auto SPI_RPI::read(uint8_t* data, size_t size, size_t speed) -> bool
{
    QLOG_TOPIC("SPI_RPI::read");

    return transfer(nullptr, data, size, speed);
}
auto SPI_RPI::write(uint8_t const* data, size_t size, size_t speed) -> bool
{
    QLOG_TOPIC("SPI_RPI::write");

    return transfer(data, nullptr, size, speed);
}

auto SPI_RPI::read_register(uint8_t reg, uint8_t* data, size_t size, size_t speed) -> bool
{
    QLOG_TOPIC("SPI_RPI::read_register");

    m_tx_buffer.clear();
    m_tx_buffer.resize(size + 1);
    m_tx_buffer[0] = reg | READ_FLAG;

    m_rx_buffer.resize(size + 1);
    if (!transfer(m_tx_buffer.data(), m_rx_buffer.data(), size + 1, speed))
    {
        return false;
    }

    std::copy(m_rx_buffer.begin() + 1, m_rx_buffer.end(), data);
    return true;
}
auto SPI_RPI::write_register(uint8_t reg, uint8_t const* data, size_t size, size_t speed) -> bool
{
    QLOG_TOPIC("SPI_RPI::write_register");

    m_tx_buffer.clear();
    m_tx_buffer.resize(size + 1);
    m_tx_buffer[0] = reg;
    std::copy(data, data + size, m_tx_buffer.begin() + 1);

    m_rx_buffer.resize(size + 1);
    return transfer(m_tx_buffer.data(), m_rx_buffer.data(), size + 1, speed);
}

auto SPI_RPI::set_config(rapidjson::Value const& json) -> bool
{
    QLOG_TOPIC("SPI_RPI::set_config");

    sz::SPI_RPI::Config sz;
    autojsoncxx::error::ErrorStack result;
    if (!autojsoncxx::from_value(sz, json, result))
    {
        std::ostringstream ss;
        ss << result;
        QLOGE("Cannot deserialize SPI_RPI config data: {}", ss.str());
        return false;
    }

    *m_config = sz;
    return true;
}
auto SPI_RPI::get_config() const -> rapidjson::Document
{
    rapidjson::Document json;
    autojsoncxx::to_document(*m_config, json);
    return std::move(json);
}

auto SPI_RPI::get_init_params() const -> rapidjson::Document
{
    rapidjson::Document json;
    autojsoncxx::to_document(*m_init_params, json);
    return std::move(json);
}


}
}

#endif
