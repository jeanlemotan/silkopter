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
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////

Comms::~Comms()
{
    m_phy_data.thread_exit = true;
    m_phy_data.tx_queue.exit();
    m_phy_data.rx_queue.exit();

    if (m_phy_data.thread.joinable())
    {
        m_phy_data.thread.join();
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////

bool Comms::start()
{
#if !defined RASPBERRY_PI
    return true;
#endif

    disconnect();

    settings::Settings::Comms const& comms_settings = m_hal.get_settings().get_comms();

    try
    {
        if (m_phy.init_dev(SPI_DEVICE, SPI_SPEED) != Phy::Init_Result::OK)
        {
            QLOGE("Cannot start phy");
            return false;
        }

        m_phy.set_rate(static_cast<Phy::Rate>(comms_settings.get_rate()));
        m_phy.set_power(comms_settings.get_tx_power());
        m_phy.set_channel(comms_settings.get_channel());

        m_is_connected = true;
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

    m_phy_data.thread = std::thread(std::bind(&Comms::phy_thread_proc, this));

    QLOGI("Started receiving video");

    return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

bool Comms::create_fec_encoder_rx(Fec_Encoder::RX_Descriptor const& descriptor)
{
    QLOGI("Creating RX FEC with K={}, N={}, MTU={}", descriptor.coding_k, descriptor.coding_n, descriptor.mtu);
    m_fec_encoder_rx.reset(new Fec_Encoder);
    if (!m_fec_encoder_rx->init_rx(descriptor))
    {
        QLOGE("Cannot start the rx fec encoder");
        m_fec_encoder_rx.reset();
        return false;
    }

    m_fec_encoder_rx->on_rx_data_decoded = [this](void const* data, size_t size)
    {
        rc_comms::Video_Header video_header;
        {
            std::lock_guard<std::mutex> lg(m_video_header_mutex);
            video_header = m_video_header;
        }

        math::vec2u16 resolution(video_header.width, video_header.height);
        if (on_video_data_received)
        {
            on_video_data_received(data, size, resolution);
        }
    };

    return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

void Comms::phy_thread_proc()
{
    Phy_Data::Packet_ptr rx_packet = m_phy_data.packet_pool.acquire();
    rx_packet->resize(Phy::MAX_PAYLOAD_SIZE);

    while (!m_phy_data.thread_exit)
    {
        Phy_Data::Packet_ptr tx_packet;
        m_phy_data.tx_queue.pop_front(tx_packet, false);

        if (tx_packet)
        {
            m_phy.send_data(tx_packet->data(), tx_packet->size());
        }

        size_t rx_size = 0;
        int rx_rssi = 0;
        while (m_phy.receive_data(rx_packet->data(), rx_size, rx_rssi))
        {
            rx_packet->resize(rx_size);
            m_phy_data.rx_queue.push_back_timeout(rx_packet, std::chrono::milliseconds(5));
            rx_packet = m_phy_data.packet_pool.acquire();
            rx_packet->resize(Phy::MAX_PAYLOAD_SIZE);
        }
    }
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

Phy const& Comms::get_phy() const
{
    return m_phy;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

Phy& Comms::get_phy()
{
    return m_phy;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

bool Comms::sent_multirotor_commands_packet()
{
    if (Clock::now() - m_multirotor_commands_last_valid_tp < std::chrono::milliseconds(500))
    {
        Phy_Data::Packet_ptr packet = m_phy_data.packet_pool.acquire();
        packet->clear();
        packet->push_back(static_cast<uint8_t>(silk::rc_comms::Packet_Type::MULTIROTOR_COMMANDS));
        size_t off = packet->size();
        util::serialization::serialize(*packet, m_multirotor_commands, off);
        m_phy_data.tx_queue.push_back(packet, false);
        return true;
    }
    else
    {
        //commands are out of date - don't send them
        return false;
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////

bool Comms::send_camera_commands_packet()
{
    if (Clock::now() - m_camera_commands_last_valid_tp < std::chrono::milliseconds(500))
    {
        Phy_Data::Packet_ptr packet = m_phy_data.packet_pool.acquire();
        packet->clear();
        packet->push_back(static_cast<uint8_t>(silk::rc_comms::Packet_Type::CAMERA_COMMANDS));
        size_t off = packet->size();
        util::serialization::serialize(*packet, m_camera_commands, off);
        m_phy_data.tx_queue.push_back(packet, false);
        return true;
    }
    else
    {
        //commands are out of date - don't send them
        return false;
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////

void Comms::process_rx_packet(rc_comms::Packet_Type packet_type, std::vector<uint8_t> const& data, size_t offset)
{
//    m_rx_packet.rx_dBm = packet.rx_dBm;
//    m_rx_packet.tx_dBm = packet.tx_dBm;
//    m_rx_packet.rx_timepoint = packet.rx_timepoint;

    bool dsz_ok = false;
    if (packet_type == rc_comms::Packet_Type::MULTIROTOR_STATE)
    {
        std::lock_guard<std::mutex> lg(m_samples_mutex);
        stream::IMultirotor_State::Value value = m_multirotor_state;
        dsz_ok = util::serialization::deserialize(data, value, offset);
        if (dsz_ok)
        {
            m_multirotor_state = value;
        }
    }
    else if (packet_type == rc_comms::Packet_Type::VIDEO)
    {
        rc_comms::Video_Header video_header;
        if (util::serialization::deserialize(data, video_header, offset))
        {
            {
                std::lock_guard<std::mutex> lg(m_video_header_mutex);
                m_video_header = video_header;
            }

            Fec_Encoder::RX_Descriptor new_descriptor;
            new_descriptor.coding_k = video_header.fec_k;
            new_descriptor.coding_n = video_header.fec_n;
            new_descriptor.mtu = Fec_Encoder::compute_mtu_from_packet_size(data.size() - offset);

            bool recreate_fec = true;
            if (m_fec_encoder_rx)
            {
                //recreate the FEC if quality changed
                Fec_Encoder::Descriptor const& descriptor = m_fec_encoder_rx->get_descriptor();
                recreate_fec = new_descriptor.coding_k != descriptor.coding_k ||
                        new_descriptor.coding_n != descriptor.coding_n ||
                        new_descriptor.mtu != descriptor.mtu;
            }

            if (recreate_fec)
            {
                create_fec_encoder_rx(new_descriptor);
            }

            if (m_fec_encoder_rx)
            {
                m_fec_encoder_rx->add_rx_packet(data.data() + offset, data.size() - offset, false);
            }
            dsz_ok = true;
        }
    }
    else
    {
        QLOGW("Unknown incoming packet type: {}", static_cast<int>(packet_type));
    }

    if (!dsz_ok)
    {
        QLOGW("Cannot deserialize incoming packet");
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////

void Comms::process_received_data(std::vector<uint8_t> const& data)
{
    if (data.size() < sizeof(rc_comms::Packet_Type))
    {
        QLOGE("Invalid data received");
        return;
    }

    rc_comms::Packet_Type packet_type = *reinterpret_cast<const rc_comms::Packet_Type*>(data.data());
    process_rx_packet(packet_type, data, sizeof(rc_comms::Packet_Type));
}

///////////////////////////////////////////////////////////////////////////////////////////////////

int8_t Comms::get_rx_dBm() const
{
    return 0;//m_rx_packet.rx_dBm;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

int8_t Comms::get_tx_dBm() const
{
    return 0;//m_rx_packet.tx_dBm;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

Clock::time_point Comms::get_last_rx_tp() const
{
    return Clock::now();//m_rx_packet.rx_timepoint;
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
    m_multirotor_commands_last_valid_tp = Clock::now();
}

///////////////////////////////////////////////////////////////////////////////////////////////////

void Comms::send_camera_commands_value(stream::ICamera_Commands::Value const& value)
{
    m_camera_commands = value;
    m_camera_commands_last_valid_tp = Clock::now();
}

///////////////////////////////////////////////////////////////////////////////////////////////////

void Comms::process()
{
    Clock::time_point now = Clock::now();
    if (now - m_telemetry_tp >= std::chrono::milliseconds(30))
    {
        m_telemetry_tp = now;
    }
//    if (now - m_rx_packet.rx_timepoint >= std::chrono::seconds(2))
//    {
//        m_rx_packet.rx_dBm = 0;
//        m_rx_packet.tx_dBm = 0;
//    }

    {
        Phy_Data::Packet_ptr rx_packet;
        while (m_phy_data.rx_queue.pop_front(rx_packet, false))
        {
            process_received_data(*rx_packet);
        }
    }

    if (now - m_multirotor_commands_last_sent_tp > std::chrono::milliseconds(20))
    {
        m_multirotor_commands_last_sent_tp = now;
        sent_multirotor_commands_packet();
    }
    if (now - m_camera_commands_last_sent_tp > std::chrono::milliseconds(100))
    {
        m_camera_commands_last_sent_tp = now;
        send_camera_commands_packet();
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////

}
