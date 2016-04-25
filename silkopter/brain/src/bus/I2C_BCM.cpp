#include "BrainStdAfx.h"
#include "bus/I2C_BCM.h"
#include "def_lang/Mapper.h"
#include "def_lang/Type_System.h"

#ifdef RASPBERRY_PI

extern "C"
{
    #include "hw/bcm2835.h"
}

#endif

namespace silk
{
namespace bus
{

std::mutex I2C_BCM::s_mutex;



I2C_BCM::I2C_BCM(ts::Type_System const& ts)
{
    m_descriptor = ts.create_value("::silk::I2C_BCM_Descriptor");
    if (!m_descriptor)
    {
        QLOGE("Cannot create descriptor value");
    }
}

I2C_BCM::~I2C_BCM()
{
}

bool I2C_BCM::init(std::shared_ptr<ts::IValue> descriptor)
{
    if (!descriptor)
    {
        QLOGE("Null descriptor!");
        return false;
    }
    uint32_t dev = 0;

    auto result = ts::mapper::get(*descriptor, "dev", dev);
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

    return init(dev);
}

std::shared_ptr<const ts::IValue> I2C_BCM::get_descriptor() const
{
    return m_descriptor;
}

bool I2C_BCM::init(uint32_t dev)
{
    if (dev > 1)
    {
        QLOGE("Only I2C devices 0 & 1 are allowed");
        return false;
    }

#ifdef RASPBERRY_PI

    bcm2835_i2c_set_baudrate(400000);
    bcm2835_i2c_begin();

#endif

    return true;
}

void I2C_BCM::lock()
{
    s_mutex.lock();
}

auto I2C_BCM::try_lock() -> bool
{
    return s_mutex.try_lock();
}
void I2C_BCM::unlock()
{
    s_mutex.unlock();
}

auto I2C_BCM::read(uint8_t address, uint8_t* data, size_t size) -> bool
{
    QLOG_TOPIC("i2c_bcm::read");

#ifdef RASPBERRY_PI

    bcm2835_i2c_setSlaveAddress(address);
    int res = bcm2835_i2c_read(reinterpret_cast<char*>(data), size);
    if (res != BCM2835_I2C_REASON_OK)
    {
        QLOGW("read {} failed: {}", res);
        return false;
    }

#endif

    return true;
}
auto I2C_BCM::write(uint8_t address, uint8_t const* data, size_t size) -> bool
{
    QLOG_TOPIC("i2c_bcm::write");

#ifdef RASPBERRY_PI

    bcm2835_i2c_setSlaveAddress(address);
    int res = bcm2835_i2c_write(reinterpret_cast<const char*>(data), size);
    if (res != BCM2835_I2C_REASON_OK)
    {
        QLOGW("write {} failed: {}", res);
        return false;
    }

#endif

    return true;
}

auto I2C_BCM::read_register(uint8_t address, uint8_t reg, uint8_t* data, size_t size) -> bool
{
    QLOG_TOPIC("i2c_bcm::read_register");

#ifdef RASPBERRY_PI

    bcm2835_i2c_setSlaveAddress(address);
    int res = bcm2835_i2c_read_register_rs(reinterpret_cast<char*>(&reg), reinterpret_cast<char*>(data), size);
    if (res != BCM2835_I2C_REASON_OK)
    {
        QLOGW("read register {} failed: {}", reg, res);
        return false;
    }

#endif

    return true;
}
auto I2C_BCM::write_register(uint8_t address, uint8_t reg, uint8_t const* data, size_t size) -> bool
{
    QLOG_TOPIC("i2c_bcm::write_register");

    m_buffer.resize(size + 1);

    m_buffer[0] = reg;
    if (data)
    {
        std::copy(data, data + size, m_buffer.begin() + 1);
    }

#ifdef RASPBERRY_PI

    bcm2835_i2c_setSlaveAddress(address);
    int res = bcm2835_i2c_write(reinterpret_cast<const char*>(m_buffer.data()), size + 1);
    if (res != BCM2835_I2C_REASON_OK)
    {
        QLOGW("write register {} failed: {}", reg, res);
        return false;
    }

#endif

    return true;
}

}
}
