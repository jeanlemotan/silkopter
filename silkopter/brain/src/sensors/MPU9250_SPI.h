#pragma once

#include "MPU9250.h"
#include "spi.h"

namespace silk
{


class MPU9250_SPI : public MPU9250
{
public:

    auto open(std::string const& device) -> bool
    {
        QLOG_TOPIC("mpu9250_spi::open");
//        if (!m_spi.open(device))
//        {
//            QLOGE("can't open spi device {}: {}", device, strerror(errno));
//            return false;
//        }
//        return true;
        return false;
    }

private:

    auto mpu_read(uint8_t reg, uint8_t* data, uint32_t size) -> bool
    {
        return false;//m_spi.read(reg, data, size);
    }
    auto mpu_read_u8(uint8_t reg, uint8_t& dst) -> bool
    {
        return false;//m_spi.read_u8(reg, dst);
    }
    auto mpu_read_u16(uint8_t reg, uint16_t& dst) -> bool
    {
        return false;//m_spi.read_u16(reg, dst);
    }
    auto mpu_write_u8(uint8_t reg, uint8_t const& t) -> bool
    {
        return false;//m_spi.write_u8(reg, t);
    }
    auto mpu_write_u16(uint8_t reg, uint16_t const& t) -> bool
    {
        return false;//m_spi.write_u16(reg, t);
    }

    auto akm_read(uint8_t reg, uint8_t* data, uint32_t size) -> bool
    {
        return false;//m_spi.read(reg, data, size);
    }
    auto akm_read_u8(uint8_t reg, uint8_t& dst) -> bool
    {
        return false;//m_spi.read_u8(reg, dst);
    }
    auto akm_read_u16(uint8_t reg, uint16_t& dst) -> bool
    {
        return false;//m_spi.read_u16(reg, dst);
    }
    auto akm_write_u8(uint8_t reg, uint8_t const& t) -> bool
    {
        return false;//m_spi.write_u8(reg, t);
    }
    auto akm_write_u16(uint8_t reg, uint16_t const& t) -> bool
    {
        return false;//m_spi.write_u16(reg, t);
    }

//    spi m_spi;
};

}
