#include "RC_Protocol.h"
#include "util/murmurhash.h"

namespace util
{
namespace comms
{

//#define LOG

RC_Protocol::RC_Protocol(RC_Phy& phy, RX_Callback rx_callback)
    : m_phy(phy)
    , m_rx_callback(rx_callback)
{
//    m_phy.set_callbacks(std::bind(&RC_Protocol::compute_tx_data, this, std::placeholders::_1, std::placeholders::_2),
//                        std::bind(&RC_Protocol::process_rx_data, this, std::placeholders::_1, std::placeholders::_2));

    //std::atomic_init(&m_crt_sent_packet_index, 0);
    //std::atomic_init(&m_received_packet_index, 0);

    m_phy.set_master_listen_rate(100);
}

RC_Protocol::~RC_Protocol()
{
}

bool RC_Protocol::init()
{
    reset_session();
    reset_session_data();

    return true;
}

void RC_Protocol::add_periodic_packet(Clock::duration period, TX_Callback tx_callback)
{
    Periodic_Packet pp;
    pp.period = period;
    pp.tx_callback = tx_callback;
    {
        std::lock_guard<std::mutex> lg(m_periodic_packets_mutex);
        m_periodic_packets.push_back(pp);
    }
}

void RC_Protocol::send_reliable_packet(uint8_t packet_type, void const* data, size_t size)
{
    Reliable_Packet packet;
    packet.packet_type = packet_type;
    packet.offset = 0;
    if (data != nullptr && size > 0)
    {
        packet.payload.resize(size);
        memcpy(packet.payload.data(), data, size);
    }

    std::lock_guard<std::mutex> lg (m_reliable_packet_queue_mutex);
    m_reliable_packet_queue.push_back(std::move(packet));
}

void RC_Protocol::reset_session()
{
    //A party will always reply to the reset signal with a signal of it's own.
    //The reset signal always contains as payload the session id

    //Case A: one party initiates the reset
    //1. One party calculates a new session id and starts spamming the reset signal
    //2. The other party receives the reset signal, copies the session id and replies with another reset signal
    //3. The initial party receives the others reset signal and checks if the ids are the same. If yes, it stops. Otherwise go to 2

    //Case B: both parties are signaling reset at the same time. They both send different ids
    //  When a party receives the reset signal, based on the session id it does the following:
    //1. If the received session id is the same, it stops signalling. The reset is done
    //2. If the received session id is smaller the it continues signalling its own id
    //3. If the received session id is bigger then it adopts it and stops spamming the reset signal.


    m_send_reset_request = true;
    m_session_id = rand();
}

void RC_Protocol::reset_session_data()
{
    m_received_reliable_packet_index = Reliable_Header::INVALID_PACKET_INDEX;

    {
        std::lock_guard<std::mutex> lg (m_reliable_packet_queue_mutex);
        m_reliable_packet_queue.clear();
    }
}

bool RC_Protocol::compute_tx_data()
{
    Clock::time_point now = Clock::now();

    size_t offset = buffer.size();

    bool packet_ready = false;

    //RESET -------------------------------------------------------
    if (m_send_reset_request || m_send_reset_reply)
    {
        reset_session_data();

        buffer.resize(sizeof(Reset_Header) + offset);
        Reset_Header& header = *reinterpret_cast<Reset_Header*>(buffer.data() + offset);
        header.header_type = Header::RESET;

        header.packet_type = m_send_reset_request == true ? Header::RESET_REQ_PACKET : Header::RESET_RES_PACKET;
        header.session_id = m_session_id;

        m_send_reset_reply = false;
#ifdef LOG
        QLOGI("TX reset packet, size {}", buffer.size());
#endif

        packet_ready = true;
    }

    //PERIODIC -------------------------------------------------------
    if (!packet_ready)
    {
        //compute the best packet to send (the one that has been waiting the longest)
        Periodic_Packet* best_packet = nullptr;
        Clock::duration best_duration = Clock::duration::zero();

        std::lock_guard<std::mutex> lg(m_periodic_packets_mutex);
        for (Periodic_Packet& p: m_periodic_packets)
        {
            Clock::duration duration = now - p.last_tp;
            if (duration >= p.period && duration > best_duration)
            {
                best_duration = duration;
                best_packet = &p;
            }
        }

        //found it, send it
        if (best_packet)
        {
            buffer.resize(sizeof(Periodic_Header) + offset);

            best_packet->last_tp = now;

            uint8_t packet_type = 0;
            if (best_packet->tx_callback(buffer, packet_type))
            {
                QASSERT(packet_type <= Header::MAX_PACKET_TYPE);
                Periodic_Header& header = *reinterpret_cast<Periodic_Header*>(buffer.data() + offset);
                header.header_type = Header::PERIODIC;
                header.packet_type = packet_type;

#ifdef LOG
                QLOGI("Send periodic packet type {}, size {}", packet_type, size);
                QLOGI("TX periodic packet type {}, size {}", packet_type, buffer.size());
#endif
                packet_ready = true;
            }
        }
        if (!packet_ready)
        {
            buffer.resize(sizeof(Header) + offset);
        }
    }

    //RELIABLE -------------------------------------------------------
    if (!packet_ready)
    {
        std::lock_guard<std::mutex> lg (m_reliable_packet_queue_mutex);
        if (!m_reliable_packet_queue.empty())
        {
            //to give the other end a chance to confirm the packet
            more_data = false;

            Reliable_Packet const& packet = m_reliable_packet_queue.front();
            size_t payload_size_left = packet.payload.size() - packet.offset;
            size_t size = std::min(payload_size_left, m_phy.get_mtu() - sizeof(Reliable_Header));

            buffer.resize(sizeof(Reliable_Header) + offset + size);
            Reliable_Header& header = *reinterpret_cast<Reliable_Header*>(buffer.data() + offset);
            header.header_type = Header::RELIABLE;
            if (size > 0)
            {
                memcpy(buffer.data() + offset + sizeof(Reliable_Header), packet.payload.data() + packet.offset, size);
            }

            header.packet_type = packet.packet_type;
            header.packet_index = m_crt_sent_reliable_packet_index;
            header.last_packet = (size == payload_size_left);

#ifdef LOG
            QLOGI("Sent fragment {} - {} out of {}KB", packet.offset, packet.offset + size, packet.payload.size());
            QLOGI("TX reliable packet type {}, index {}{}, size {}", packet.packet_type, m_crt_sent_reliable_packet_index, header.last_packet ? " last" : "", buffer.size());
#endif
            packet_ready = true;
        }
    }

    //EMPTY -------------------------------------------------------
    if (!packet_ready && m_empty_packet_needed)
    {
        m_empty_packet_needed = false;
        buffer.resize(sizeof(Periodic_Header) + offset);
        Periodic_Header& header = *reinterpret_cast<Periodic_Header*>(buffer.data() + offset);
        header.header_type = Header::PERIODIC;
        header.packet_type = Header::EMPTY_PACKET;

#ifdef LOG
        QLOGI("TX empty packet, size {}", buffer.size());
#endif

        packet_ready = true;
    }

    //Seal the packet -------------------------------------------------------
    if (packet_ready)
    {
        Header& header = *reinterpret_cast<Header*>(buffer.data() + offset);
        header.last_received_reliable_packet_index = m_received_reliable_packet_index;

        header.crc = 0;
        uint16_t computed_crc = q::util::compute_murmur_hash16(buffer.data() + offset, buffer.size() - offset);
        header.crc = computed_crc;
    }

    return packet_ready;
}

void RC_Protocol::process_rx_data(util::comms::RC_Phy::RX_Data const& rx_data, RC_Phy::Buffer& buffer)
{
    if (buffer.size() < sizeof(Header))
    {
        QLOGE("Invalid packet size (< header size)");
        return;
    }
    Header& header = *reinterpret_cast<Header*>(buffer.data());

    uint16_t crc = header.crc;
    header.crc = 0;
    uint16_t computed_crc = q::util::compute_murmur_hash16(buffer.data(), buffer.size());
    if (crc != computed_crc)
    {
#ifdef LOG
        QLOGW("Invalid crc. Expected {}, got {}", crc, computed_crc);
#endif
        return;
    }

    //handle the reset handshake!
    if (header.header_type == Header::RESET)
    {
#ifdef LOG
        QLOGI("RX reset packet, size {}", buffer.size());
#endif
        reset_session_data();

        Reset_Header const& header = *reinterpret_cast<Reset_Header const*>(buffer.data());
        uint32_t remote_session_id = header.session_id;
        if (header.packet_type == Header::RESET_REQ_PACKET)
        {
            //if I was resetting also, sync session ids
            if (m_send_reset_request)
            {
                //we have the same session? then the reset is complete
                if (remote_session_id >= m_session_id)
                {
                    m_send_reset_request = false; //stop spamming if the other party has the bigger id
                    m_session_id = remote_session_id;

                    QLOGI("RC Session synchronized, session id: {}", m_session_id);
                }
            }
            else
            {
                //if I was not resetting, take the others session id so they know we are in sync
                m_session_id = remote_session_id;
            }

            //respond to the request
            m_send_reset_reply = true;
        }
        else
        {
            //if I was resetting also, sync session ids
            if (m_send_reset_request)
            {
                //stop spamming only when the other party is replying correctly
                if (m_session_id == remote_session_id)
                {
                    m_send_reset_request = false;
                    QLOGI("RC Session synchronized, session id: {}", m_session_id);
                }
            }
        }

        return;
    }

    //ignore all packets until the reset is confirmed
    if (m_send_reset_request)
    {
        return;
    }

    //confirm and advance the packet queue
    {
        std::lock_guard<std::mutex> lg(m_reliable_packet_queue_mutex);
        if (!m_reliable_packet_queue.empty())
        {
            if (header.last_received_reliable_packet_index != Reliable_Header::INVALID_PACKET_INDEX &&
                header.last_received_reliable_packet_index == m_crt_sent_reliable_packet_index)
            {
                m_crt_sent_reliable_packet_index++;
                if (m_crt_sent_reliable_packet_index > Reliable_Header::MAX_PACKET_INDEX)
                {
                    m_crt_sent_reliable_packet_index = 0;
                }

                Reliable_Packet& packet = m_reliable_packet_queue.front();
                size_t size = std::min(packet.payload.size() - packet.offset, m_phy.get_mtu() - sizeof(Reliable_Header));
                packet.offset += size;
                if (packet.offset >= packet.payload.size())
                {
#ifdef LOG
                    QLOGI("Confirmed packet size {}KB. {} packets pending", packet.payload.size(), m_reliable_packet_queue.size());
#endif
                    m_reliable_packet_queue.pop_front();
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

    RX_Packet rx_packet;
    rx_packet.rx_dBm = rx_data.rx_dBm;
    rx_packet.tx_dBm = rx_data.tx_dBm;
    rx_packet.rx_timepoint = rx_data.rx_timepoint;


    if (header.header_type == Header::RELIABLE)
    {
        m_empty_packet_needed = true;

        Reliable_Header& header = *reinterpret_cast<Reliable_Header*>(buffer.data());
#ifdef LOG
        QLOGI("RX reliable packet type {}, index {}{}, size {}", (int)header.packet_type, (int)header.packet_index, header.last_packet ? " last" : "", buffer.size());
#endif
        if (header.packet_index != m_received_reliable_packet_index)
        {
            size_t offset = m_rx_reliable.payload.size();
            size_t size = buffer.size() - sizeof(Reliable_Header);
            if (size > 0)
            {
                m_rx_reliable.payload.resize(offset + size);
                memcpy(m_rx_reliable.payload.data() + offset, buffer.data() + sizeof(Reliable_Header), size);
            }

            rx_packet.packet_type = header.packet_type;
            if (header.last_packet == 1)
            {
                m_rx_callback(rx_packet, m_rx_reliable.payload.data(), m_rx_reliable.payload.size());
                m_rx_reliable.payload.clear();
            }

            m_received_reliable_packet_index = header.packet_index;
        }
    }
    else if (header.header_type == Header::PERIODIC)
    {
        Periodic_Header const& header = *reinterpret_cast<Periodic_Header const*>(buffer.data());
        if (header.packet_type != Header::EMPTY_PACKET)
        {
#ifdef LOG
            QLOGI("RX periodic packet type {}, size {}", (int)header.packet_type, buffer.size());
#endif
            size_t offset = m_rx_periodic.payload.size();
            size_t size = buffer.size() - sizeof(Periodic_Header);
            if (size > 0)
            {
                m_rx_periodic.payload.resize(offset + size);
                memcpy(m_rx_periodic.payload.data() + offset, buffer.data() + sizeof(Periodic_Header), size);
            }

            rx_packet.packet_type = header.packet_type;
            m_rx_callback(rx_packet, m_rx_periodic.payload.data(), m_rx_periodic.payload.size());
            m_rx_periodic.payload.clear();
        }
        else
        {
#ifdef LOG
            QLOGI("RX empty, size {}", (int)header.packet_type, buffer.size());
#endif
        }
    }
}

void RC_Protocol::process()
{

}

}
}
