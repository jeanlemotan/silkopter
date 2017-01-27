#include "Comms.h"

#include "common/node/INode.h"
#include "common/node/IPilot.h"

#include "common/Comm_Data.h"

#include "utils/comms/UDP_Socket.h"
#include "utils/comms/RFMON_Socket.h"
#include "utils/comms/RF4463F30_Socket.h"

#include "settings.def.h"

namespace silk
{

//constexpr char const* VIDEO_INTERFACE1 = "wlan2";
//constexpr char const* VIDEO_INTERFACE2 = "wlan3";

//constexpr uint8_t RC_SDN_GPIO = 6;
//constexpr uint8_t RC_NIRQ_GPIO = 26;
//constexpr char const* RC_SPI_DEVICE = "/dev/spidev0.0";
//constexpr size_t RC_SPEED = 16000000;

extern settings::Settings s_settings;

///////////////////////////////////////////////////////////////////////////////////////////////////

Comms::Comms()
    : m_rc_phy(true)
    , m_rc_protocol(m_rc_phy, std::bind(&Comms::process_rx_packet, this, std::placeholders::_1))
    , m_video_streamer()
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////

bool Comms::start()
{
    disconnect();

    settings::Settings::Comms const& comms = s_settings.get_comms();

    util::comms::Video_Streamer::RX_Descriptor rx_descriptor;
    rx_descriptor.interfaces = comms.get_video_interfaces();
    rx_descriptor.coding_k = comms.get_video_coding_k();
    rx_descriptor.coding_n = comms.get_video_coding_n();
    rx_descriptor.max_latency = std::chrono::milliseconds(comms.get_video_max_latency_ms());
    rx_descriptor.reset_duration = std::chrono::milliseconds(comms.get_video_reset_duration_ms());

    try
    {
        m_is_connected = m_rc_phy.init(comms.get_rc_spi_device(),
                                       comms.get_rc_spi_speed(),
                                       comms.get_rc_sdn_gpio(),
                                       comms.get_rc_nirq_gpio())
                && m_rc_protocol.init()
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

    //m_rc_phy.set_rate(100);
    m_rc_protocol.add_periodic_packet(std::chrono::milliseconds(30), std::bind(&Comms::compute_multirotor_commands_packet, this, std::placeholders::_1, std::placeholders::_2));

    m_video_streamer.on_data_received = std::bind(&Comms::handle_video, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);

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

size_t Comms::compute_multirotor_commands_packet(uint8_t* data, uint8_t& packet_type)
{
    packet_type = static_cast<uint8_t>(rc_comms::Packet_Type::MULTIROTOR_COMMANDS);

    size_t off = 0;
    util::serialization::serialize(m_serialization_buffer, m_multirotor_commands, off);

    memcpy(data, m_serialization_buffer.data(), off);

    return off;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

void Comms::process_rx_packet(util::comms::RC_Protocol::RX_Packet const& packet)
{
    m_rx_packet.rx_dBm = packet.rx_dBm;
    m_rx_packet.tx_dBm = packet.tx_dBm;
    m_rx_packet.rx_timepoint = packet.rx_timepoint;

    if (packet.packet_type == static_cast<uint8_t>(rc_comms::Packet_Type::MULTIROTOR_STATE))
    {
        size_t off = 0;
        stream::IMultirotor_State::Value value;
        if (util::serialization::deserialize(packet.payload, value, off))
        {
            std::lock_guard<std::mutex> lg(m_samples_mutex);
            m_multirotor_state = value;
        }
        else
        {
            QLOGW("Cannot deserialize incoming multirotor state value");
        }
    }
    else if (packet.packet_type == static_cast<uint8_t>(rc_comms::Packet_Type::HOME))
    {
        size_t off = 0;
        Home_Data data;
        if (util::serialization::deserialize(packet.payload, data, off))
        {
            std::lock_guard<std::mutex> lg(m_samples_mutex);
            m_home_data = data;
        }
        else
        {
            QLOGW("Cannot deserialize incoming home data");
        }
    }
    else
    {
        QLOGW("Unknown incoming packet type: {}", static_cast<int>(packet.packet_type));
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////

void Comms::handle_video(void const* data, size_t size, math::vec2u16 const& resolution)
{
    std::lock_guard<std::mutex> lg(m_samples_mutex);

    if (resolution != m_video_resolution)
    {
        m_video_resolution = resolution;
        m_video_data.clear();
    }

    if (size > 0)
    {
        m_remote_viewer_server.send_data(data, size, resolution, m_multirotor_state);
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
}

///////////////////////////////////////////////////////////////////////////////////////////////////

void Comms::process()
{
    m_video_streamer.process();
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

    static Clock::time_point xxx = Clock::time_point(Clock::duration::zero());
    if (m_multirotor_state.mode != m_multirotor_commands.mode)
    {
        if (xxx.time_since_epoch().count() == 0)
        {
            xxx = Clock::now();
        }
        if (Clock::now() - xxx > std::chrono::seconds(1))
        {
            QLOGI("Simulated mode switch from {} to {}", m_multirotor_state.mode, m_multirotor_commands.mode);
            m_multirotor_state.mode = m_multirotor_commands.mode;
            xxx = Clock::time_point(Clock::duration::zero());
        }
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////

}
