#include "I2C_BCM.h"

#ifdef RASPBERRY_PI

extern "C"
{
    #include "utils/hw/bcm2835.h"
}

#endif

namespace util
{
namespace hw
{

I2C_BCM::I2C_BCM()
{
}

I2C_BCM::~I2C_BCM()
{
}

ts::Result<void> I2C_BCM::init(uint32_t device, uint32_t baud)
{
    if (device > 0)
    {
        return ts::Error("Only I2C devices 0 is allowed");
    }

#ifdef RASPBERRY_PI
    bcm2835_i2c_set_baudrate(baud);
    bcm2835_i2c_begin();
#endif

    return ts::success;
}

void I2C_BCM::lock()
{
    m_mutex.lock();
}

bool I2C_BCM::try_lock()
{
    return m_mutex.try_lock();
}
void I2C_BCM::unlock()
{
    m_mutex.unlock();
}

bool I2C_BCM::read(uint8_t address, uint8_t* data, size_t size)
{
    QLOG_TOPIC("i2c_bcm::read");

#ifdef RASPBERRY_PI
    bcm2835_i2c_setSlaveAddress(address);
    int res = bcm2835_i2c_read(reinterpret_cast<char*>(data), size);
    if (res != BCM2835_I2C_REASON_OK)
    {
        QLOGW("read failed: {}", res);
        return false;
    }
#endif

    return true;
}
bool I2C_BCM::write(uint8_t address, uint8_t const* data, size_t size)
{
    QLOG_TOPIC("i2c_bcm::write");

#ifdef RASPBERRY_PI
    bcm2835_i2c_setSlaveAddress(address);
    int res = bcm2835_i2c_write(reinterpret_cast<const char*>(data), size);
    if (res != BCM2835_I2C_REASON_OK)
    {
        QLOGW("write failed: {}", res);
        return false;
    }
#endif

    return true;
}

bool I2C_BCM::read_register(uint8_t address, uint8_t reg, uint8_t* data, size_t size)
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
bool I2C_BCM::write_register(uint8_t address, uint8_t reg, uint8_t const* data, size_t size)
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