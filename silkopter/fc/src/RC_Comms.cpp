#include "FCStdAfx.h"
#include "RC_Comms.h"
#include "utils/Timed_Scope.h"
#include "utils/hw/SPI_PIGPIO.h"
#include "utils/hw/SPI_Dev.h"

#include "hal.def.h"


namespace silk
{

//#define USE_SPI_PIGPIO

#ifdef USE_SPI_PIGPIO
constexpr size_t SPI_PORT = 0;
constexpr size_t SPI_CHANNEL = 0;
constexpr size_t SPI_SPEED = 10000000;
#else
const char* SPI_DEVICE = "/dev/spidev0.0";
constexpr size_t SPI_SPEED = 10000000;
#endif

RC_Comms::RC_Comms(HAL& hal)
    : m_hal(hal)
{
    srand((size_t)this + clock());
    m_station_id = rand();
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

    std::shared_ptr<const hal::IUAV_Descriptor> uav_descriptor = m_hal.get_uav_descriptor();
    if (uav_descriptor)
    {
        silk::hal::IUAV_Descriptor::Comms const& s = uav_descriptor->get_comms();
        silk::hal::IUAV_Descriptor::Comms::Quality const& qs = s.get_high();

        QLOGI("Phy FEC: K={}, N={}, MTU={}", qs.get_fec_k(), qs.get_fec_n(), qs.get_mtu());
        m_phy.setup_fec_channel(qs.get_fec_k(), qs.get_fec_n(), qs.get_mtu());
        m_mtu = qs.get_mtu();

        QLOGI("Phy Rate: {}", qs.get_rate());
        m_phy.set_rate(static_cast<Phy::Rate>(qs.get_rate()));

        QLOGI("Phy TX Power: {}", s.get_tx_power());
        m_phy.set_power(s.get_tx_power());

        QLOGI("Phy Channel: {}", s.get_channel());
        m_phy.set_channel(s.get_channel());
    }
    else
    {
        m_phy_rate = static_cast<int>(Phy::Rate::RATE_B_2M_CCK);
        m_phy.set_rate(Phy::Rate::RATE_B_2M_CCK);
        m_phy.set_power(10);
        m_phy.set_channel(1);
    }

    m_is_connected = true;

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

//bool RC_Comms::create_fec_encoder_tx(Fec_Encoder::TX_Descriptor const& descriptor)
//{
//    QLOGI("Creating TX FEC with K={}, N={}, MTU={}", descriptor.coding_k, descriptor.coding_n, descriptor.mtu);
//    m_fec_encoder_tx.reset(new Fec_Encoder);
//    if (!m_fec_encoder_tx->init_tx(descriptor))
//    {
//        QLOGE("Cannot start the tx fec encoder");
//        m_fec_encoder_tx.reset();
//        return false;
//    }

//    m_fec_encoder_tx->on_tx_data_encoded = [this](void const* data, size_t size)
//    {
//        rc_comms::Video_Header video_header;
//        {
//            std::lock_guard<std::mutex> lg(m_video_header_mutex);
//            video_header = m_video_header;
//        }

//        Phy_Data::Packet_ptr packet = m_phy_data.packet_pool.acquire();
//        packet->resize(sizeof(rc_comms::Packet_Type::VIDEO) + sizeof(video_header) + size);
//        rc_comms::Packet_Type packet_type = rc_comms::Packet_Type::VIDEO;
//        size_t offset = 0;
//        util::serialization::serialize(*packet, packet_type, offset);
//        util::serialization::serialize(*packet, video_header, offset);
//        packet->resize(offset + size);
//        memcpy(packet->data() + offset, data, size);

//        m_phy_data.tx_queue.push_back(packet, true);
//    };

//    return true;
//}

///////////////////////////////////////////////////////////////////////////////////////////////////

void RC_Comms::phy_thread_proc()
{
    Phy_Data::Packet_ptr rx_packet = m_phy_data.packet_pool.acquire();

    static size_t bps = 0;
    static size_t pps = 0;
    static Clock::time_point tp;

    while (!m_phy_data.thread_exit)
    {
        if (Clock::now() - tp >= std::chrono::seconds(1))
        {
            QLOGI("bps: {}, pps: {}", bps, pps);
            bps = 0;
            pps = 0;
            tp = Clock::now();
        }

        size_t count = 10;
        Phy_Data::Packet_ptr tx_packet;
        m_phy_data.tx_queue.pop_front_timeout(tx_packet, std::chrono::milliseconds(5));
        while (tx_packet)
        {
            pps++;
            bps += tx_packet->payload.size();
            m_phy.send_data(tx_packet->payload.data(), tx_packet->payload.size(), tx_packet->use_fec);
            tx_packet = nullptr;
            if (count-- > 0)
            {
                m_phy_data.tx_queue.pop_front(tx_packet, false);
                std::this_thread::yield();
            }
        }

        size_t rx_size = 0;
        int16_t rx_rssi = std::numeric_limits<int16_t>::lowest();
        rx_packet->payload.resize(Phy::MAX_PAYLOAD_SIZE);
        if (m_phy.receive_data(rx_packet->payload.data(), rx_size, rx_rssi))
        {
            rx_packet->payload.resize(rx_size);
            m_phy_data.rx_queue.push_back_timeout(rx_packet, std::chrono::milliseconds(1));

            if (rx_rssi != std::numeric_limits<int16_t>::lowest())
            {
                std::lock_guard<std::mutex> lg(m_phy_data.rx_rssi_mutex);
                m_phy_data.rx_rssi_count++;
                m_phy_data.rx_rssi_accumulated += rx_rssi;
            }

            //get anothger packet
            rx_packet = m_phy_data.packet_pool.acquire();
        }

        if (m_new_phy_rate != m_phy_rate)
        {
            m_phy_rate = m_new_phy_rate.load();
            m_phy.set_rate(static_cast<Phy::Rate>(m_new_phy_rate.load()));
        }

        std::this_thread::yield();
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

    rc_comms::Packet_Header const& packet_header = *reinterpret_cast<const rc_comms::Packet_Header*>(data.data());
    process_rx_packet((rc_comms::Packet_Type)packet_header.packet_type, data, sizeof(rc_comms::Packet_Header));
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

rc_comms::Packet_Header RC_Comms::prepare_packet_header(rc_comms::Packet_Type packet_type, uint32_t packet_index) const
{
    rc_comms::Packet_Header packet_header;
    packet_header.packet_type = (uint32_t)packet_type;
    packet_header.station_id = m_station_id;
    packet_header.packet_index = packet_index;
    packet_header.rx_rssi = m_rx_rssi;
    return packet_header;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

void RC_Comms::add_video_data(stream::IVideo::Value const& value)
{
    silk::hal::IUAV_Descriptor::Comms const& comms_settings = m_hal.get_uav_descriptor()->get_comms();

    size_t offset = m_video_data_buffer.size();
    m_video_data_buffer.resize(offset + value.data.size());
    memcpy(m_video_data_buffer.data() + offset, value.data.data(), value.data.size());

    rc_comms::Packet_Header packet_header = prepare_packet_header(rc_comms::Packet_Type::VIDEO, 0u);

    rc_comms::Video_Header video_header;
    video_header.width = value.resolution.x;
    video_header.height = value.resolution.y;

    size_t payload_size = m_mtu - sizeof(packet_header) - sizeof(video_header);
    while (m_video_data_buffer.size() >= payload_size)
    {
        Phy_Data::Packet_ptr packet = m_phy_data.packet_pool.acquire();
        packet->payload.resize(m_mtu);
        packet->use_fec = true;
        size_t offset = 0;
        packet_header.packet_index = ++m_phy_data.last_sent_packet_index;
        util::serialization::serialize(packet->payload, packet_header, offset);
        util::serialization::serialize(packet->payload, video_header, offset);
        memcpy(packet->payload.data() + offset, m_video_data_buffer.data(), payload_size);
        m_video_data_buffer.erase(m_video_data_buffer.begin(), m_video_data_buffer.begin() + payload_size);

        m_phy_data.tx_queue.push_back(packet, false);
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
    if (now - m_last_multirotor_state_sent_tp > std::chrono::milliseconds(30))
    {
        m_last_multirotor_state_sent_tp = now;

        stream::IMultirotor_State::Value state;
        {
            std::lock_guard<std::mutex> lg(m_multirotor_state_mutex);
            state = m_multirotor_state;
        }
        Phy_Data::Packet_ptr packet = m_phy_data.packet_pool.acquire();
        packet->use_fec = false;
        packet->payload.clear();

        rc_comms::Packet_Header packet_header = prepare_packet_header(rc_comms::Packet_Type::MULTIROTOR_STATE, ++m_phy_data.last_sent_packet_index);

        size_t off = 0;
        util::serialization::serialize(packet->payload, packet_header, off);
        util::serialization::serialize(packet->payload, state, off);
        m_phy_data.tx_queue.push_back(packet, false);
    }

    Clock::time_point tp = Clock::now();
    Phy_Data::Packet_ptr rx_packet;
    while (m_phy_data.rx_queue.pop_front(rx_packet, false) && Clock::now() - tp <= std::chrono::milliseconds(5))
    {
        process_received_data(rx_packet->payload);
    }

    if (now - m_last_rx_rssi_tp >= std::chrono::seconds(1))
    {
        m_last_rx_rssi_tp = now;

        std::lock_guard<std::mutex> lg(m_phy_data.rx_rssi_mutex);
        if (m_phy_data.rx_rssi_count > 0)
        {
            m_rx_rssi = (int32_t)m_phy_data.rx_rssi_accumulated / (int32_t)m_phy_data.rx_rssi_count;
            m_phy_data.rx_rssi_count = 0;
            m_phy_data.rx_rssi_accumulated = 0;
        }
        else
        {
            m_rx_rssi = std::numeric_limits<int16_t>::lowest();
        }
    }
}

}
