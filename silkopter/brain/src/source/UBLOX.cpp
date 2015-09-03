#include "BrainStdAfx.h"
#include "UBLOX.h"

#include "sz_math.hpp"
#include "sz_UBLOX.hpp"


namespace silk
{
namespace node
{

constexpr uint8_t PREAMBLE1 = 0xB5;
constexpr uint8_t PREAMBLE2 = 0x62;

constexpr uint16_t MIN_PACKET_SIZE = 8;
constexpr uint16_t MAX_PAYLOAD_SIZE = 512;

constexpr std::chrono::milliseconds ACK_TIMEOUT(500);

constexpr std::chrono::seconds REINIT_WATCHDOG_TIMEOUT(5);

enum Class : uint16_t
{
    CLASS_NAV   = 0x01,
    CLASS_INF   = 0x04,
    CLASS_ACK   = 0x05,
    CLASS_CFG   = 0x06,
    CLASS_MON   = 0x0A,
};


enum Message : uint16_t
{
    MESSAGE_ACK_ACK     = (0x01 << 8) | CLASS_ACK,
    MESSAGE_ACK_NACK    = (0x00 << 8) | CLASS_ACK,

    MESSAGE_CFG_PRT     = (0x00 << 8) | CLASS_CFG,
    MESSAGE_CFG_MSG     = (0x01 << 8) | CLASS_CFG,
    MESSAGE_CFG_RATE    = (0x08 << 8) | CLASS_CFG,
    MESSAGE_CFG_ANT     = (0x13 << 8) | CLASS_CFG,
    MESSAGE_CFG_SBAS    = (0x16 << 8) | CLASS_CFG,
    MESSAGE_CFG_RST     = (0x04 << 8) | CLASS_CFG,

    MESSAGE_NAV_POLLH   = (0x02 << 8) | CLASS_NAV,
    MESSAGE_NAV_STATUS  = (0x03 << 8) | CLASS_NAV,
    MESSAGE_NAV_SOL     = (0x06 << 8) | CLASS_NAV,
    MESSAGE_NAV_VELNED  = (0x12 << 8) | CLASS_NAV,
    MESSAGE_NAV_POSECEF = (0x01 << 8) | CLASS_NAV,
    MESSAGE_NAV_VELECEF = (0x11 << 8) | CLASS_NAV,

    MESSAGE_INF_NOTICE  = (0x02 << 8) | CLASS_INF,

    MESSAGE_MON_HW      = (0x09 << 8) | CLASS_MON,
    MESSAGE_MON_VER     = (0x04 << 8) | CLASS_MON,
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

struct NAV_POSECEF
{
    U4 iTOW = 0; //ms
    I4 ecefX = 0;//cm
    I4 ecefY = 0;//cm
    I4 ecefZ = 0;//cm
    U4 pAcc = 0;//cm
};
struct NAV_VELECEF
{
    U4 iTOW = 0; //ms
    I4 ecefVX = 0;//cm/s
    I4 ecefVY = 0;//cm/s
    I4 ecefVZ = 0;//cm/s
    U4 sAcc = 0;//cm/s
};

struct CFG_ITFM
{
    X4 config = 0;
    X4 config2 = 0;
};

struct CFG_RST
{
    X2 navBbrMask = 0;
    U1 resetMode = 0;
    U1 reserved1 = 0;
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
    char start = 0;
};

struct MON_HW_6
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

struct MON_HW_7
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
    U1 VP[17] = { 0 };
    U1 jamInd = 0;
    U2 reserved3 = 0;
    X4 pinIrq = 0;
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
    , m_init_params(new sz::UBLOX::Init_Params())
    , m_config(new sz::UBLOX::Config())
{
    m_position_stream = std::make_shared<Position_Stream>();
    m_velocity_stream = std::make_shared<Velocity_Stream>();
    m_gps_info_stream = std::make_shared<GPS_Info_Stream>();
}

UBLOX::~UBLOX()
{
}

auto UBLOX::lock(Buses& buses) -> bool
{
    if (buses.i2c)
    {
        buses.i2c->lock(); //lock the bus
        return true;
    }
    if (buses.spi)
    {
        buses.spi->lock(); //lock the bus
        return true;
    }
    if (buses.uart)
    {
        buses.uart->lock(); //lock the bus
        return true;
    }
    return false;
}
void UBLOX::unlock(Buses& buses)
{
    if (buses.i2c)
    {
        buses.i2c->unlock(); //unlock the bus
        return;
    }
    if (buses.spi)
    {
        buses.spi->unlock(); //unlock the bus
        return;
    }
    if (buses.uart)
    {
        buses.uart->unlock(); //lock the bus
        return;
    }
}

auto UBLOX::get_outputs() const -> std::vector<Output>
{
    std::vector<Output> outputs =
    {{
         { "Position (ecef)", m_position_stream },
         { "Velocity (ecef)", m_velocity_stream },
         { "GPS Info", m_gps_info_stream },
    }};
    return outputs;
}

auto UBLOX::init(rapidjson::Value const& init_params) -> bool
{
    QLOG_TOPIC("ublox::init");

    sz::UBLOX::Init_Params sz;
    autojsoncxx::error::ErrorStack result;
    if (!autojsoncxx::from_value(sz, init_params, result))
    {
        std::ostringstream ss;
        ss << result;
        QLOGE("Cannot deserialize UBLOX data: {}", ss.str());
        return false;
    }
    *m_init_params = sz;
    return init();
}
auto UBLOX::init() -> bool
{
    m_i2c = m_hal.get_buses().find_by_name<bus::II2C>(m_init_params->bus);
    m_spi = m_hal.get_buses().find_by_name<bus::ISPI>(m_init_params->bus);
    m_uart = m_hal.get_buses().find_by_name<bus::IUART>(m_init_params->bus);

    m_init_params->rate = math::clamp<size_t>(m_init_params->rate, 1, 10);

    m_position_stream->set_rate(m_init_params->rate);
    m_position_stream->set_tp(q::Clock::now());

    m_velocity_stream->set_rate(m_init_params->rate);
    m_velocity_stream->set_tp(q::Clock::now());

    m_gps_info_stream->set_rate(m_init_params->rate);
    m_gps_info_stream->set_tp(q::Clock::now());

    return setup();
}

auto UBLOX::read(Buses& buses, uint8_t* rx_data, size_t max_size) -> size_t
{
    if (buses.uart)
    {
        return buses.uart->read(rx_data, max_size);
    }
    else if (buses.spi)
    {
        max_size = math::min<size_t>(max_size, 32u);
        m_dummy_tx_data.resize(max_size, 0);
        return buses.spi->transfer(m_dummy_tx_data.data(), rx_data, max_size) ? max_size : 0;
    }
    else if (buses.i2c)
    {
        QASSERT(0);
        return 0;
    }
    QASSERT(0);
    return 0;
}
auto UBLOX::write(Buses& buses, uint8_t const* tx_data, size_t size) -> bool
{
    if (buses.uart)
    {
        return buses.uart->write(tx_data, size);
    }
    else if (buses.spi)
    {
        m_dummy_rx_data.resize(size);
        return buses.spi->transfer(tx_data, m_dummy_rx_data.data(), size);
    }
    else if (buses.i2c)
    {
        QASSERT(0);
        return false;
    }
    QASSERT(0);
    return false;
}

auto UBLOX::setup() -> bool
{
    QLOG_TOPIC("ublox::setup");

    QLOGI("Initializing UBLOX GPS...");

//    tcflush(m_fd, TCIOFLUSH);

    Buses buses = { m_i2c.lock(), m_spi.lock(), m_uart.lock() };
    if (!buses.i2c && !buses.spi && !buses.uart)
    {
        QLOGE("No bus configured");
        return false;
    }

    lock(buses);
    At_Exit at_exit([this, &buses]()
    {
        unlock(buses);
    });

    //read some data from the port to make sure the GPS doesn't have pending data
//    {
//        QLOGI("Flushing GPS buffers");
//        for (size_t i = 0; i < 100; i++)
//        {
//            read(buses, m_temp_buffer.data(), m_temp_buffer.size());
//            //QLOGI("\t{}: {} bytes", i, res);
//        }
//    }

    reset(buses);

    {
        std::vector<std::pair<Message, size_t>> msgs = {{
                                                              {MESSAGE_NAV_POSECEF, 0},
                                                              {MESSAGE_NAV_VELECEF, 0},
                                                              {MESSAGE_NAV_SOL, 0},
                                                              {MESSAGE_NAV_STATUS, 0},
                                                              {MESSAGE_MON_HW, 0},
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
            if (!send_packet_with_retry(buses, MESSAGE_CFG_MSG, data, ACK_TIMEOUT, 2))
            {
                QLOGW("\t\t\t...{}", m_ack ? "FAILED" : "TIMEOUT");
                //return false;
            }
        }
    }

    {
        QLOGI("Configuring GPS rate to {}...", m_init_params->rate);
        CFG_RATE data;
        data.measRate = std::chrono::milliseconds(1000 / m_init_params->rate).count();
        data.timeRef = 0;//UTC time
        data.navRate = 1;
        if (!send_packet_with_retry(buses, MESSAGE_CFG_RATE, data, ACK_TIMEOUT, 2))
        {
            QLOGW("\t\t\t...{}", m_ack ? "FAILED" : "TIMEOUT");
            //return false;
        }
    }

    QLOGI("Requesting configs");
    {
        //ask for configs
        send_packet(buses, MESSAGE_MON_VER, nullptr, 0);
        send_packet(buses, MESSAGE_CFG_PRT, nullptr, 0);
        send_packet(buses, MESSAGE_CFG_RATE, nullptr, 0);
        send_packet(buses, MESSAGE_CFG_SBAS, nullptr, 0);
        send_packet(buses, MESSAGE_CFG_ANT, nullptr, 0);
        send_packet(buses, MESSAGE_MON_HW, nullptr, 0);
    }

    util::coordinates::LLA default_coords(0, 0, 0);
    m_last_position_value = util::coordinates::lla_to_ecef(default_coords);

    m_position_stream->set_tp(q::Clock::now());
    m_velocity_stream->set_tp(q::Clock::now());
    m_gps_info_stream->set_tp(q::Clock::now());

    m_last_process_tp = q::Clock::now();
    return true;
}

void UBLOX::poll_for_data(Buses& buses)
{
    auto now = q::Clock::now();
    //if (now - m_last_poll_tp > std::chrono::milliseconds(200))
    {
        m_last_poll_tp = now;
        //send_packet(buses, MESSAGE_NAV_POSECEF, nullptr, 0);
        //send_packet(buses, MESSAGE_NAV_VELECEF, nullptr, 0);
        send_packet(buses, MESSAGE_NAV_SOL, nullptr, 0);
        //send_packet(buses, MESSAGE_NAV_STATUS, nullptr, 0);
        send_packet(buses, MESSAGE_MON_HW, nullptr, 0);
    }
}

void UBLOX::reset(Buses& buses)
{
    auto now = q::Clock::now();
    if (now - m_last_reset_tp > std::chrono::seconds(5))
    {
        m_last_reset_tp = now;
        QLOGI("Resseting the receiver");
        CFG_RST data;
        data.navBbrMask = 0x0; //HOT START
        data.resetMode = 0x0; //HW reset
        send_packet(buses, MESSAGE_CFG_RST, data);
    }
}

void UBLOX::process()
{
    QLOG_TOPIC("ublox::process");

    m_position_stream->clear();
    m_velocity_stream->clear();
    m_gps_info_stream->clear();

    auto now = q::Clock::now();
    if (now - m_last_process_tp < m_position_stream->get_dt())
    {
        return;
    }

    //QLOGI("Process... {}", now - m_last_tp);

    m_last_process_tp = now;


//    if (m_setup_state != Setup_State::DONE)
//    {
//        if (m_setup_state == Setup_State::UNKNOWN || m_setup_state == Setup_State::FAILED)
//        {
//            m_setup_state = Setup_State::RUNNING;

//            //start the async setup
//            silk::async(std::function<void()>([this]()
//            {
//                bool res = setup();
//                m_setup_state = res ? Setup_State::DONE : Setup_State::FAILED;
//            }));
//        }
//        return;
//    }

    Buses buses = { m_i2c.lock(), m_spi.lock(), m_uart.lock() };
    if (!buses.i2c && !buses.spi && !buses.uart)
    {
        return;
    }

    lock(buses);
    At_Exit at_exit([this, &buses]()
    {
        unlock(buses);
    });

    read_data(buses);

    //poll for data so the reeiver doesn't disable the SPI interface.
    //see section: 8.2 Extended TX timeout
    poll_for_data(buses);

    bool is_gps_info_healthy = true;
    if (now - m_last_gps_info_tp > REINIT_WATCHDOG_TIMEOUT)
    {
        QLOGW("No GPS Info packets for {}", now - m_last_gps_info_tp);
        poll_for_data(buses);
        is_gps_info_healthy = false;
    }
    bool is_position_healthy = true;
    if (now - m_last_position_tp > REINIT_WATCHDOG_TIMEOUT)
    {
        QLOGW("No GPS Position packets for {}", now - m_last_position_tp);
        poll_for_data(buses);
        is_position_healthy = false;
    }
    bool is_velocity_healthy = true;
    if (now - m_last_velocity_tp > REINIT_WATCHDOG_TIMEOUT)
    {
        QLOGW("No GPS Velocity packets for {}", now - m_last_velocity_tp);
        poll_for_data(buses);
        is_velocity_healthy = false;
    }

    size_t samples_needed = m_position_stream->compute_samples_needed();
    while (samples_needed > 0)
    {
        m_position_stream->push_sample(m_last_position_value, m_last_gps_info_value.fix != stream::IGPS_Info::Value::Fix::INVALID && is_position_healthy);
        samples_needed--;
    }

    samples_needed = m_velocity_stream->compute_samples_needed();
    while (samples_needed > 0)
    {
        m_velocity_stream->push_sample(m_last_velocity_value, m_last_gps_info_value.fix != stream::IGPS_Info::Value::Fix::INVALID && is_velocity_healthy);
        samples_needed--;
    }

    samples_needed = m_gps_info_stream->compute_samples_needed();
    while (samples_needed > 0)
    {
        m_gps_info_stream->push_sample(m_last_gps_info_value, is_gps_info_healthy);
        samples_needed--;
    }
}

void UBLOX::read_data(Buses& buses)
{
    auto start = q::Clock::now();
    constexpr std::chrono::milliseconds MAX_DURATION(5);

    while (q::Clock::now() - start < MAX_DURATION)
    {
        auto result = decode_packet(m_packet, m_buffer);
        if (result == Decoder_State::Result::FOUND_PACKET)
        {
            process_packet(buses, m_packet);
            m_packet.payload.clear();

            //remove SPI no-data markers
            auto it = std::find_if(m_buffer.begin(), m_buffer.end(), [](uint8_t x) { return x != 0xFF; });
            if (it != m_buffer.begin())
            {
                m_buffer.erase(m_buffer.begin(), it);
            }
        }

        //remove SPI no-data markers
        if (result == Decoder_State::Result::NEEDS_DATA || result == Decoder_State::Result::DONE)
        {
            auto res = read(buses, m_temp_buffer.data(), m_temp_buffer.size());
            if (res > 0)
            {
                std::copy(m_temp_buffer.begin(), m_temp_buffer.begin() + res, std::back_inserter(m_buffer));
                if (result == Decoder_State::Result::DONE)
                {
                    //remove SPI no-data markers
                    auto it = std::find_if(m_buffer.begin(), m_buffer.end(), [](uint8_t x) { return x != 0xFF; });
                    if (it != m_buffer.begin())
                    {
                        m_buffer.erase(m_buffer.begin(), it);
                    }
                }
            }
            else
            {
                break;
            }
        }
    }
}

auto UBLOX::decode_packet(Packet& packet, std::deque<uint8_t>& buffer) -> Decoder_State::Result
{
    while (!buffer.empty())//MIN_PACKET_SIZE)
    {
        bool is_invalid = false;

        for (; m_state.data_idx < buffer.size() && is_invalid == false; ++m_state.data_idx)
        {
            auto const d = buffer[m_state.data_idx];
            if (d != 0)
            {
               // QLOGI("step: {}, d: {}", step, d);
            }
            switch (m_state.step)
            {
            case 0:
                if (d != PREAMBLE1)
                {
                    //QLOGI("step: {}, d: {}, invalid", step, d);
                    is_invalid = true;
                    break;
                }
                packet.payload.clear();
                //QLOGI("step: {}, d: {}", step, d);
                m_state.step++;
                break;
            case 1:
                if (d != PREAMBLE2)
                {
                    //QLOGI("step: {}, d: {}, invalid", step, d);
                    is_invalid = true;
                    break;
                }
                //QLOGI("step: {}, d: {}", step, d);
                m_state.step++;
                break;
            case 2:
                packet.cls = d;
                m_state.step++;
                m_state.ck_b = m_state.ck_a = d;
                break;
            case 3:
                //QLOGI("step: {}, d: {}, cls {}, msg {}", step, d, packet.cls, d);
                packet.message = static_cast<Message>((d << 8) | packet.cls);
                m_state.step++;
                m_state.ck_b += (m_state.ck_a += d);
                break;
            case 4:
                m_state.payload_size = d;
                m_state.step++;
                m_state.ck_b += (m_state.ck_a += d);
                break;
            case 5:
                m_state.payload_size = (d << 8) | m_state.payload_size;
                //QLOGI("step: {}, d: {}, size", step, d, payload_size);
                if (m_state.payload_size > MAX_PAYLOAD_SIZE)
                {
                    //QLOGI("step: {}, d: {}, invalid", step, d);
                    is_invalid = true;
                    break;
                }
                if (m_state.payload_size > 0)
                {
                    packet.payload.reserve(m_state.payload_size);
                    m_state.step++;
                }
                else
                {
                    m_state.step += 2; //go directly to checksum
                }
                m_state.ck_b += (m_state.ck_a += d);
                break;
            case 6:
                m_state.ck_b += (m_state.ck_a += d);
                packet.payload.push_back(d);
                m_state.step = (packet.payload.size() == m_state.payload_size) ? m_state.step + 1 : m_state.step;
                break;
            case 7:
                if (m_state.ck_a != d)
                {
                    //QLOGI("step: {}, d: {}, invalid", step, d);
                    is_invalid = true;
                    break;
                }
                //QLOGI("step: {}, d: {}", step, d);
                m_state.step++;
                break;
            case 8:
                if (m_state.ck_b != d)
                {
                    //QLOGI("step: {}, d: {}, invalid", step, d);
                    is_invalid = true;
                    break;
                }

                //QLOGI("step MSG: {}, d: {} - {}", m_state.step, d, m_packet.message);
                //consume all the data from the buffer
                buffer.erase(buffer.begin(), buffer.begin() + m_state.data_idx + 1);

                m_state = Decoder_State();
                // a valid UBlox packet
                return Decoder_State::Result::FOUND_PACKET;
            }
        }

        if (is_invalid)
        {
            m_state = Decoder_State();
            //remove the preamble and start again
            buffer.pop_front();
        }
        else
        {
            //so far so good but we need more data
            return Decoder_State::Result::NEEDS_DATA;
        }
    }

    m_state = Decoder_State();
    return Decoder_State::Result::DONE;
}


auto UBLOX::wait_for_ack(Buses& buses, q::Clock::duration d) -> bool
{
    QLOG_TOPIC("ublox::wait_for_ack");

    m_ack.reset();
    auto start = q::Clock::now();
    do
    {
        read_data(buses);
        if (m_ack.is_initialized())
        {
            return true;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    } while (q::Clock::now() - start < d);

    return false;
}


void UBLOX::process_packet(Buses& buses, Packet& packet)
{
    QLOG_TOPIC("ublox::process_packet");

    //QLOGI("packet class {}, message {}, size {}", static_cast<int>(packet.cls), static_cast<int>(packet.message) >> 8, packet.payload.size());

    switch (packet.message)
    {
    case MESSAGE_ACK_ACK: m_ack = true; break;
    case MESSAGE_ACK_NACK: m_ack = false; break;

    case MESSAGE_NAV_SOL: process_nav_sol_packet(buses, packet); break;
    case MESSAGE_NAV_POSECEF: process_nav_posecef_packet(buses, packet); break;
    case MESSAGE_NAV_VELECEF: process_nav_velecef_packet(buses, packet); break;
    case MESSAGE_NAV_STATUS: process_nav_status_packet(buses, packet); break;
    case MESSAGE_NAV_POLLH: process_nav_pollh_packet(buses, packet); break;

    case MESSAGE_CFG_PRT: process_cfg_prt_packet(buses, packet); break;
    case MESSAGE_CFG_RATE: process_cfg_rate_packet(buses, packet); break;
    case MESSAGE_CFG_SBAS: process_cfg_sbas_packet(buses, packet); break;
    case MESSAGE_CFG_ANT: process_cfg_ant_packet(buses, packet); break;
    case MESSAGE_CFG_MSG: process_cfg_msg_packet(buses, packet); break;

    case MESSAGE_INF_NOTICE: process_inf_notice_packet(buses, packet); break;

    case MESSAGE_MON_VER: process_mon_ver_packet(buses, packet); break;
    case MESSAGE_MON_HW: process_mon_hw_packet(buses, packet); break;

    default:
    {
        if (packet.cls == CLASS_NAV)
        {
            CFG_MSG data;
            data.msgClass = packet.cls;
            data.msgID = static_cast<int>(packet.message) >> 8;
            data.rate = 0;
            send_packet(buses, MESSAGE_CFG_MSG, data);
            QLOGI("Sending stop request for packet class {}, message {}, size {}", static_cast<int>(packet.cls), static_cast<int>(data.msgID), packet.payload.size());
        }
    }
    break;
    }

}

void UBLOX::process_nav_pollh_packet(Buses& buses, Packet& packet)
{
    QASSERT(packet.payload.size() == sizeof(NAV_POLLH));
    NAV_POLLH& data = reinterpret_cast<NAV_POLLH&>(*packet.payload.data());

    //LOG_INFO("POLLH: iTOW:{}, Lon:{}, Lat:{}, H:{}, HAcc:{}, VAcc:{}", data.iTOW, data.lon / 10000000.0, data.lat / 10000000.0, data.hMSL / 1000.0, data.hAcc / 1000.0, data.vAcc / 1000.0);

    {
//        m_wgs84_location_stream->last_sample.value.lat_lon.set(math::radians(data.lat / 10000000.0), math::radians(data.lon / 10000000.0));
//        m_wgs84_location_stream->last_sample.value.lat_lon_accuracy = data.hAcc / 100.f;
//        m_wgs84_location_stream->last_sample.value.altitude = data.height / 100.f;
//        m_wgs84_location_stream->last_sample.value.altitude_accuracy = data.vAcc / 100.f;
    }

//    m_has_pollh = true;
}

void UBLOX::process_nav_status_packet(Buses& buses, Packet& packet)
{
    QASSERT(packet.payload.size() == sizeof(NAV_STATUS));
    NAV_STATUS& data = reinterpret_cast<NAV_STATUS&>(*packet.payload.data());

    //QLOGI("STATUS: iTOW:{}, Fix:{}, flags:{}, fs:{}, flags2:{}", data.iTOW, data.gpsFix, data.flags, data.fixStat, data.flags2);

    {
        if (data.gpsFix == 0x02)
        {
            m_last_gps_info_value.fix = stream::IGPS_Info::Value::Fix::FIX_2D;
        }
        else if (data.gpsFix == 0x03)
        {
            m_last_gps_info_value.fix = stream::IGPS_Info::Value::Fix::FIX_3D;
        }
        else
        {
            m_last_gps_info_value.fix = stream::IGPS_Info::Value::Fix::INVALID;
        }
    }
    m_last_gps_info_tp = q::Clock::now();
}

void UBLOX::process_nav_sol_packet(Buses& buses, Packet& packet)
{
    QASSERT(packet.payload.size() == sizeof(NAV_SOL));
    NAV_SOL& data = reinterpret_cast<NAV_SOL&>(*packet.payload.data());

    if (0 && data.numSV > 0)
    {
        QLOGI("SOL: iTOW:{}, Fix:{}, flags:{}, ecef:{}, 3dacc:{}, vel:{}, velacc:{}, sv:{}", data.iTOW, data.gpsFix,
                  data.flags,
                  math::vec3d(data.ecefX, data.ecefY, data.ecefZ) / 100.0,
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


    m_last_position_value = math::vec3d(data.ecefX, data.ecefY, data.ecefZ) / 100.0;
    m_last_velocity_value = math::vec3f(data.ecefVX, data.ecefVY, data.ecefVZ) / 100.f;

    m_last_gps_info_value.visible_satellites = data.numSV;
    m_last_gps_info_value.fix_satellites = data.numSV;
    m_last_gps_info_value.pacc = (data.pAcc / 100.f) / 1.18f; //converting to cm and then to std dev
    m_last_gps_info_value.vacc = (data.sAcc / 100.f) / 1.18f; //converting to cm and then to std dev
    m_last_gps_info_value.pdop = data.pDOP / 100.f;
    if (data.gpsFix == 0x02)
    {
        m_last_gps_info_value.fix = stream::IGPS_Info::Value::Fix::FIX_2D;
    }
    else if (data.gpsFix == 0x03)
    {
        m_last_gps_info_value.fix = stream::IGPS_Info::Value::Fix::FIX_3D;
    }
    else
    {
        m_last_gps_info_value.fix = stream::IGPS_Info::Value::Fix::INVALID;
    }

    m_last_position_tp = q::Clock::now();
    m_last_velocity_tp = q::Clock::now();
    m_last_gps_info_tp = q::Clock::now();
}

void UBLOX::process_nav_posecef_packet(Buses& buses, Packet& packet)
{
    QASSERT(packet.payload.size() == sizeof(NAV_POSECEF));
    NAV_POSECEF& data = reinterpret_cast<NAV_POSECEF&>(*packet.payload.data());

    m_last_position_value = math::vec3d(data.ecefX, data.ecefY, data.ecefZ) / 100.0;
    m_last_gps_info_value.pacc = (data.pAcc / 100.f) / 1.18f; //converting to cm and then to std dev

    m_last_position_tp = q::Clock::now();
}

void UBLOX::process_nav_velecef_packet(Buses& buses, Packet& packet)
{
    QASSERT(packet.payload.size() == sizeof(NAV_VELECEF));
    NAV_VELECEF& data = reinterpret_cast<NAV_VELECEF&>(*packet.payload.data());

    m_last_velocity_value = math::vec3f(data.ecefVX, data.ecefVY, data.ecefVZ) / 100.f;
    m_last_gps_info_value.vacc = (data.sAcc / 100.f) / 1.18f; //converting to cm and then to std dev

    m_last_velocity_tp = q::Clock::now();
}

void UBLOX::process_cfg_prt_packet(Buses& buses, Packet& packet)
{
    QASSERT(packet.payload.size() == sizeof(CFG_PRT));
    CFG_PRT& data = reinterpret_cast<CFG_PRT&>(*packet.payload.data());

    QLOGI("GPS port config: {}baud", data.baudRate);
}

void UBLOX::process_cfg_ant_packet(Buses& buses, Packet& packet)
{
    QASSERT(packet.payload.size() == sizeof(CFG_ANT));
    CFG_ANT& data = reinterpret_cast<CFG_ANT&>(*packet.payload.data());

    //int a = 0;
    //a = 1;
}

void UBLOX::process_cfg_msg_packet(Buses& buses, Packet& packet)
{
    QASSERT(packet.payload.size() == sizeof(CFG_MSG));
    CFG_MSG& data = reinterpret_cast<CFG_MSG&>(*packet.payload.data());

    QLOGI("GPS class {} message {} has a rate of {}", data.msgClass, data.msgID, data.rate);
}

void UBLOX::process_cfg_rate_packet(Buses& buses, Packet& packet)
{
    QASSERT(packet.payload.size() == sizeof(CFG_RATE));
    CFG_RATE& data = reinterpret_cast<CFG_RATE&>(*packet.payload.data());

    QLOGI("GPS rate config: Measurement every {}ms, NAV every {} {}", data.measRate, data.navRate, data.navRate == 1 ? "cycle" : "cycles");
}

void UBLOX::process_cfg_sbas_packet(Buses& buses, Packet& packet)
{
    QASSERT(packet.payload.size() == sizeof(CFG_SBAS));
    CFG_SBAS& data = reinterpret_cast<CFG_SBAS&>(*packet.payload.data());

    //int a = 0;
    //a = 1;

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

void UBLOX::process_inf_notice_packet(Buses& buses, Packet& packet)
{
    std::string str(reinterpret_cast<char const*>(packet.payload.data()), packet.payload.size());
    QLOGI("GPS notice: {}", str);
}

void UBLOX::process_mon_hw_packet(Buses& buses, Packet& packet)
{
    uint32_t jamInd = 0;
    uint32_t noisePerMS = 0;
    uint32_t agcCnt = 0;
    if (packet.payload.size() == sizeof(MON_HW_6))
    {
        auto& data = reinterpret_cast<MON_HW_6&>(*packet.payload.data());
        jamInd = data.jamInd;
        noisePerMS = data.noisePerMS;
        agcCnt = data.agcCnt;
    }
    else if (packet.payload.size() == sizeof(MON_HW_7))
    {
        auto& data = reinterpret_cast<MON_HW_7&>(*packet.payload.data());
        jamInd = data.jamInd;
        noisePerMS = data.noisePerMS;
        agcCnt = data.agcCnt;
    }
    else
    {
        QLOGE("unknown HW_MON packet size: {}", packet.payload.size());
    }

    QLOGI("GPS HW: jamming:{}, noise:{}, agc:{}", jamInd, noisePerMS, agcCnt);
    send_packet(buses, MESSAGE_MON_HW, nullptr, 0);
}

void UBLOX::process_mon_ver_packet(Buses& buses, Packet& packet)
{
    QASSERT(packet.payload.size() >= sizeof(MON_VER));
    MON_VER& data = reinterpret_cast<MON_VER&>(*packet.payload.data());

    QLOGI("GPS Version SW:{} / HW:{}", data.swVersion, data.hwVersion);
}

///////////////////////////////

auto UBLOX::send_packet(Buses& buses, uint16_t msg, uint8_t const* payload, size_t payload_size) -> bool
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

    if (!write(buses, buffer.data(), off))
    {
        QLOGE("Cannot write message {} to GPS. Write failed: {}", static_cast<uint8_t>(msg), strerror(errno));
        return false;
    }

    //tcflush(m_fd, TCIOFLUSH);

    return true;
}

template<class T> auto UBLOX::send_packet(Buses& buses, uint16_t msg, T const& data) -> bool
{
    static_assert(sizeof(T) < 200, "Message too big");

    auto res = send_packet(buses, msg, reinterpret_cast<uint8_t const*>(&data), sizeof(T));

    return res;
}

template<class T> auto UBLOX::send_packet_with_retry(Buses& buses, uint16_t msg, T const& data, q::Clock::duration timeout, size_t retries) -> bool
{
    m_ack.reset();
    for (size_t i = 0; i <= retries; i++)
    {
        send_packet(buses, msg, data);
        if (wait_for_ack(buses, timeout) && *m_ack == true)
        {
            return true;
        }
        QLOGI("\t retrying {}", i);
    }
    return false;
}


auto UBLOX::set_config(rapidjson::Value const& json) -> bool
{
    QLOG_TOPIC("ublox::set_config");

    sz::UBLOX::Config sz;
    autojsoncxx::error::ErrorStack result;
    if (!autojsoncxx::from_value(sz, json, result))
    {
        std::ostringstream ss;
        ss << result;
        QLOGE("Cannot deserialize UBLOX config data: {}", ss.str());
        return false;
    }

    *m_config = sz;
    return true;
}
auto UBLOX::get_config() const -> rapidjson::Document
{
    rapidjson::Document json;
    autojsoncxx::to_document(*m_config, json);
    return std::move(json);
}

auto UBLOX::get_init_params() const -> rapidjson::Document
{
    rapidjson::Document json;
    autojsoncxx::to_document(*m_init_params, json);
    return std::move(json);
}

auto UBLOX::send_message(rapidjson::Value const& /*json*/) -> rapidjson::Document
{
    return rapidjson::Document();
}


}
}
