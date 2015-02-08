#pragma once

#include "MPU9250.h"
#include "i2c.h"

namespace silk
{


class MPU9250_I2C : public MPU9250
{
public:

    auto open(std::string const& device) -> bool
    {
        QLOG_TOPIC("mpu9250_i2c::open");
        if (!m_i2c.open(device))
        {
            QLOGE("can't open i2c device {}: {}", device, strerror(errno));
            return false;
        }
        return true;
    }

private:

    static const uint8_t ADDR_MPU9250                       = 0x68;

    auto mpu_read(uint8_t reg, uint8_t* data, uint32_t size) -> bool
    {
        return m_i2c.read(ADDR_MPU9250, reg, data, size);
    }
    auto mpu_read_u8(uint8_t reg, uint8_t& dst) -> bool
    {
        return m_i2c.read_u8(ADDR_MPU9250, reg, dst);
    }
    auto mpu_read_u16(uint8_t reg, uint16_t& dst) -> bool
    {
        return m_i2c.read_u16(ADDR_MPU9250, reg, dst);
    }
    auto mpu_write_u8(uint8_t reg, uint8_t const& t) -> bool
    {
        return m_i2c.write_u8(ADDR_MPU9250, reg, t);
    }
    auto mpu_write_u16(uint8_t reg, uint16_t const& t) -> bool
    {
        return m_i2c.write_u16(ADDR_MPU9250, reg, t);
    }

    auto akm_read(uint8_t reg, uint8_t* data, uint32_t size) -> bool
    {
        return m_i2c.read(m_akm_address, reg, data, size);
    }
    auto akm_read_u8(uint8_t reg, uint8_t& dst) -> bool
    {
        return m_i2c.read_u8(m_akm_address, reg, dst);
    }
    auto akm_read_u16(uint8_t reg, uint16_t& dst) -> bool
    {
        return m_i2c.read_u16(m_akm_address, reg, dst);
    }
    auto akm_write_u8(uint8_t reg, uint8_t const& t) -> bool
    {
        return m_i2c.write_u8(m_akm_address, reg, t);
    }
    auto akm_write_u16(uint8_t reg, uint16_t const& t) -> bool
    {
        return m_i2c.write_u16(m_akm_address, reg, t);
    }

    i2c m_i2c;
};

}
