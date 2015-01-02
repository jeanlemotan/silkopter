#include "BrainStdAfx.h"
#include "GPS_UBLOX.h"

namespace silk
{

constexpr uint8_t PREAMBLE1 = 0xB5;
constexpr uint8_t PREAMBLE2 = 0x62;

constexpr uint16_t MAX_PAYLOAD_SIZE = 256;

GPS_UBLOX::~GPS_UBLOX()
{
}

auto GPS_UBLOX::detect(uint8_t const* data, size_t size) -> bool
{
    auto end = data + size;
    auto last = decode_packet(m_packet, data, end);
    return last != end;
}

auto GPS_UBLOX::decode_packet(Packet& packet, uint8_t const* data, uint8_t const* end) -> uint8_t const*
{
    if (!data || !end || end <= data)
    {
        QASSERT(0);
        return end;
    }


    auto& step = packet.step;
    auto& ck_a = packet.ck_a;
    auto& ck_b = packet.ck_b;
    auto& payload_size = packet.payload_size;

    for (auto it = data; it != end; ++it)
    {
        auto const d = *it;
        //SILK_INFO("step: {}, d: {}", step, d);
        auto next = step + 1;
        switch (step)
        {
        case 0:
            packet.payload.clear();
            step = (d == PREAMBLE1) ? next : 0;
            break;
        case 1:
            step = (d == PREAMBLE2) ? next : 0;
            break;
        case 2:
            packet.cls = static_cast<Class>(d);
            step++;
            ck_b = ck_a = d;
            break;
        case 3:
            packet.message = static_cast<Message>(d);
            step++;
            ck_b += (ck_a += d);
            break;
        case 4:
            payload_size = d;
            step++;
            ck_b += (ck_a += d);
            break;
        case 5:
            payload_size = (d << 8) | payload_size;
            if (payload_size > MAX_PAYLOAD_SIZE)
            {
                step = 0;
                break;
            }
            packet.payload.reserve(payload_size);
            step++;
            ck_b += (ck_a += d);
            break;
        case 6:
            ck_b += (ck_a += d);
            packet.payload.push_back(d);
            step = (packet.payload.size() == payload_size) ? next : step;
            break;
        case 7:
            step = (ck_a == d) ? next : 0;
            break;
        case 8:
            step = 0;
            if (ck_b != d)
            {
                break;
            }
            // a valid UBlox packet
            return it;
        }
    }
    return end;
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

    m_is_initialized = true;

    return true;
}
void GPS_UBLOX::process()
{
    if (m_fd < 0)
    {
        return;
    }

    do
    {
        auto res = read(m_fd, m_buffer.data(), m_buffer.size());
        if (res > 0)
        {
            auto const* start = m_buffer.data();
            auto const* end = m_buffer.data() + res;
            while (start != end)
            {
                start = decode_packet(m_packet, start, end);
                if (start != end)
                {
                    process_packet(m_packet);
                    m_packet.step = 0;
                    m_packet.payload.clear();
                }
            }
        }
        else
        {
            break;
        }
    } while (1);
}

void GPS_UBLOX::process_packet(Packet& packet)
{
    switch (packet.message)
    {
    case Message::NAV_SOL: process_nav_sol_packet(packet); break;
    case Message::NAV_STATUS: process_nav_status_packet(packet); break;
    case Message::NAV_POLLH: process_nav_pollh_packet(packet); break;
    //default: SILK_INFO("Ignoring GPS packet class {}, message {}", static_cast<int>(packet.cls), static_cast<int>(packet.message)); break;
    }

}

void GPS_UBLOX::process_nav_pollh_packet(Packet& packet)
{
    QASSERT(packet.payload.size() == sizeof(NAV_POLLH));
    NAV_POLLH& data = reinterpret_cast<NAV_POLLH&>(*packet.payload.data());

    SILK_INFO("POLLH: iTOW:{}, Lon:{}, Lat:{}, H:{}, HAcc:{}, VAcc:{}", data.iTOW, data.lon / 10000000.f, data.lat / 10000000.f, data.hMSL / 1000.f, data.hAcc / 1000.f, data.vAcc / 1000.f);
}

void GPS_UBLOX::process_nav_status_packet(Packet& packet)
{
    QASSERT(packet.payload.size() == sizeof(NAV_STATUS));
    NAV_STATUS& data = reinterpret_cast<NAV_STATUS&>(*packet.payload.data());

    SILK_INFO("STATUS: iTOW:{}, Fix:{}, flags:{}, fs:{}, flags2:{}", data.iTOW, data.gpsFix, data.flags, data.fixStat, data.flags2);

    //gpsFix
//    - 0x00 = no fix
//    - 0x01 = dead reckoning only
//    - 0x02 = 2D-fix
//    - 0x03 = 3D-fix
//    - 0x04 = GPS + dead reckoning combined
//    - 0x05 = Time only fix
//    - 0x06..0xff = reserved
}

void GPS_UBLOX::process_nav_sol_packet(Packet& packet)
{
    QASSERT(packet.payload.size() == sizeof(NAV_SOL));
    NAV_SOL& data = reinterpret_cast<NAV_SOL&>(*packet.payload.data());

    SILK_INFO("SOL: iTOW:{}, Fix:{}, flags:{}, ecef:{}, 3dacc:{}, vel:{}, velacc:{}, sv:{}", data.iTOW, data.gpsFix,
              data.flags,
              math::vec3f(data.ecefX, data.ecefY, data.ecefZ) / 100.f,
              data.pAcc / 100.f,
              math::vec3f(data.ecefVX, data.ecefVY, data.ecefVZ) / 100.f,
              data.sAcc / 100.f,
              data.numSV);

    //gpsfix
//    0x00 = No Fix
//    0x01 = Dead Reckoning only
//    0x02 = 2D-Fix
//    0x03 = 3D-Fix
//    0x04 = GPS + dead reckoning combined
//    0x05 = Time only fix
}

auto GPS_UBLOX::get_sample() const -> GPS_Sample const&
{

}






}
