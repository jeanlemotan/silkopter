#include "BrainStdAfx.h"
#include "bus/I2C_RPI.h"

#ifdef RASPBERRY_PI

extern "C"
{
    #include "hw/bcm2835.h"
}

#include "sz_I2C_RPI.hpp"

extern auto initialize_bcm() -> bool;


namespace silk
{
namespace bus
{

std::mutex I2C_RPI::s_mutex;



I2C_RPI::I2C_RPI()
    : m_init_params(new sz::I2C_RPI::Init_Params())
    , m_config(new sz::I2C_RPI::Config())
{
}

I2C_RPI::~I2C_RPI()
{
}

auto I2C_RPI::init(rapidjson::Value const& init_params) -> bool
{
    QLOG_TOPIC("i2c_RPI::init");

    sz::I2C_RPI::Init_Params sz;
    autojsoncxx::error::ErrorStack result;
    if (!autojsoncxx::from_value(sz, init_params, result))
    {
        std::ostringstream ss;
        ss << result;
        QLOGE("Cannot deserialize I2C_RPI data: {}", ss.str());
        return false;
    }
    *m_init_params = sz;
    return init();
}

auto I2C_RPI::init() -> bool
{
    if (m_init_params->dev > 1)
    {
        QLOGE("Only I2C devices 0 & 1 are allowed");
        return false;
    }

    auto ok = initialize_bcm();
    if (!ok)
    {
        return false;
    }
    bcm2835_i2c_set_baudrate(400000);
    bcm2835_i2c_begin();

    return true;
}

void I2C_RPI::lock()
{
    s_mutex.lock();
}

auto I2C_RPI::try_lock() -> bool
{
    return s_mutex.try_lock();
}
void I2C_RPI::unlock()
{
    s_mutex.unlock();
}

auto I2C_RPI::read(uint8_t address, uint8_t* data, size_t size) -> bool
{
    QLOG_TOPIC("i2c_RPI::read");

    bcm2835_i2c_setSlaveAddress(address);
    int res = bcm2835_i2c_read(reinterpret_cast<char*>(data), size);
    if (res != BCM2835_I2C_REASON_OK)
    {
        QLOGW("read {} failed: {}", res);
        return false;
    }

    return true;
}
auto I2C_RPI::write(uint8_t address, uint8_t const* data, size_t size) -> bool
{
    QLOG_TOPIC("i2c_RPI::write");

    bcm2835_i2c_setSlaveAddress(address);
    int res = bcm2835_i2c_write(reinterpret_cast<const char*>(data), size);
    if (res != BCM2835_I2C_REASON_OK)
    {
        QLOGW("write {} failed: {}", res);
        return false;
    }

    return true;
}

auto I2C_RPI::read_register(uint8_t address, uint8_t reg, uint8_t* data, size_t size) -> bool
{
    QLOG_TOPIC("i2c_RPI::read_register");

    bcm2835_i2c_setSlaveAddress(address);
    int res = bcm2835_i2c_read_register_rs(reinterpret_cast<char*>(&reg), reinterpret_cast<char*>(data), size);
    if (res != BCM2835_I2C_REASON_OK)
    {
        QLOGW("read register {} failed: {}", reg, res);
        return false;
    }

    return true;
}
auto I2C_RPI::write_register(uint8_t address, uint8_t reg, uint8_t const* data, size_t size) -> bool
{
    QLOG_TOPIC("i2c_RPI::write_register");

    m_buffer.resize(size + 1);

    m_buffer[0] = reg;
    if (data)
    {
        std::copy(data, data + size, m_buffer.begin() + 1);
    }

    bcm2835_i2c_setSlaveAddress(address);
    int res = bcm2835_i2c_write(reinterpret_cast<const char*>(m_buffer.data()), size);
    if (res != BCM2835_I2C_REASON_OK)
    {
        QLOGW("write register {} failed: {}", reg, res);
        return false;
    }

    return true;
}

auto I2C_RPI::set_config(rapidjson::Value const& json) -> bool
{
    QLOG_TOPIC("i2c_RPI::set_config");

    sz::I2C_RPI::Config sz;
    autojsoncxx::error::ErrorStack result;
    if (!autojsoncxx::from_value(sz, json, result))
    {
        std::ostringstream ss;
        ss << result;
        QLOGE("Cannot deserialize I2C_RPI config data: {}", ss.str());
        return false;
    }

    *m_config = sz;
    return true;
}
auto I2C_RPI::get_config() const -> rapidjson::Document
{
    rapidjson::Document json;
    autojsoncxx::to_document(*m_config, json);
    return std::move(json);
}

auto I2C_RPI::get_init_params() const -> rapidjson::Document
{
    rapidjson::Document json;
    autojsoncxx::to_document(*m_init_params, json);
    return std::move(json);
}


}
}

#endif
