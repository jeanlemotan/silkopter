#pragma once

#include "i2c.h"
#include "GPS_UBLOX.h"

namespace silk
{

class GPS_UBLOX_I2C : public GPS_UBLOX
{
public:
    GPS_UBLOX_I2C(i2c& i2c)
        : m_i2c(i2c)
    {
    }

    virtual auto read(uint8_t* data, size_t max_size) -> size_t
    {
        QASSERT(0);
        return 0;
    }
    virtual auto write(uint8_t const* data, size_t size) -> bool
    {
        QASSERT(0);
        return false;
    }

private:
    i2c m_i2c;
};





}
