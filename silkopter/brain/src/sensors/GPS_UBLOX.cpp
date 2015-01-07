#include "BrainStdAfx.h"
#include "GPS_UBLOX.h"

namespace silk
{

constexpr uint8_t PREAMBLE1 = 0xB5;
constexpr uint8_t PREAMBLE2 = 0x62;

constexpr uint16_t MAX_PAYLOAD_SIZE = 256;

constexpr std::chrono::milliseconds MEASUREMENT_PERIOD(200);
constexpr std::chrono::milliseconds ACK_TIMEOUT(500);


typedef uint8_t U1;
typedef int8_t I1;
typedef uint8_t X1;

typedef uint16_t U2;
typedef int16_t I2;
typedef uint16_t X2;

typedef uint32_t U4;
typedef int32_t I4;
typedef uint32_t X4;

typedef float R4;
typedef double R8;

#pragma pack(push, 1)

struct NAV_POLLH
{
    U4 iTOW = 0; //ms
    I4 lon = 0; //deg
    I4 lat = 0; //deg
    I4 height = 0; //mm
    I4 hMSL = 0; //mm
    U4 hAcc = 0; //mm
    U4 vAcc = 0; //mm
};
struct NAV_STATUS
{
    U4 iTOW = 0; //ms
    U1 gpsFix = 0;
    X1 flags = 0;
    X1 fixStat = 0;
    X1 flags2 = 0;
    U4 ttff = 0;
    U4 msss = 0;
};

struct NAV_SOL
{
    U4 iTOW = 0; //ms
    I4 fTOW = 0; //ns
    I2 week = 0;
    U1 gpsFix = 0;
    X1 flags = 0;
    I4 ecefX = 0;//cm
    I4 ecefY = 0;//cm
    I4 ecefZ = 0;//cm
    U4 pAcc = 0;//cm
    I4 ecefVX = 0;//cm/s
    I4 ecefVY = 0;//cm/s
    I4 ecefVZ = 0;//cm/s
    U4 sAcc = 0;//cm/s
    U2 pDOP = 0;
    U1 reserved1 = 0;
    U1 numSV = 0;
    U4 reserved2 = 0;
};

struct CFG_ITFM
{
    X4 config = 0;
    X4 config2 = 0;
};

struct CFG_PRT
{
    U1 portID = 0;
    U1 reserved0 = 0;
    X2 txReady = 0;
    X4 mode = 0;
    U4 baudRate = 0;
    X2 inProtoMask = 0;
    X2 outProtoMask = 0;
    U2 reserved4 = 0;
    U2 reserved5 = 0;
};

struct CFG_RATE
{
    U2 measRate = 0;
    U2 navRate = 0;
    U2 timeRef = 0;
};
struct CFG_SBAS
{
    X1 mode = 0;
    X1 usage = 0;
    U1 maxSBAS = 0;
    X1 scanmode2 = 0;
    X4 scanmode1 = 0;
};

struct CFG_ANT
{
    X2 flags = 0;
    X2 pins = 0;
};

struct CFG_MSG
{
    U1 msgClass = 0;
    U1 msgID = 0;
    U1 rate = 0;
};

struct INF_NOTICE
{
    char start;
};

struct MON_HW
{
    X4 pinSel = 0;
    X4 pinBank = 0;
    X4 pinDir = 0;
    X4 pinVal = 0;
    U2 noisePerMS = 0;
    U2 agcCnt = 0;
    U1 aStatus = 0;
    U1 aPower = 0;
    X1 flags = 0;
    U1 reserved1 = 0;
    X4 usedMask = 0;
    U1 VP[25] = { 0 };
    U1 jamInd = 0;
    U2 reserved3 = 0;
    X4 pullIrq = 0;
    X4 pullH = 0;
    X4 pullL = 0;
};

struct MON_VER
{
    char swVersion[30] = {0};
    char hwVersion[10] = {0};
    //char romVersion[30] = {0}; //this is missing ffrom my neo-6 for some reason
};

#pragma pack(pop)


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
            packet.cls = d;
            step++;
            ck_b = ck_a = d;
            break;
        case 3:
            packet.message = static_cast<Message>((d << 8) | packet.cls);
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
            if (payload_size > 0)
            {
                packet.payload.reserve(payload_size);
                step = next;
            }
            else
            {
                step = next + 1; //go directly to checksum
            }
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

    {
        CFG_RATE data;
        data.measRate = std::chrono::duration_cast<std::chrono::milliseconds>(MEASUREMENT_PERIOD).count();
        data.timeRef = 0;//UTC time
        data.navRate = 1;
        send_packet(Message::CFG_RATE, data);
        if (!wait_for_ack(ACK_TIMEOUT) || *m_ack == false)
        {
            SILK_ERR("Cannot change GPS rate");
            return false;
        }
    }
    {
        std::array<std::pair<Message, size_t>, 3> msgs = {{
                                                              {Message::NAV_SOL, 1},
                                                              {Message::NAV_STATUS, 1},
                                                              {Message::MON_HW, 1},
                                                          }};
        for (auto m: msgs)
        {
            CFG_MSG data;
            data.msgClass = static_cast<int>(m.first) & 255;
            data.msgID = static_cast<int>(m.first) >> 8;
            data.rate = m.second;
            send_packet(Message::CFG_MSG, data);
            if (!wait_for_ack(ACK_TIMEOUT) || *m_ack == false)
            {
                SILK_ERR("Cannot change GPS rate or {} for message {}", m.second, static_cast<int>(m.first));
                return false;
            }
        }
    }

    {
        //ask for configs
        send_packet(Message::MON_VER, nullptr, 0);
        send_packet(Message::CFG_PRT, nullptr, 0);
        send_packet(Message::CFG_RATE, nullptr, 0);
        send_packet(Message::CFG_SBAS, nullptr, 0);
        send_packet(Message::CFG_ANT, nullptr, 0);
        send_packet(Message::MON_HW, nullptr, 0);
    }


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

auto GPS_UBLOX::wait_for_ack(q::Clock::duration d) -> bool
{
    if (m_fd < 0)
    {
        return false;
    }

    m_ack.reset();
    auto start = q::Clock::now();
    do
    {
        process();
        if (m_ack.is_initialized())
        {
            return true;
        }
    } while (q::Clock::now() - start < d);

    return false;
}


void GPS_UBLOX::process_packet(Packet& packet)
{
    switch (packet.message)
    {
    case Message::ACK_ACK: m_ack = true; break;
    case Message::ACK_NACK: m_ack = false; break;

    case Message::NAV_SOL: process_nav_sol_packet(packet); break;
    case Message::NAV_STATUS: process_nav_status_packet(packet); break;
    case Message::NAV_POLLH: process_nav_pollh_packet(packet); break;

    case Message::CFG_PRT: process_cfg_prt_packet(packet); break;
    case Message::CFG_RATE: process_cfg_rate_packet(packet); break;
    case Message::CFG_SBAS: process_cfg_sbas_packet(packet); break;
    case Message::CFG_ANT: process_cfg_ant_packet(packet); break;
    case Message::CFG_MSG: process_cfg_msg_packet(packet); break;

    case Message::INF_NOTICE: process_inf_notice_packet(packet); break;

    case Message::MON_VER: process_mon_ver_packet(packet); break;
    case Message::MON_HW: process_mon_hw_packet(packet); break;

        //default: SILK_INFO("Ignoring GPS packet class {}, message {}", static_cast<int>(packet.cls), static_cast<int>(packet.message)); break;
    }

}

void GPS_UBLOX::process_nav_pollh_packet(Packet& packet)
{
    QASSERT(packet.payload.size() == sizeof(NAV_POLLH));
    NAV_POLLH& data = reinterpret_cast<NAV_POLLH&>(*packet.payload.data());

    //SILK_INFO("POLLH: iTOW:{}, Lon:{}, Lat:{}, H:{}, HAcc:{}, VAcc:{}", data.iTOW, data.lon / 10000000.f, data.lat / 10000000.f, data.hMSL / 1000.f, data.hAcc / 1000.f, data.vAcc / 1000.f);
}

void GPS_UBLOX::process_nav_status_packet(Packet& packet)
{
    QASSERT(packet.payload.size() == sizeof(NAV_STATUS));
    NAV_STATUS& data = reinterpret_cast<NAV_STATUS&>(*packet.payload.data());

    //SILK_INFO("STATUS: iTOW:{}, Fix:{}, flags:{}, fs:{}, flags2:{}", data.iTOW, data.gpsFix, data.flags, data.fixStat, data.flags2);

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

    if (data.numSV > 0)
    {
        SILK_INFO("SOL: iTOW:{}, Fix:{}, flags:{}, ecef:{}, 3dacc:{}, vel:{}, velacc:{}, sv:{}", data.iTOW, data.gpsFix,
                  data.flags,
                  math::vec3f(data.ecefX, data.ecefY, data.ecefZ) / 100.f,
                  data.pAcc / 100.f,
                  math::vec3f(data.ecefVX, data.ecefVY, data.ecefVZ) / 100.f,
                  data.sAcc / 100.f,
                  data.numSV);
    }

    //gpsfix
//    0x00 = No Fix
//    0x01 = Dead Reckoning only
//    0x02 = 2D-Fix
//    0x03 = 3D-Fix
//    0x04 = GPS + dead reckoning combined
//    0x05 = Time only fix
}

void GPS_UBLOX::process_cfg_prt_packet(Packet& packet)
{
    QASSERT(packet.payload.size() == sizeof(CFG_PRT));
    CFG_PRT& data = reinterpret_cast<CFG_PRT&>(*packet.payload.data());

    SILK_INFO("GPS port config: {}baud", data.baudRate);
}

void GPS_UBLOX::process_cfg_ant_packet(Packet& packet)
{
    QASSERT(packet.payload.size() == sizeof(CFG_ANT));
    CFG_ANT& data = reinterpret_cast<CFG_ANT&>(*packet.payload.data());

    int a = 0;
    a = 1;
}

void GPS_UBLOX::process_cfg_msg_packet(Packet& packet)
{
    QASSERT(packet.payload.size() == sizeof(CFG_MSG));
    CFG_MSG& data = reinterpret_cast<CFG_MSG&>(*packet.payload.data());

    SILK_INFO("GPS class {} message {} has a rate of {}", data.msgClass, data.msgID, data.rate);
}

void GPS_UBLOX::process_cfg_rate_packet(Packet& packet)
{
    QASSERT(packet.payload.size() == sizeof(CFG_RATE));
    CFG_RATE& data = reinterpret_cast<CFG_RATE&>(*packet.payload.data());

    SILK_INFO("GPS rate config: Measurement every {}ms, NAV every {} {}", data.measRate, data.navRate, data.navRate == 1 ? "cycle" : "cycles");
}

void GPS_UBLOX::process_cfg_sbas_packet(Packet& packet)
{
    QASSERT(packet.payload.size() == sizeof(CFG_SBAS));
    CFG_SBAS& data = reinterpret_cast<CFG_SBAS&>(*packet.payload.data());

    int a = 0;
    a = 1;

    //disable SBAS
//    if (data.mode != 0)
//    {
//        data.mode = 0;
//        if (!send_packet(packet.message, data))
//        {
//            SILK_WARNING("Cannot deactivate SBAS");
//        }
//    }
}

void GPS_UBLOX::process_inf_notice_packet(Packet& packet)
{
    q::String str(reinterpret_cast<char const*>(packet.payload.data()), packet.payload.size());
    SILK_INFO("GPS notice: {}", str);
}

void GPS_UBLOX::process_mon_hw_packet(Packet& packet)
{
    QASSERT(packet.payload.size() == sizeof(MON_HW));
    MON_HW& data = reinterpret_cast<MON_HW&>(*packet.payload.data());

    SILK_INFO("GPS HW: jamming:{}, noise:{}", data.jamInd, data.noisePerMS);
}

void GPS_UBLOX::process_mon_ver_packet(Packet& packet)
{
    QASSERT(packet.payload.size() >= sizeof(MON_VER));
    MON_VER& data = reinterpret_cast<MON_VER&>(*packet.payload.data());

    SILK_INFO("GPS Version SW:{} / HW:{}", data.swVersion, data.hwVersion);
}

///////////////////////////////

auto GPS_UBLOX::get_sample() const -> sensors::GPS_Sample const&
{

}

auto GPS_UBLOX::send_packet(Message msg, uint8_t const* payload, size_t payload_size) -> bool
{
    std::array<uint8_t, 256> buffer;
    size_t off = 0;
    buffer[off++] = PREAMBLE1;
    buffer[off++] = PREAMBLE2;
    buffer[off++] = static_cast<uint16_t>(msg) & 255;
    buffer[off++] = static_cast<uint16_t>(msg) >> 8;
    buffer[off++] = payload_size & 255;
    buffer[off++] = payload_size >> 8;

    if (payload && payload_size > 0)
    {
        std::copy(payload, payload + payload_size, buffer.data() + off);
        off += payload_size;
    }

    uint8_t ck_a = 0;
    uint8_t ck_b = 0;
    for (size_t i = 2; i < off; i++)
    {
        ck_a += buffer[i];
        ck_b += ck_a;
    }

    buffer[off++] = ck_a;
    buffer[off++] = ck_b;

    Packet pk;
    decode_packet(pk, buffer.data(), buffer.data() + off);
    QASSERT(pk.ck_a == ck_a);
    QASSERT(pk.ck_b == ck_b);
    QASSERT(pk.message == msg);
    QASSERT(pk.payload_size == payload_size);

    if (write(m_fd, buffer.data(), off) < 0)
    {
        SILK_ERR("Cannot write message {} to GPS. Write failed: {}", static_cast<uint8_t>(msg), strerror(errno));
        return false;
    }

    return true;
}

template<class T>
auto GPS_UBLOX::send_packet(Message msg, T const& payload) -> bool
{
    static_assert(sizeof(T) < 200, "Message too big");

    auto res = send_packet(msg, reinterpret_cast<uint8_t const*>(&payload), sizeof(T));

    return res;
}





}
