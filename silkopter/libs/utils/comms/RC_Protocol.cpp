#include "RC_Protocol.h"

namespace util
{
namespace comms
{

//#define LOG

RC_Protocol::RC_Protocol(RC_Phy& phy, RX_Callback rx_callback)
    : m_phy(phy)
    , m_rx_callback(rx_callback)
{
    m_phy.set_callbacks(std::bind(&RC_Protocol::compute_tx_data, this, std::placeholders::_1),
                        std::bind(&RC_Protocol::process_rx_data, this, std::placeholders::_1));

    //std::atomic_init(&m_crt_sent_packet_index, 0);
    //std::atomic_init(&m_received_packet_index, 0);
}

bool RC_Protocol::init()
{
    return true;
}

size_t RC_Protocol::get_mtu() const
{
    return m_phy.get_mtu() - sizeof(Header);
}

void RC_Protocol::add_periodic_packet(Clock::duration period, TX_Callback tx_callback)
{
    Periodic_Packet pp;
    pp.period = period;
    pp.tx_callback = tx_callback;
    m_periodic_packets.push_back(pp);
}

void RC_Protocol::send_packet(uint8_t packet_type, uint8_t const* data, size_t size)
{
    if (data == nullptr || size == 0)
    {
        QASSERT(false);
        return;
    }
    TX_Packet p;
    p.packet_type = packet_type;
    p.offset = 0;
    p.payload.resize(size);
    std::copy(data, data + size, p.payload.begin());

    m_tx_packet_queue.push_back(p);
}

size_t RC_Protocol::compute_tx_data(uint8_t* data)
{
    Header& header = *reinterpret_cast<Header*>(data);
    header.last_received_packet_index = m_received_packet_index;

    Clock::time_point now = Clock::now();

    {
        std::lock_guard<std::mutex> lg(m_periodic_packets_mutex);
        for (Periodic_Packet& p: m_periodic_packets)
        {
            if (now - p.last_tp >= p.period)
            {
                p.last_tp = now;

                uint8_t packet_type = 0;
                size_t size = p.tx_callback(data + sizeof(Header), packet_type);
                QASSERT(packet_type <= Header::MAX_PACKET_TYPE);

                header.packet_index = 0;
                header.packet_type = packet_type;
                header.last_packet = 1;

#ifdef LOG
                QLOGI("Send periodic packet type {}, size {}", packet_type, size);
#endif

                return sizeof(Header) + size;
            }
        }
    }

    {
        std::lock_guard<std::mutex> lg(m_tx_packet_queue_mutex);
        if (!m_tx_packet_queue.empty())
        {
            TX_Packet& packet = m_tx_packet_queue.front();
            size_t payload_size_left = packet.payload.size() - packet.offset;
            size_t size = std::min(payload_size_left, get_mtu());
            QASSERT(size > 0);

            memcpy(data + sizeof(Header), packet.payload.data() + packet.offset, size);

            header.packet_type = packet.packet_type;
            header.packet_index = m_crt_sent_packet_index;
            header.last_packet = (size == payload_size_left);

#ifdef LOG
            QLOGI("Sent fragment {} - {} out of {}KB", packet.offset, packet.offset + size, packet.payload.size());
#endif
            return sizeof(Header) + size;
        }
    }

    {
        header.last_packet = 1;
        header.packet_type = Header::EMPTY_PACKET;
        header.packet_index = 0;
        return sizeof(Header);
    }
}

void RC_Protocol::process_rx_data(util::comms::RC_Phy::RX_Data const& data)
{
    QASSERT(data.payload.size() >= sizeof(Header));
    Header const& header = *reinterpret_cast<Header const*>(data.payload.data());

    m_received_packet_index = header.packet_index;


    //confirm and advance the packet queue
    {
        std::lock_guard<std::mutex> lg(m_tx_packet_queue_mutex);
        if (!m_tx_packet_queue.empty())
        {
            TX_Packet& packet = m_tx_packet_queue.front();
            size_t size = std::min(packet.payload.size() - packet.offset, get_mtu());
            QASSERT(size > 0);

            if (header.last_received_packet_index == m_crt_sent_packet_index)
            {
                m_crt_sent_packet_index++;
                if (m_crt_sent_packet_index > Header::MAX_PACKET_INDEX)
                {
                    m_crt_sent_packet_index = 1;
                }

                packet.offset += size;
                if (packet.offset >= packet.payload.size())
                {
#ifdef LOG
                    QLOGI("Confirmed packet size {}KB. {} packets pending", packet.payload.size(), m_tx_packet_queue.size());
#endif
                    m_tx_packet_queue.pop_front();
                }
                else
                {
#ifdef LOG
                    QLOGI("Confirmed fragment {} - {} out of {}KB", packet.offset - size, packet.offset, packet.payload.size());
#endif
                }
            }
        }
    }

    {
        std::lock_guard<std::mutex> lg(m_incoming_packet_mutex);

        size_t offset = m_incoming_packet.payload.size();
        size_t size = data.payload.size() - sizeof(Header);
        if (size > 0)
        {
            m_incoming_packet.payload.resize(offset + size);
            memcpy(m_incoming_packet.payload.data() + offset, data.payload.data() + sizeof(Header), size);
        }

        if (header.packet_type == Header::EMPTY_PACKET) //just a ping
        {
            m_incoming_packet.payload.clear();
            m_incoming_packet.packet_type = static_cast<uint8_t>(-1);
        }
        else if (header.packet_type < Header::EMPTY_PACKET) //starting a new packet stream?
        {
            m_incoming_packet.packet_type = header.packet_type;

            if (header.last_packet == 1)
            {
                m_incoming_packet.rx_dBm = data.rx_dBm;
                m_incoming_packet.tx_dBm = data.tx_dBm;
                m_incoming_packet.rx_timepoint = data.rx_timepoint;
                if (m_incoming_packet.packet_type != static_cast<uint8_t>(-1))
                {
                    m_rx_callback(m_incoming_packet);
                }
                else
                {
                    QLOGW("Incomplete packet received!");
                }
                m_incoming_packet.payload.clear();
                m_incoming_packet.packet_type = static_cast<uint8_t>(-1);
            }
        }
    }
}

void RC_Protocol::process()
{

}

}
}
