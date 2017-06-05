#include "Comms.h"

#include "common/node/INode.h"
#include "common/node/IPilot.h"

#include "common/Comm_Data.h"

#include "utils/comms/UDP_Socket.h"
#include "utils/comms/RFMON_Socket.h"
#include "utils/comms/RF4463F30_Socket.h"
#include "utils/hw/SPI_PIGPIO.h"
#include "utils/hw/SPI_Dev.h"

#include "HAL.h"
#include "settings.def.h"

namespace silk
{

//constexpr char const* VIDEO_INTERFACE1 = "wlan2";
//constexpr char const* VIDEO_INTERFACE2 = "wlan3";

//#define USE_SPI_PIGPIO

#ifdef USE_SPI_PIGPIO
constexpr size_t SPI_PORT = 0;
constexpr size_t SPI_CHANNEL = 0;
constexpr size_t SPI_SPEED = 4000000;
#else
const char* SPI_DEVICE = "/dev/spidev0.0";
constexpr size_t SPI_SPEED = 10000000;
#endif


///////////////////////////////////////////////////////////////////////////////////////////////////

Comms::Comms(HAL& hal)
    : m_hal(hal)
    , m_rc_phy(true)
    , m_rc_protocol(m_rc_phy, std::bind(&Comms::process_rx_packet, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3))
    //, m_video_streamer()
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////

bool Comms::start()
{
    disconnect();

    settings::Settings::Comms const& comms = m_hal.get_settings().get_comms();

    util::comms::Video_Streamer::RX_Descriptor rx_descriptor;
    rx_descriptor.interfaces = comms.get_video_wlan_interfaces();
    rx_descriptor.coding_k = comms.get_video_fec_coding_k();
    rx_descriptor.coding_n = comms.get_video_fec_coding_n();
    rx_descriptor.max_latency = std::chrono::milliseconds(comms.get_video_max_latency_ms());
    rx_descriptor.reset_duration = std::chrono::milliseconds(comms.get_video_reset_duration_ms());

#ifdef USE_SPI_PIGPIO
    util::hw::SPI_PIGPIO* spi = new util::hw::SPI_PIGPIO();
    m_spi.reset(spi);

    ts::Result<void> result = spi->init(SPI_PORT, SPI_CHANNEL, SPI_SPEED, 0);
#else
    util::hw::SPI_Dev* spi = new util::hw::SPI_Dev();
    m_spi.reset(spi);

    ts::Result<void> result = spi->init(SPI_DEVICE, SPI_SPEED);
#endif
    if (result != ts::success)
    {
        QLOGW("Cannot start spi: {}", result.error().what());
        return false;
    }

    try
    {
        m_is_connected = m_rc_phy.init(*m_spi,
                                       comms.get_rc_sdn_gpio())
                      && m_rc_protocol.init(2, 3)
                      && m_video_streamer.init_rx(rx_descriptor);
    }
    catch(std::exception e)
    {
        m_is_connected = false;
    }

    if (!m_is_connected)
    {
        QLOGW("Cannot start comms");
        return false;
    }

    uint8_t channel = m_hal.get_settings().get_comms().get_rc_channel();
    if (!m_rc_phy.set_channel(channel))
    {
        QLOGW("Cannot set channel {}", channel);
        return false;
    }
    m_rc_phy.set_xtal_adjustment(m_hal.get_settings().get_comms().get_rc_xtal_adjustment());

    //m_rc_phy.set_rate(100);
    m_rc_protocol.add_periodic_packet(std::chrono::milliseconds(30), std::bind(&Comms::compute_multirotor_commands_packet, this, std::placeholders::_1, std::placeholders::_2));
    m_rc_protocol.add_periodic_packet(std::chrono::milliseconds(200), std::bind(&Comms::compute_camera_commands_packet, this, std::placeholders::_1, std::placeholders::_2));

    //send connected message so the UAV can send vital data back
    m_rc_protocol.reset_session();
    m_rc_protocol.send_reliable_packet(static_cast<uint8_t>(rc_comms::Packet_Type::RC_CONNECTED), nullptr, 0);

    m_video_streamer.on_data_received = std::bind(&Comms::send_video_to_viewers, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);

    QLOGI("Started receiving video");

    return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

void Comms::disconnect()
{
    reset();
    m_is_connected = false;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

bool Comms::is_connected() const
{
    return m_is_connected;//m_rcp != nullptr && m_rcp->is_connected();
}

///////////////////////////////////////////////////////////////////////////////////////////////////

void Comms::reset()
{
    m_last_req_id = 0;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

util::comms::RC_Phy const& Comms::get_rc_phy() const
{
    return m_rc_phy;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

util::comms::RC_Phy& Comms::get_rc_phy()
{
    return m_rc_phy;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

Remote_Viewer_Server const& Comms::get_remote_viewer_server() const
{
    return m_remote_viewer_server;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

Remote_Viewer_Server& Comms::get_remote_viewer_server()
{
    return m_remote_viewer_server;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

util::comms::Video_Streamer const& Comms::get_video_streamer() const
{
    return m_video_streamer;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

util::comms::Video_Streamer& Comms::get_video_streamer()
{
    return m_video_streamer;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

bool Comms::compute_multirotor_commands_packet(util::comms::RC_Protocol::Buffer& buffer, uint8_t& packet_type)
{
    if (Clock::now() - m_multirotor_commands_tp < std::chrono::milliseconds(500))
    {
        packet_type = static_cast<uint8_t>(rc_comms::Packet_Type::MULTIROTOR_COMMANDS);

        size_t off = buffer.size();
        util::serialization::serialize(buffer, m_multirotor_commands, off);

        return true;
    }
    else
    {
        //commands are out of date - don't send them
        return false;
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////

bool Comms::compute_camera_commands_packet(util::comms::RC_Protocol::Buffer& buffer, uint8_t& packet_type)
{
    if (Clock::now() - m_camera_commands_tp < std::chrono::milliseconds(500))
    {
        packet_type = static_cast<uint8_t>(rc_comms::Packet_Type::CAMERA_COMMANDS);

        size_t off = buffer.size();
        util::serialization::serialize(buffer, m_camera_commands, off);

        return true;
    }
    else
    {
        //commands are out of date - don't send them
        return false;
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////

void Comms::process_rx_packet(util::comms::RC_Protocol::RX_Packet const& packet, uint8_t* data, size_t size)
{
    m_rx_packet.rx_dBm = packet.rx_dBm;
    m_rx_packet.tx_dBm = packet.tx_dBm;
    m_rx_packet.rx_timepoint = packet.rx_timepoint;

    if (packet.packet_type == 0xFF)
    {
        //send_video_to_viewers(data, size, math::vec2u16(640, 480));
        return;
    }

    m_rx_packet_sz_buffer.resize(size);
    if (size > 0)
    {
        memcpy(m_rx_packet_sz_buffer.data(), data, size);
    }


    bool dsz_ok = false;
    if (packet.packet_type == static_cast<uint8_t>(rc_comms::Packet_Type::MULTIROTOR_STATE_PART1))
    {
        size_t off = 0;
        std::lock_guard<std::mutex> lg(m_samples_mutex);
        stream::IMultirotor_State::Value value = m_multirotor_state;
        dsz_ok = util::serialization::deserialize_part1(m_rx_packet_sz_buffer, value, off);
        if (dsz_ok)
        {
            m_multirotor_state = value;
        }
    }
    else if (packet.packet_type == static_cast<uint8_t>(rc_comms::Packet_Type::MULTIROTOR_STATE_PART2))
    {
        size_t off = 0;
        std::lock_guard<std::mutex> lg(m_samples_mutex);
        stream::IMultirotor_State::Value value = m_multirotor_state;
        dsz_ok = util::serialization::deserialize_part2(m_rx_packet_sz_buffer, value, off);
        if (dsz_ok)
        {
            m_multirotor_state = value;
        }
    }
    else if (packet.packet_type == static_cast<uint8_t>(rc_comms::Packet_Type::MULTIROTOR_STATE_HOME))
    {
        size_t off = 0;
        std::lock_guard<std::mutex> lg(m_samples_mutex);
        stream::IMultirotor_State::Value value = m_multirotor_state;
        dsz_ok = util::serialization::deserialize_home(m_rx_packet_sz_buffer, value, off);
        if (dsz_ok)
        {
            m_multirotor_state = value;
        }
    }
    else
    {
        QLOGW("Unknown incoming packet type: {}", static_cast<int>(packet.packet_type));
    }

    if (!dsz_ok)
    {
        QLOGW("Cannot deserialize incoming multirotor state value");
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////

void Comms::send_video_to_viewers(void const* data, size_t size, math::vec2u16 const& resolution)
{
    std::lock_guard<std::mutex> lg(m_samples_mutex);

    if (resolution != m_video_resolution)
    {
        m_video_resolution = resolution;
    }

    if (size > 0)
    {
        m_remote_viewer_server.send_video_data(data, size, resolution);
//        uint8_t const* data = reinterpret_cast<uint8_t const*>(_data);
//        size_t offset = m_video_data.size();
//        m_video_data.resize(offset + size);
//        memcpy(m_video_data.data() + offset, data, size);

//        //keep the buffer from growing too much
//        constexpr size_t MAX_VIDEO_DATA_SIZE = 1024*1024;
//        if (m_video_data.size() > MAX_VIDEO_DATA_SIZE)
//        {
//            size_t del = m_video_data.size() - MAX_VIDEO_DATA_SIZE;
//            m_video_data.erase(m_video_data.begin(), m_video_data.begin() + del);
//        }
    }

//    FILE* f = fopen("a.h264", "a+");
//    fwrite(_data, size, 1, f);
//    fclose(f);
}

///////////////////////////////////////////////////////////////////////////////////////////////////

void Comms::send_telemetry_to_viewers()
{
    m_remote_viewer_server.send_telemetry(m_multirotor_commands, m_multirotor_state);
}

///////////////////////////////////////////////////////////////////////////////////////////////////

int8_t Comms::get_rx_dBm() const
{
    return m_rx_packet.rx_dBm;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

int8_t Comms::get_tx_dBm() const
{
    return m_rx_packet.tx_dBm;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

Clock::time_point Comms::get_last_rx_tp() const
{
    return m_rx_packet.rx_timepoint;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

//void Comms::get_video_data(std::vector<uint8_t>& dst, math::vec2u16& resolution)
//{
//    std::lock_guard<std::mutex> lg(m_samples_mutex);
//    size_t offset = dst.size();
//    size_t size = m_video_data.size();
//    if (size > 0)
//    {
//        dst.resize(offset + size);
//        memcpy(dst.data() + offset, m_video_data.data(), size);
//        m_video_data.clear();
//    }
//    resolution = m_video_resolution;
//}

///////////////////////////////////////////////////////////////////////////////////////////////////

stream::IMultirotor_State::Value Comms::get_multirotor_state() const
{
    std::lock_guard<std::mutex> lg(m_samples_mutex);
    return m_multirotor_state;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

void Comms::send_multirotor_commands_value(stream::IMultirotor_Commands::Value const& value)
{
    m_multirotor_commands = value;
    m_multirotor_commands_tp = Clock::now();
}

///////////////////////////////////////////////////////////////////////////////////////////////////

void Comms::send_camera_commands_value(stream::ICamera_Commands::Value const& value)
{
    m_camera_commands = value;
    m_camera_commands_tp = Clock::now();
}

///////////////////////////////////////////////////////////////////////////////////////////////////

void Comms::process()
{
    m_video_streamer.process();

    Clock::time_point now = Clock::now();
    if (now - m_telemetry_tp >= std::chrono::milliseconds(30))
    {
        m_telemetry_tp = now;
        send_telemetry_to_viewers();
    }

    m_remote_viewer_server.process();

//    if (Clock::now() - get_last_rx_tp() > std::chrono::seconds(5))
//    {
//        static FILE* fff = nullptr;
//        if (!fff)
//        {
//            srand(time(nullptr));
//            fff = fopen("a.h264", "rb");
//            if (!fff)
//            {
//                exit(1);
//            }
//        }

//        static std::vector<uint8_t> video_data;
//        video_data.resize((rand() % 3280) + 512);
//        int r = fread(video_data.data(), 1, video_data.size(), fff);
//        if (r == 0)
//        {
//            QLOGI("DONE, REWIND!!!!!");
//            fseek(fff, 0, SEEK_SET);
//        }
//        video_data.resize(r);

//        m_remote_viewer_server.send_data(video_data.data(), video_data.size(), math::vec2u16(0, 0), m_multirotor_state);
//    }

//    static Clock::time_point xxx = Clock::time_point(Clock::duration::zero());
//    if (m_multirotor_state.mode != m_multirotor_commands.mode)
//    {
//        if (xxx.time_since_epoch().count() == 0)
//        {
//            xxx = Clock::now();
//        }
//        if (Clock::now() - xxx > std::chrono::seconds(1))
//        {
//            QLOGI("Simulated mode switch from {} to {}", m_multirotor_state.mode, m_multirotor_commands.mode);
//            m_multirotor_state.mode = m_multirotor_commands.mode;
//            xxx = Clock::time_point(Clock::duration::zero());
//        }
//    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////

}
