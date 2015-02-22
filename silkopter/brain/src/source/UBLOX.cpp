#include "BrainStdAfx.h"
#include "UBLOX.h"

#include "sz_math.hpp"
#include "sz_hal_nodes.hpp"

namespace silk
{
namespace node
{
namespace source
{

constexpr uint8_t PREAMBLE1 = 0xB5;
constexpr uint8_t PREAMBLE2 = 0x62;

constexpr uint16_t MIN_PACKET_SIZE = 8;
constexpr uint16_t MAX_PAYLOAD_SIZE = 256;

constexpr std::chrono::milliseconds ACK_TIMEOUT(2000);

constexpr std::chrono::seconds REINIT_WATCHGOD_TIMEOUT(3);



enum Message : uint16_t
{
    MESSAGE_ACK_ACK     = (0x01 << 8) | 0x05,
    MESSAGE_ACK_NACK    = (0x00 << 8) | 0x05,

    MESSAGE_CFG_PRT     = (0x00 << 8) | 0x06,
    MESSAGE_CFG_MSG     = (0x01 << 8) | 0x06,
    MESSAGE_CFG_RATE    = (0x08 << 8) | 0x06,
    MESSAGE_CFG_ANT     = (0x13 << 8) | 0x06,
    MESSAGE_CFG_SBAS    = (0x16 << 8) | 0x06,

    MESSAGE_NAV_POLLH   = (0x02 << 8) | 0x01,
    MESSAGE_NAV_STATUS  = (0x03 << 8) | 0x01,
    MESSAGE_NAV_SOL     = (0x06 << 8) | 0x01,
    MESSAGE_NAV_VELNED  = (0x12 << 8) | 0x01,

    MESSAGE_INF_NOTICE  = (0x02 << 8) | 0x04,

    MESSAGE_MON_HW      = (0x09 << 8) | 0x0A,
    MESSAGE_MON_VER     = (0x04 << 8) | 0x0A,
};


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


UBLOX::UBLOX(HAL& hal)
    : m_hal(hal)
{
}

UBLOX::~UBLOX()
{
}

auto UBLOX::get_name() const -> std::string const&
{
    return m_params.name;
}
auto UBLOX::get_output_stream_count() const -> size_t
{
    return 1;
}
auto UBLOX::get_output_stream(size_t idx) -> stream::IStream&
{
    QASSERT(idx < get_output_stream_count());
    return m_stream;
}

auto UBLOX::init(rapidjson::Value const& json) -> bool
{
    sz::UBLOX_Init_Params sz;
    autojsoncxx::error::ErrorStack result;
    if (!autojsoncxx::from_value(sz, json, result))
    {
        std::ostringstream ss;
        ss << result;
        QLOGE("Cannot deserialize UBLOX data: {}", ss.str());
        return false;
    }
    Init_Params params;
    params.name = sz.name;
    params.bus = m_hal.get_buses().find_by_name<bus::IBus>(sz.bus);
    params.rate = sz.rate;
    return init(params);
}
auto UBLOX::init(Init_Params const& params) -> bool
{
    QLOG_TOPIC("ublox::init");

    m_params = params;

    m_i2c = dynamic_cast<bus::II2C*>(params.bus);
    m_spi = dynamic_cast<bus::ISPI*>(params.bus);
    m_uart = dynamic_cast<bus::IUART*>(params.bus);
    if (!init())
    {
        return false;
    }

    if (!m_params.name.empty())
    {
        m_stream.name = q::util::format2<std::string>("{}-location", m_params.name);
        if (!m_hal.get_sources().add(*this) ||
            !m_hal.get_streams().add(m_stream))
        {
            return false;
        }
    }

    return true;
}
auto UBLOX::init() -> bool
{
    if (!m_i2c && !m_spi && !m_uart)
    {
        QLOGE("No bus configured");
        return false;
    }

    m_params.rate = math::clamp<size_t>(m_params.rate, 1, 5);
    m_stream.rate = m_params.rate;

    return true;
}

auto UBLOX::read(uint8_t* data, size_t max_size) -> size_t
{
    if (m_uart)
    {
        return m_uart->read(data, max_size);
    }
    else if (m_spi)
    {
        QASSERT(0);
        return 0;
    }
    else if (m_i2c)
    {
        QASSERT(0);
        return 0;
    }
}
auto UBLOX::write(uint8_t const* data, size_t size) -> bool
{
    if (m_uart)
    {
        return m_uart->write(data, size);
    }
    else if (m_spi)
    {
        QASSERT(0);
        return false;
    }
    else if (m_i2c)
    {
        QASSERT(0);
        return false;
    }
}

auto UBLOX::setup() -> bool
{
    QLOG_TOPIC("ublox::setup");

    QLOGI("Initializing UBLOX GPS...");

//    tcflush(m_fd, TCIOFLUSH);

    {
        QLOGI("Configuring GPS rate to {}...", m_params.rate);
        CFG_RATE data;
        data.measRate = std::chrono::milliseconds(1000 / m_params.rate).count();
        data.timeRef = 0;//UTC time
        data.navRate = 1;
        if (!send_packet_with_retry(MESSAGE_CFG_RATE, data, ACK_TIMEOUT, 3))
        {
            QLOGE("\t\t\t...{}", m_ack ? "FAILED" : "TIMEOUT");
            return false;
        }
    }
    {
        std::array<std::pair<Message, size_t>, 4> msgs = {{
                                                              {MESSAGE_NAV_POLLH, 1},
                                                              {MESSAGE_NAV_SOL, 1},
                                                              {MESSAGE_NAV_STATUS, 1},
                                                              {MESSAGE_MON_HW, 1},
                                                          }};
        for (auto const& m: msgs)
        {
            QLOGI("Configuring GPS rate to {} for meessage (msg: {})...",
                  m.second,
                  static_cast<int>(m.first));

            CFG_MSG data;
            data.msgClass = static_cast<int>(m.first) & 255;
            data.msgID = static_cast<int>(m.first) >> 8;
            data.rate = m.second;
            if (!send_packet_with_retry(MESSAGE_CFG_MSG, data, ACK_TIMEOUT, 3))
            {
                QLOGE("\t\t\t...{}", m_ack ? "FAILED" : "TIMEOUT");
                return false;
            }
        }
    }

    QLOGI("Requesting configs");
    {
        //ask for configs
        send_packet(MESSAGE_MON_VER, nullptr, 0);
        send_packet(MESSAGE_CFG_PRT, nullptr, 0);
        send_packet(MESSAGE_CFG_RATE, nullptr, 0);
        send_packet(MESSAGE_CFG_SBAS, nullptr, 0);
        send_packet(MESSAGE_CFG_ANT, nullptr, 0);
        send_packet(MESSAGE_MON_HW, nullptr, 0);
    }

    m_stream.last_complete_time_point = q::Clock::now();
    m_is_setup = true;
    return true;
}

void UBLOX::process()
{
    m_stream.samples.clear();

    QLOG_TOPIC("ublox::process");
    if (!m_is_setup)
    {
        if (!m_setup_future.valid() || //first time we're starting this
             m_setup_future.get_state() == boost::future_state::ready) //the previous async setup failed
        {
            if (!m_is_setup) //check again to avoid a rare race condition
            {
                //start the async setup
                m_setup_future = silk::async([this]() { setup(); });
            }
        }
        return;
    }

    read_data();

    auto now = q::Clock::now();
    auto dt = now - m_stream.last_complete_time_point;

    //watchdog
    if (m_stream.has_nav_status && m_stream.has_pollh && m_stream.has_sol)
    {
        Stream::Sample& sample = m_stream.last_sample;
        sample.dt = dt;
        sample.sample_idx++;
        m_stream.samples.push_back(sample);

        m_stream.last_complete_time_point = now;

        m_stream.has_nav_status = m_stream.has_pollh = m_stream.has_sol = false;
        m_stream.last_sample.value = Stream::Value();
    }
    else if (dt >= REINIT_WATCHGOD_TIMEOUT)
    {
        //check if we need to reset
        m_is_setup = false;
    }
}

void UBLOX::read_data()
{
    do
    {
        auto res = read(m_temp_buffer.data(), m_temp_buffer.size());
        if (res > 0)
        {
            std::copy(m_temp_buffer.begin(), m_temp_buffer.begin() + res, std::back_inserter(m_buffer));

            bool found = false;
            do
            {
                found = decode_packet(m_packet, m_buffer);
                if (found)
                {
                    process_packet(m_packet);
                    m_packet.payload.clear();
                }
            } while(found);
        }
        else
        {
            break;
        }
    } while (1);
}

auto UBLOX::decode_packet(Packet& packet, std::deque<uint8_t>& buffer) -> bool
{
    while (buffer.size() >= MIN_PACKET_SIZE)
    {
        size_t step = 0;
        size_t payload_size = 0;
        uint8_t ck_a = 0;
        uint8_t ck_b = 0;
        bool is_broken = false;

        packet.payload.clear();

        for (auto it = buffer.begin(); it != buffer.end(); ++it)
        {
            auto const d = *it;
            //LOG_INFO("step: {}, d: {}", step, d);
            switch (step)
            {
            case 0:
                packet.payload.clear();
                if (d != PREAMBLE1)
                {
                    is_broken = true;
                    break;
                }
                step++;
                break;
            case 1:
                if (d != PREAMBLE2)
                {
                    is_broken = true;
                    break;
                }
                step++;
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
                    is_broken = true;
                    break;
                }
                if (payload_size > 0)
                {
                    packet.payload.reserve(payload_size);
                    step++;
                }
                else
                {
                    step += 2; //go directly to checksum
                }
                ck_b += (ck_a += d);
                break;
            case 6:
                ck_b += (ck_a += d);
                packet.payload.push_back(d);
                step = (packet.payload.size() == payload_size) ? step + 1 : step;
                break;
            case 7:
                if (ck_a != d)
                {
                    is_broken = true;
                    break;
                }
                step++;
                break;
            case 8:
                if (ck_b != d)
                {
                    is_broken = true;
                    break;
                }

                //consume all the data from the buffer
                buffer.erase(buffer.begin(), it);
                // a valid UBlox packet
                return true;
            }
            if (is_broken)
            {
                break;
            }
        }

        if (is_broken)
        {
            //remove the preamble and start again
            buffer.pop_front();
        }
        else
        {
            //we need more data
            return false;
        }
    }
    return false;
}


auto UBLOX::wait_for_ack(q::Clock::duration d) -> bool
{
    QLOG_TOPIC("ublox::wait_for_ack");

    m_ack.reset();
    auto start = q::Clock::now();
    do
    {
        read_data();
        if (m_ack.is_initialized())
        {
            return true;
        }
    } while (q::Clock::now() - start < d);

    return false;
}


void UBLOX::process_packet(Packet& packet)
{
    QLOG_TOPIC("ublox::process_packet");

    switch (packet.message)
    {
    case MESSAGE_ACK_ACK: m_ack = true; break;
    case MESSAGE_ACK_NACK: m_ack = false; break;

    case MESSAGE_NAV_SOL: process_nav_sol_packet(packet); break;
    case MESSAGE_NAV_STATUS: process_nav_status_packet(packet); break;
    case MESSAGE_NAV_POLLH: process_nav_pollh_packet(packet); break;

    case MESSAGE_CFG_PRT: process_cfg_prt_packet(packet); break;
    case MESSAGE_CFG_RATE: process_cfg_rate_packet(packet); break;
    case MESSAGE_CFG_SBAS: process_cfg_sbas_packet(packet); break;
    case MESSAGE_CFG_ANT: process_cfg_ant_packet(packet); break;
    case MESSAGE_CFG_MSG: process_cfg_msg_packet(packet); break;

    case MESSAGE_INF_NOTICE: process_inf_notice_packet(packet); break;

    case MESSAGE_MON_VER: process_mon_ver_packet(packet); break;
    case MESSAGE_MON_HW: process_mon_hw_packet(packet); break;

    default:
    {
        CFG_MSG data;
        data.msgClass = packet.cls;
        data.msgID = static_cast<int>(packet.message) >> 8;
        data.rate = 0;
        send_packet(MESSAGE_CFG_MSG, data);
        QLOGI("Sending stop request for packet class {}, message {}", static_cast<int>(packet.cls), static_cast<int>(packet.message));
    }
    break;
    }

}

void UBLOX::process_nav_pollh_packet(Packet& packet)
{
    QASSERT(packet.payload.size() == sizeof(NAV_POLLH));
    NAV_POLLH& data = reinterpret_cast<NAV_POLLH&>(*packet.payload.data());

    //LOG_INFO("POLLH: iTOW:{}, Lon:{}, Lat:{}, H:{}, HAcc:{}, VAcc:{}", data.iTOW, data.lon / 10000000.f, data.lat / 10000000.f, data.hMSL / 1000.f, data.hAcc / 1000.f, data.vAcc / 1000.f);

    {
        m_stream.last_sample.value.latitude = data.lat / 10000000.f;
        m_stream.last_sample.value.longitude = data.lon / 10000000.f;
        m_stream.has_pollh = true;
    }
}

void UBLOX::process_nav_status_packet(Packet& packet)
{
    QASSERT(packet.payload.size() == sizeof(NAV_STATUS));
    NAV_STATUS& data = reinterpret_cast<NAV_STATUS&>(*packet.payload.data());

    //LOG_INFO("STATUS: iTOW:{}, Fix:{}, flags:{}, fs:{}, flags2:{}", data.iTOW, data.gpsFix, data.flags, data.fixStat, data.flags2);

    //gpsFix
//    - 0x00 = no fix
//    - 0x01 = dead reckoning only
//    - 0x02 = 2D-fix
//    - 0x03 = 3D-fix
//    - 0x04 = GPS + dead reckoning combined
//    - 0x05 = Time only fix
//    - 0x06..0xff = reserved

    {
        m_stream.has_nav_status = true;
    }
}

void UBLOX::process_nav_sol_packet(Packet& packet)
{
    QASSERT(packet.payload.size() == sizeof(NAV_SOL));
    NAV_SOL& data = reinterpret_cast<NAV_SOL&>(*packet.payload.data());

    if (data.numSV > 0)
    {
        QLOGI("SOL: iTOW:{}, Fix:{}, flags:{}, ecef:{}, 3dacc:{}, vel:{}, velacc:{}, sv:{}", data.iTOW, data.gpsFix,
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


    {
        m_stream.last_sample.value.sattelite_count = data.numSV;
        m_stream.last_sample.value.velocity = math::vec3f(data.ecefVX, data.ecefVY, data.ecefVZ) / 100.f;
        if (data.gpsFix == 0x02)
        {
            m_stream.last_sample.value.fix = stream::ILocation::Value::Fix::FIX_2D;
        }
        else if (data.gpsFix == 0x03)
        {
            m_stream.last_sample.value.fix = stream::ILocation::Value::Fix::FIX_3D;
        }
        m_stream.has_sol = true;
    }
}

void UBLOX::process_cfg_prt_packet(Packet& packet)
{
    QASSERT(packet.payload.size() == sizeof(CFG_PRT));
    CFG_PRT& data = reinterpret_cast<CFG_PRT&>(*packet.payload.data());

    QLOGI("GPS port config: {}baud", data.baudRate);
}

void UBLOX::process_cfg_ant_packet(Packet& packet)
{
    QASSERT(packet.payload.size() == sizeof(CFG_ANT));
    CFG_ANT& data = reinterpret_cast<CFG_ANT&>(*packet.payload.data());

    int a = 0;
    a = 1;
}

void UBLOX::process_cfg_msg_packet(Packet& packet)
{
    QASSERT(packet.payload.size() == sizeof(CFG_MSG));
    CFG_MSG& data = reinterpret_cast<CFG_MSG&>(*packet.payload.data());

    QLOGI("GPS class {} message {} has a rate of {}", data.msgClass, data.msgID, data.rate);
}

void UBLOX::process_cfg_rate_packet(Packet& packet)
{
    QASSERT(packet.payload.size() == sizeof(CFG_RATE));
    CFG_RATE& data = reinterpret_cast<CFG_RATE&>(*packet.payload.data());

    QLOGI("GPS rate config: Measurement every {}ms, NAV every {} {}", data.measRate, data.navRate, data.navRate == 1 ? "cycle" : "cycles");
}

void UBLOX::process_cfg_sbas_packet(Packet& packet)
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
//            LOG_WARNING("Cannot deactivate SBAS");
//        }
//    }
}

void UBLOX::process_inf_notice_packet(Packet& packet)
{
    std::string str(reinterpret_cast<char const*>(packet.payload.data()), packet.payload.size());
    QLOGI("GPS notice: {}", str);
}

void UBLOX::process_mon_hw_packet(Packet& packet)
{
    QASSERT(packet.payload.size() == sizeof(MON_HW));
    MON_HW& data = reinterpret_cast<MON_HW&>(*packet.payload.data());

    QLOGI("GPS HW: jamming:{}, noise:{}, agc:{}", data.jamInd, data.noisePerMS, data.agcCnt);

    send_packet(MESSAGE_MON_HW, nullptr, 0);
}

void UBLOX::process_mon_ver_packet(Packet& packet)
{
    QASSERT(packet.payload.size() >= sizeof(MON_VER));
    MON_VER& data = reinterpret_cast<MON_VER&>(*packet.payload.data());

    QLOGI("GPS Version SW:{} / HW:{}", data.swVersion, data.hwVersion);
}

///////////////////////////////

auto UBLOX::send_packet(uint16_t msg, uint8_t const* payload, size_t payload_size) -> bool
{
    std::array<uint8_t, 256> buffer;
    size_t off = 0;
    buffer[off++] = PREAMBLE1;
    buffer[off++] = PREAMBLE2;
    buffer[off++] = msg & 255;
    buffer[off++] = msg >> 8;
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

//    Packet pk;
//    decode_packet(pk, buffer.data(), buffer.data() + off);
//    QASSERT(pk.ck_a == ck_a);
//    QASSERT(pk.ck_b == ck_b);
//    QASSERT(pk.message == msg);
//    QASSERT(pk.payload_size == payload_size);

    if (!write(buffer.data(), off))
    {
        QLOGE("Cannot write message {} to GPS. Write failed: {}", static_cast<uint8_t>(msg), strerror(errno));
        return false;
    }

    //tcflush(m_fd, TCIOFLUSH);

    return true;
}

template<class T> auto UBLOX::send_packet(uint16_t msg, T const& data) -> bool
{
    static_assert(sizeof(T) < 200, "Message too big");

    auto res = send_packet(msg, reinterpret_cast<uint8_t const*>(&data), sizeof(T));

    return res;
}

template<class T> auto UBLOX::send_packet_with_retry(uint16_t msg, T const& data, q::Clock::duration timeout, size_t retries) -> bool
{
    for (size_t i = 0; i <= retries; i++)
    {
        send_packet(msg, data);
        if (wait_for_ack(timeout) && *m_ack == true)
        {
            return true;
        }
    }
    return false;
}





}
}
}