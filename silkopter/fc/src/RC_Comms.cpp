#include "FCStdAfx.h"
#include "RC_Comms.h"
#include "utils/Timed_Scope.h"
#include "utils/hw/SPI_PIGPIO.h"
#include "utils/hw/SPI_Dev.h"

#include "hal.def.h"


namespace silk
{

constexpr uint8_t SDN_GPIO = 6;

#define USE_SPI_PIGPIO

#ifdef USE_SPI_PIGPIO
constexpr size_t SPI_PORT = 1;
constexpr size_t SPI_CHANNEL = 0;
constexpr size_t SPI_SPEED = 10000000;
#else
const char* SPI_DEVICE = "/dev/spidev1.0";
constexpr size_t SPI_SPEED = 8000000;
#endif

RC_Comms::RC_Comms(HAL& hal)
    : m_hal(hal)
{
}

RC_Comms::~RC_Comms()
{
    m_phy_data.thread_exit = true;
    m_phy_data.tx_queue.exit();
    m_phy_data.rx_queue.exit();

    if (m_phy_data.thread.joinable())
    {
        m_phy_data.thread.join();
    }
}

auto RC_Comms::start() -> bool
{
    silk::hal::IUAV_Descriptor::Comms const& comms_settings = m_hal.get_uav_descriptor()->get_comms();

    try
    {
#ifdef USE_SPI_PIGPIO
        if (m_phy.init_pigpio(SPI_PORT, SPI_CHANNEL, SPI_SPEED) != Phy::Init_Result::OK)
        {
            QLOGE("Cannot start phy");
            return false;
        }
#else
        if (m_phy.init_dev(SPI_DEVICE, SPI_SPEED) != Phy::Init_Result::OK)
        {
            QLOGE("Cannot start phy");
            return false;
        }
#endif

        Fec_Encoder::TX_Descriptor tx_descriptor;
        tx_descriptor.coding_k = comms_settings.get_fec_coding_k();
        tx_descriptor.coding_n = comms_settings.get_fec_coding_n();
        tx_descriptor.mtu = comms_settings.get_mtu();
        if (!m_fec_encoder_tx.init_tx(tx_descriptor))
        {
            QLOGE("Cannot start the tx fec encoder");
            return false;
        }

        m_fec_encoder_tx.on_tx_data_encoded = [this](void const* data, size_t size)
        {
            math::vec2u16 rezolution;
            {
                std::lock_guard<std::mutex> lg(m_rezolution_mutex);
                rezolution = m_rezolution;
            }

            Phy_Data::Packet_ptr packet = m_phy_data.packet_pool.acquire();
            packet->resize(sizeof(rc_comms::Packet_Type::VIDEO) + sizeof(rezolution) + size);
            rc_comms::Packet_Type packet_type = rc_comms::Packet_Type::VIDEO;
            size_t offset = 0;
            util::serialization::serialize(*packet, packet_type, offset);
            util::serialization::serialize(*packet, rezolution, offset);
            packet->resize(offset + size);
            memcpy(packet->data() + offset, data, size);

            m_phy_data.tx_queue.push_back(packet, true);
        };

        m_phy.set_rate(static_cast<Phy::Rate>(comms_settings.get_rate()));
        m_phy.set_power(comms_settings.get_tx_power());
        m_phy.set_channel(comms_settings.get_channel());

//        util::comms::Video_Streamer::TX_Descriptor tx_descriptor;
//        tx_descriptor.interface = comms_settings.get_video_wlan_interface();
//        tx_descriptor.coding_k = comms_settings.get_fec_coding_k();
//        tx_descriptor.coding_n = comms_settings.get_fec_coding_n();

//        m_is_connected = m_rc_phy.init(*m_spi, SDN_GPIO) &&
//                         m_rc_protocol.init(2, 3) &&
//                         m_video_streamer.init_tx(tx_descriptor);
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

    m_phy_data.thread = std::thread(std::bind(&RC_Comms::phy_thread_proc, this));


//    m_rc_phy.set_channel(comms_settings.get_rc_channel());
//    m_rc_phy.set_xtal_adjustment(comms_settings.get_rc_xtal_ajdustment());

    //m_rc_phy.set_rate(100);
//    m_rc_protocol.add_periodic_packet(std::chrono::milliseconds(30), std::bind(&RC_Comms::compute_multirotor_state_packet, this, std::placeholders::_1, std::placeholders::_2));
//    m_rc_protocol.reset_session();

    QLOGI("Started comms");

    return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

void RC_Comms::phy_thread_proc()
{
    Phy_Data::Packet_ptr rx_packet = m_phy_data.packet_pool.acquire();
    rx_packet->resize(Phy::MAX_PAYLOAD_SIZE);

    while (!m_phy_data.thread_exit)
    {
        Phy_Data::Packet_ptr tx_packet;
        m_phy_data.tx_queue.pop_front_timeout(tx_packet, std::chrono::milliseconds(5));

        if (tx_packet)
        {
            m_phy.send_data(tx_packet->data(), tx_packet->size());
        }

        size_t rx_size = 0;
        int rx_rssi = 0;
        if (m_phy.receive_data(rx_packet->data(), rx_size, rx_rssi))
        {
            rx_packet->resize(rx_size);
            m_phy_data.rx_queue.push_back_timeout(rx_packet, std::chrono::milliseconds(5));
            rx_packet = m_phy_data.packet_pool.acquire();
            rx_packet->resize(Phy::MAX_PAYLOAD_SIZE);
        }
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////

auto RC_Comms::is_connected() const -> bool
{
    return m_is_connected;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

Phy const& RC_Comms::get_phy() const
{
    return m_phy;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

Phy& RC_Comms::get_phy()
{
    return m_phy;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

void RC_Comms::process_rx_packet(rc_comms::Packet_Type packet_type, std::vector<uint8_t> const& data, size_t offset)
{
//    m_rx_packet.rx_dBm = packet.rx_dBm;
//    m_rx_packet.tx_dBm = packet.tx_dBm;
//    m_rx_packet.rx_timepoint = packet.rx_timepoint;

    if (packet_type == rc_comms::Packet_Type::MULTIROTOR_COMMANDS)
    {
        stream::IMultirotor_Commands::Value value;
        if (util::serialization::deserialize(data, value, offset))
        {
            std::lock_guard<std::mutex> lg(m_new_multirotor_commands_mutex);
            m_new_multirotor_commands = value;
        }
        else
        {
            QLOGW("Cannot deserialize incoming multirotor state value");
        }
    }
    else if (packet_type == rc_comms::Packet_Type::CAMERA_COMMANDS)
    {
        stream::ICamera_Commands::Value value;
        if (util::serialization::deserialize(data, value, offset))
        {
            std::lock_guard<std::mutex> lg(m_new_camera_commands_mutex);
            m_new_camera_commands = value;
        }
        else
        {
            QLOGW("Cannot deserialize incoming camera state value");
        }
    }
    else
    {
        QLOGW("Unknown incoming packet type: {}", static_cast<int>(packet_type));
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////

void RC_Comms::process_received_data(std::vector<uint8_t> const& data)
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

boost::optional<stream::IMultirotor_Commands::Value> const& RC_Comms::get_multirotor_commands() const
{
    return m_multirotor_commands;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

boost::optional<stream::ICamera_Commands::Value> const& RC_Comms::get_camera_commands() const
{
    return m_camera_commands;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

void RC_Comms::set_multirotor_state(stream::IMultirotor_State::Value const& value)
{
    std::lock_guard<std::mutex> lg(m_multirotor_state_mutex);
    m_multirotor_state = value;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

void RC_Comms::add_video_data(stream::IVideo::Value const& value)
{
    m_fec_encoder_tx.add_tx_packet(value.data.data(), value.data.size(), false);

    {
        std::lock_guard<std::mutex> lg(m_rezolution_mutex);
        m_rezolution = value.resolution;
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////

void RC_Comms::process()
{
    if (!is_connected())
    {
        return;
    }

    {
        std::lock_guard<std::mutex> lg(m_new_camera_commands_mutex);
        m_camera_commands = m_new_camera_commands;
        m_new_camera_commands = boost::none;
    }
//    m_camera_commands = stream::ICamera_Commands::Value();
//    m_camera_commands->quality = stream::ICamera_Commands::Quality::HIGH;

    {
        std::lock_guard<std::mutex> lg(m_new_multirotor_commands_mutex);
        m_multirotor_commands = m_new_multirotor_commands;
        m_new_multirotor_commands = boost::none;
    }

    auto now = Clock::now();
    if (now - m_last_multirotor_state_sent_tp > std::chrono::milliseconds(100))
    {
        m_last_multirotor_state_sent_tp = now;

        stream::IMultirotor_State::Value state;
        {
            std::lock_guard<std::mutex> lg(m_multirotor_state_mutex);
            state = m_multirotor_state;
        }
        Phy_Data::Packet_ptr packet = m_phy_data.packet_pool.acquire();
        packet->clear();
        packet->push_back(static_cast<uint8_t>(silk::rc_comms::Packet_Type::MULTIROTOR_STATE));
        size_t off = packet->size();
        util::serialization::serialize(*packet, state, off);
        m_phy_data.tx_queue.push_back(packet, false);
    }

    if (now - m_last_phy_received_tp >= std::chrono::milliseconds(1))
    {
        m_last_phy_received_tp = now;

        Phy_Data::Packet_ptr rx_packet;
        while (m_phy_data.rx_queue.pop_front(rx_packet, false))
        {
            process_received_data(*rx_packet);
        }
    }
}

}
