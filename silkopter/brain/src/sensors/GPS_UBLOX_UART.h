#pragma once

#include "GPS_UBLOX.h"

namespace silk
{

class GPS_UBLOX_UART : public GPS_UBLOX
{
public:
    GPS_UBLOX_UART(int fd)
        : m_fd(fd)
    {
    }

    virtual auto read(uint8_t* data, size_t max_size) -> size_t
    {
        return m_fd >= 0 ? ::read(m_fd, data, max_size) : 0;
    }
    virtual auto write(uint8_t const* data, size_t size) -> bool
    {
        return m_fd >= 0 ? ::write(m_fd, data, size) == size : false;
    }

private:
    int m_fd = -1;
};





}
