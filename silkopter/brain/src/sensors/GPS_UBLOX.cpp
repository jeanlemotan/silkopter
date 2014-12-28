#include "BrainStdAfx.h"
#include "GPS_UBLOX.h"

namespace silk
{

constexpr uint8_t PREAMBLE1 = 0xB5;
constexpr uint8_t PREAMBLE2 = 0x62;

GPS_UBLOX::~GPS_UBLOX()
{
    if (m_fd)
    {
        close(m_fd);
    }
}

auto GPS_UBLOX::detect(uint8_t const* data, size_t size) -> bool
{
    QASSERT(!data || size == 0);
    if (!data || size == 0)
    {
        return false;
    }


    auto& step = m_detection.step;
    auto& ck_a = m_detection.ck_a;
    auto& ck_b = m_detection.ck_b;
    auto& payload_length = m_detection.payload_length;
    auto& payload_counter = m_detection.payload_counter;

    for (size_t i = 0; i < size; i++)
    {
        auto const d = data[i];
        switch (step)
        {
        case 1:
            if (d == PREAMBLE2)
            {
                step++;
                break;
            }
            step = 0;
        case 0:
            if (d == PREAMBLE1)
            {
                step++;
            }
            break;
        case 2:
            step++;
            ck_b = ck_a = d;
            break;
        case 3:
            step++;
            ck_b += (ck_a += d);
            break;
        case 4:
            step++;
            ck_b += (ck_a += d);
            payload_length = d;
            break;
        case 5:
            step++;
            ck_b += (ck_a += d);
            payload_counter = 0;
            break;
        case 6:
            ck_b += (ck_a += d);
            if (++payload_counter == payload_length)
            {
                step++;
            }
            break;
        case 7:
            step++;
            if (ck_a != d)
            {
                step = 0;
            }
            break;
        case 8:
            step = 0;
            if (ck_b == d)
            {
                // a valid UBlox packet
                return true;
            }
        }
    }
    return false;
}

auto GPS_UBLOX::init(int fd) -> bool
{
    QASSERT(!m_is_initialized);
    if (m_is_initialized)
    {
        SILK_ERR("Already initialized with fd: {}", m_fd);
        return false;
    }

    m_fd = fd;

    return true;
}
void GPS_UBLOX::process()
{

}

auto GPS_UBLOX::get_sample() const -> GPS_Sample const&
{

}






}
