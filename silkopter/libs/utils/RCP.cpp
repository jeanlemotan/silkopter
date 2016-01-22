#include "RCP.h"
#include "utils/Timed_Scope.h"
#include "lz4/lz4.h"

namespace util
{

auto RCP::acquire_tx_datagram(size_t data_size) -> RCP::TX::Datagram_ptr
{
    return acquire_tx_datagram(data_size, data_size);
}

auto RCP::acquire_tx_datagram(size_t zero_size, size_t data_size) -> RCP::TX::Datagram_ptr
{
    auto datagram = m_tx.datagram_pool.acquire();

    QASSERT(zero_size <= data_size);
    datagram->data.resize(data_size);

    std::fill(datagram->data.begin(), datagram->data.begin() + zero_size, 0);

    datagram->sent_tp = q::Clock::time_point(q::Clock::duration{0});
    datagram->added_tp = q::Clock::time_point(q::Clock::duration{0});
    datagram->sent_count = 0;

    return datagram;
}

auto RCP::acquire_rx_datagram(size_t data_size) -> RCP::RX::Datagram_ptr
{
    return acquire_rx_datagram(data_size, data_size);
}
auto RCP::acquire_rx_datagram(size_t zero_size, size_t data_size) -> RCP::RX::Datagram_ptr
{
    auto datagram = m_rx.datagram_pool.acquire();
    QASSERT(zero_size <= data_size);
    datagram->data.resize(data_size);
    std::fill(datagram->data.begin(), datagram->data.begin() + zero_size, 0);
    return datagram;
}
auto RCP::RX::acquire_packet() -> RCP::RX::Packet_ptr
{
    auto packet = packet_pool.acquire();
    packet->received_fragment_count = 0;
    packet->added_tp = q::Clock::now();
    return packet;
}

template<class H> auto RCP::get_header(uint8_t const* data) -> H const&
{
    QASSERT(data);
    auto const& h = *reinterpret_cast<H const*>(data);
    return h;
}
template<class H> auto RCP::get_header(uint8_t* data) -> H&
{
    QASSERT(data);
    auto& h = *reinterpret_cast<H*>(data);
    return h;
}

auto RCP::get_header_size(uint8_t const* data_ptr, size_t data_size) -> size_t
{
    if (data_size < sizeof(Header))
    {
        return 0;
    }
    auto const& header = get_header<Header>(data_ptr);
    switch (header.type)
    {
    case Type::TYPE_PACKET:
    {
        auto const& pheader = get_header<Packet_Header>(data_ptr);
        return (pheader.fragment_idx == 0) ? sizeof(Packet_Main_Header) : sizeof(Packet_Header);
    }
    case Type::TYPE_CONFIRMATIONS: return sizeof(Confirmations_Header);
    case Type::TYPE_CONNECT_REQ: return sizeof(Connect_Req_Header);
    case Type::TYPE_CONNECT_RES: return sizeof(Connect_Res_Header);
    }
    return 0;
}

auto RCP::compute_crc(uint8_t const* data, size_t size) -> uint32_t
{
    auto crc = q::util::compute_murmur_hash32(data, size, 0);
    return crc;
}

//to avoid popping front in vectors
//Note - I use a vector instead of a deque because the deque is too slow at iterating.
//The erase can be implemented optimally for vectors if order need not be preserved.
template<class T> void RCP::erase_unordered(std::vector<T>& c, typename std::vector<T>::iterator it)
{
    QASSERT(!c.empty());
    if (it + 1 == c.end())
    {
        c.pop_back();
    }
    else
    {
        std::swap(*it, c.back());
        c.pop_back();
    }
}

//////////////////////////////////////////////////////////////////////////////

RCP::RCP(RCP_Socket& socket)
    : m_socket(socket)
{
//        auto hsz =sizeof(Header);
//        hsz =sizeof(Packet_Main_Header);
//        hsz =sizeof(Packet_Header);

//        hsz =sizeof(Packets_Confirmed_Header);

    m_mtu = m_socket.get_mtu();
    m_tx.send_buffer_header_size = m_socket.prepare_buffer(m_tx.send_buffer);

    m_socket.receive_callback = std::bind(&RCP::handle_receive, this, std::placeholders::_1, std::placeholders::_2);
    m_socket.send_callback = std::bind(&RCP::handle_send, this, std::placeholders::_1);

    m_rx.packet_pool.release = [](RX::Packet& p)
    {
        p.fragments.clear();
    };

//        m_rx.temp_buffer.resize(100 * 1024);

    std::fill(m_rx.last_packet_ids.begin(), m_rx.last_packet_ids.end(), 0);
    std::fill(m_last_id.begin(), m_last_id.end(), 0);

    m_global_receive_params.max_receive_time = std::chrono::seconds(5);

    m_init_tp = q::Clock::now();

    for (auto& ch: m_tx.channel_data)
    {
        ch.lz4_state.resize(LZ4_sizeofState());
    }
}

void RCP::set_send_params(uint8_t channel_idx, Send_Params const& params)
{
    if (channel_idx >= MAX_CHANNELS)
    {
        QASSERT(0);
        return;
    }
    m_send_params[channel_idx] = params;
}
auto RCP::get_send_params(uint8_t channel_idx) const -> Send_Params const&
{
    if (channel_idx >= MAX_CHANNELS)
    {
        QASSERT(0);
        return m_send_params[0];
    }
    return m_send_params[channel_idx];
}

void RCP::set_receive_params(uint8_t channel_idx, Receive_Params const& params)
{
    if (channel_idx >= MAX_CHANNELS)
    {
        QASSERT(0);
        return;
    }
    m_receive_params[channel_idx] = params;
}
void RCP::reconnect()
{
    std::lock_guard<std::mutex> lg(m_connection.mutex);
    disconnect();
    m_connection.last_sent_tp = q::Clock::now() - RECONNECT_BEACON_TIMEOUT;
}
auto RCP::is_connected() const -> bool
{
    bool is_connected = false;
    {
        std::lock_guard<std::mutex> lg(m_connection.mutex);
        is_connected = m_connection.is_connected;
    }
    return is_connected;
}
auto RCP::send(uint8_t channel_idx, uint8_t const* data, size_t size) -> bool
{
    auto const& params = m_send_params[channel_idx];
    return send(channel_idx, params, data, size);
}
auto RCP::try_sending(uint8_t channel_idx, uint8_t const* data, size_t size) -> bool
{
    auto const& params = m_send_params[channel_idx];
    return try_sending(channel_idx, params, data, size);
}

auto RCP::send(uint8_t channel_idx, Send_Params const& params, uint8_t const* data, size_t size) -> bool
{
    QLOG_TOPIC("RCP::send");
    if (!data || size == 0 || channel_idx >= MAX_CHANNELS)
    {
        QLOGE("Invalid channel ({}) or data ({}, {} bytes)", channel_idx, data, size);
        return false;
    }
    if (!m_connection.is_connected)
    {
        QLOGE("Not connected.");
        return false;
    }

    auto& channel_data = m_tx.channel_data[channel_idx];
    std::lock_guard<std::mutex> lg(channel_data.send_mutex);

    return _send_locked(channel_idx, params, data, size);
}
auto RCP::try_sending(uint8_t channel_idx, Send_Params const& params, uint8_t const* data, size_t size) -> bool
{
    QLOG_TOPIC("RCP::try_sending");
    if (!data || size == 0 || channel_idx >= MAX_CHANNELS)
    {
        QLOGE("Invalid channel ({}) or data ({}, {} bytes)", channel_idx, data, size);
        return false;
    }
    if (!m_connection.is_connected)
    {
        QLOGE("Not connected.");
        return false;
    }

    auto& channel_data = m_tx.channel_data[channel_idx];
    if (channel_data.send_mutex.try_lock())
    {
        bool res = _send_locked(channel_idx, params, data, size);
        channel_data.send_mutex.unlock();
        return res;
    }
    QLOGW("Cannot send packet because still sending previous one");
    return false;
}

auto RCP::_send_locked(uint8_t channel_idx, Send_Params const& params, uint8_t const* data, size_t size) -> bool
{
    //The send mutex should be locked here!!!
    if (size >= (1 << 24))
    {
        QLOGE("Packet too big: {}.", size);
        return false;
    }

    auto& channel_data = m_tx.channel_data[channel_idx];

    bool is_compressed = params.is_compressed;
    size_t uncompressed_size = size;
    if (is_compressed)
    {
        //auto start = q::Clock::now();

        auto comp_size = LZ4_compressBound(static_cast<int>(size));
        channel_data.compression_buffer.resize(comp_size);
        int ret = LZ4_compress_fast_extState(channel_data.lz4_state.data(),
                                             reinterpret_cast<const char*>(data),
                                             reinterpret_cast<char*>(channel_data.compression_buffer.data()),
                                             static_cast<int>(size),
                                             comp_size,
                                             5);
        if (ret > 0 && static_cast<size_t>(ret) < uncompressed_size)
        {
//            QLOGI("Compressed {}B -> {}B. {}% : {}", static_cast<int>(uncompressed_size), ret, ret * 100.f / uncompressed_size, q::Clock::now() - start);
            channel_data.compression_buffer.resize(ret);
            data = channel_data.compression_buffer.data();
            size = ret;
        }
        else
        {
            if (ret <= 0)
            {
                QLOGW("Cannot compress data: {}. Sending uncompressed", ret);
            }
            is_compressed = false;
        }
    }

    size_t max_fragment_size = m_mtu - sizeof(Packet_Main_Header);
    size_t left = size;
    size_t fragment_count = ((size - 1) / max_fragment_size) + 1;
    if (fragment_count > MAX_FRAGMENTS)
    {
        auto max = MAX_FRAGMENTS;
        QLOGE("Too many fragments: {} / {}.", fragment_count, max);
        return false;
    }

    auto now = q::Clock::now();

    auto id = ++m_last_id[channel_idx];

    {
        //cancel all previous packets if needed
        if (params.cancel_previous_data)
        {
            auto& queue = m_tx.packet_queue;
            std::lock_guard<std::mutex> lg(m_tx.packet_queue_mutex);

            for (auto it = queue.begin(); it != queue.end();)
            {
                auto& datagram = *it;
                auto const& hdr = get_header<Packet_Header>(datagram->data.data());
                if (hdr.channel_idx == channel_idx && hdr.id < id)
                {
                    it = queue.erase(it);
                    continue;
                }
                ++it;
            }
        }

        //QLOGI("crt +{}", fragment_count);

        channel_data.fragments_to_insert.clear();
        channel_data.fragments_to_insert.reserve(fragment_count);

        //add the new fragments
        for (size_t i = 0; i < fragment_count; i++)
        {
            QASSERT(left > 0);
            auto fragment_size = math::min(max_fragment_size, left);

            auto header_size = (i == 0) ? sizeof(Packet_Main_Header) : sizeof(Packet_Header);
            auto fragment = acquire_tx_datagram(header_size, header_size + fragment_size);

            fragment->params = params;

            auto& header = get_header<Packet_Header>(fragment->data.data());
            header.id = id;
            header.channel_idx = channel_idx;
            header.flag_needs_confirmation = params.is_reliable || params.unreliable_retransmit_count > 0;
            header.flag_is_compressed = is_compressed;
            header.type = TYPE_PACKET;
            header.fragment_idx = i;

            if (i == 0)
            {
                auto& f_header = get_header<Packet_Main_Header>(fragment->data.data());
                f_header.packet_size = uncompressed_size;
                f_header.fragment_count = fragment_count;
            }
            std::copy(data, data + fragment_size, fragment->data.data() + header_size);

            data += fragment_size;
            left -= fragment_size;

            fragment->added_tp = now;
            prepare_to_send_datagram(*fragment);

            channel_data.fragments_to_insert.push_back(fragment);
        }
//        for (auto const& d: queue)
//        {
//            QLOGI("{}, id: {}, f: {}, sc: {}", &d - &queue.front(), int(get_header<Packet_Header>(d->data).id), int(get_header<Packet_Header>(d->data).fragment_idx), d->sent_count);
//        }

        if (!channel_data.fragments_to_insert.empty())
        {
            std::lock_guard<std::mutex> lg(m_tx.packet_queue_mutex);

            auto& queue = m_tx.packet_queue;
            queue.insert(std::upper_bound(queue.begin(), queue.end(), channel_data.fragments_to_insert[0], tx_packet_datagram_predicate),
                    channel_data.fragments_to_insert.begin(),
                    channel_data.fragments_to_insert.end());
        }
        channel_data.fragments_to_insert.clear();

    }

    send_datagram();

    return true;
}

inline auto RCP::tx_packet_datagram_predicate(TX::Datagram_ptr const& AA, TX::Datagram_ptr const& BB) -> bool
{
    int priority_AA = AA->params.importance;
    int priority_BB = BB->params.importance;
    int sent_count_AA = AA->sent_count;
    int sent_count_BB = BB->sent_count;
    uint32_t id_AA = get_header<Packet_Header>(AA->data.data()).id;
    uint32_t id_BB = get_header<Packet_Header>(BB->data.data()).id;

    return priority_AA > priority_BB ||
            (priority_AA == priority_BB && sent_count_AA < sent_count_BB) ||
            (priority_AA == priority_BB && sent_count_AA == sent_count_BB && id_AA < id_BB);
}

bool RCP::receive(uint8_t channel_idx, std::vector<uint8_t>& data)
{
    QLOG_TOPIC("RCP::receive");

    data.clear();
    if (channel_idx >= MAX_CHANNELS)
    {
        QLOGE("Invalid channel {}", channel_idx);
        return false;
    }
    if (!m_connection.is_connected)
    {
        return false;
    }

    auto& queue = m_rx.packet_queues[channel_idx];
    auto& packets = queue.packets;
    std::lock_guard<std::mutex> lg(queue.mutex);

    auto const& params = m_receive_params[channel_idx];


    auto& last_packet_id = m_rx.last_packet_ids[channel_idx];

    while (true)
    {
        if (packets.empty())
        {
            break;
        }
        RX::Packet_ptr const& packet = packets.front().second;
        QASSERT(packet);

        auto next_expected_id = last_packet_id + 1;

        auto id = packet->any_header.id;
        QASSERT(id > 0);

        //is it from the past?
        if (id <= last_packet_id)
        {
            QLOGW("Ignoring past packet {}", id);
            m_global_stats.rx_zombie_packets++;
            if (packet->any_header.flag_needs_confirmation)
            {
                add_packet_confirmation(channel_idx, id);
            }
            packets.pop_front();
            m_global_stats.rx_dropped_packets++;
            continue;
        }

        auto now = q::Clock::now();
        auto max_receive_time = params.max_receive_time.count() > 0 ? params.max_receive_time : m_global_receive_params.max_receive_time;
        bool is_late = (max_receive_time.count() > 0 && now - packet->added_tp >= max_receive_time);

        //the next packet in sequence is missing - wait for it some more or cancel
        if (id > next_expected_id)
        {
            if (!is_late)
            {
                //wait some more
                break;
            }

            //waited enough, cancel the pending packet
            QLOGW("Canceling ghost packet {}", next_expected_id);
            add_packet_confirmation(channel_idx, next_expected_id);
            last_packet_id = next_expected_id;
            m_global_stats.rx_dropped_packets++;
            continue;
        }

        //no header yet or not all packages received?
        if (!packet->fragments[0] || packet->received_fragment_count < packet->main_header.fragment_count)
        {
            if (!is_late)
            {
                break;
            }

            //QLOGW("Canceling late packet {}. {} / {}", id, packet->received_fragment_count, packet->fragments[0] ? packet->main_header.fragment_count : 0);
            add_packet_confirmation(channel_idx, id);
            packets.pop_front();
            last_packet_id = id;
            m_global_stats.rx_dropped_packets++;

//                QLOGW("Still waiting for packet {}: {}/{} received", id, packet->received_fragment_count, static_cast<size_t>(packet->main_header.fragment_count));
            continue;
        }
        QASSERT(packet->fragments[0]);

        //QLOGI("Received packet {}", id);

        last_packet_id = id;
        m_global_stats.rx_packets++;

        //copy the data
        auto const& main_header = packet->main_header;
        m_rx.compression_buffer.clear();
        m_rx.compression_buffer.resize(main_header.packet_size);
        size_t offset = 0;
        size_t fragment_idx = 0;
        for (auto it = packet->fragments.begin(); it != packet->fragments.end(); ++it, fragment_idx++)
        {
            auto const& fragment = it->second;
            QASSERT(it->first == fragment_idx);
            auto header_size = (fragment_idx == 0 ? sizeof(Packet_Main_Header) : sizeof(Packet_Header));
            QASSERT(fragment->data.size() > header_size);
            std::copy(fragment->data.begin() + header_size, fragment->data.end(), m_rx.compression_buffer.begin() + offset);
            offset += fragment->data.size() - header_size;
        }
        packet->fragments.clear();

        if (main_header.flag_is_compressed)
        {
            data.resize(main_header.packet_size);
            int ret = LZ4_decompress_fast(reinterpret_cast<const char*>(m_rx.compression_buffer.data()),
                                          reinterpret_cast<char*>(data.data()),
                                          static_cast<int>(main_header.packet_size));
            if (ret < 0)
            {
                QLOGW("Decompression error: {}", ret);
                data.clear();
            }
            //return true;//ret == Z_OK;
        }
        else
        {
            QASSERT(m_rx.compression_buffer.size() == main_header.packet_size);
            std::swap(data, m_rx.compression_buffer);
        }

        packets.pop_front();

        break;
    }

    return !data.empty();
}

void RCP::process_connection()
{
    if (m_connection.is_connected)
    {
        return;
    }

    std::lock_guard<std::mutex> lg(m_connection.mutex);

    auto now = q::Clock::now();
    if (now - m_connection.last_sent_tp < RECONNECT_BEACON_TIMEOUT)
    {
        return;
    }
    m_connection.last_sent_tp = now;

    QLOGI("Sending reconnect request");
    send_packet_connect_req();
}

void RCP::process()
{
    {
        static q::Clock::time_point tp = q::Clock::now();
        if (q::Clock::now() - tp > std::chrono::seconds(1))
        {
            tp = q::Clock::now();
            std::lock_guard<std::mutex> lg(m_tx.packet_queue_mutex);
            QLOGI("{}: txd {} / tdf {} / rx {} / rxf {} / cf {}", m_tx.packet_queue.size(), m_global_stats.tx_datagrams, m_global_stats.tx_fragments, m_global_stats.rx_datagrams, m_global_stats.rx_fragments, m_global_stats.tx_confirmed_fragments);
        }
    }

    if (!m_connection.is_connected)
    {
        process_connection();
    }
    else
    {
        send_pending_confirmations();
    }

    send_datagram();
}

void RCP::prepare_to_send_datagram(TX::Datagram& datagram)
{
    auto& header = get_header<Header>(datagram.data.data());
    header.size = datagram.data.size();
    header.crc = 0;
    auto crc = compute_crc(datagram.data.data(), datagram.data.size());
    header.crc = crc;

    if (datagram.added_tp.time_since_epoch().count() == 0)
    {
        datagram.added_tp = q::Clock::now();
    }

//            if (header.crc == 0)
//            {
//                for (auto x: datagram->data)
//                {
//                    QLOGI("{}", x);
//                }
//                QLOGI("crc: {}/{}", crc, static_cast<int>(header.crc));
//            }

//            QASSERT_MSG(header.crc != 0, "{}", crc);
}

void RCP::add_and_send_datagram(TX::Send_Queue& queue, std::mutex& mutex, TX::Datagram_ptr const& datagram)
{
    if (!datagram)
    {
        QASSERT(0);
        return;
    }

    prepare_to_send_datagram(*datagram);

    //add to the queue
    {
        std::lock_guard<std::mutex> lg(mutex);
        queue.push_back(datagram);
    }
    send_datagram();
}

void RCP::update_stats(Stats& stats, TX::Datagram const& datagram)
{
    auto const& header = get_header<Header>(datagram.data.data());
    stats.tx_datagrams++;
    stats.tx_bytes += datagram.data.size();
    if (header.type == TYPE_PACKET)
    {
        auto const& pheader = get_header<Packet_Header>(datagram.data.data());
        if (pheader.fragment_idx == 0)
        {
            stats.tx_packets++;
        }
        stats.tx_fragments++;
    }
}

auto RCP::add_datagram_to_send_buffer(TX::Datagram_ptr const& datagram) -> bool
{
    QASSERT(datagram);
    size_t max_size = m_mtu + m_tx.send_buffer_header_size;
    size_t tx_send_buffer_size = m_tx.send_buffer.size();
    if (datagram->data.size() + tx_send_buffer_size <= max_size)
    {
        update_stats(m_global_stats, *datagram);

//        QLOGI("Sending after {}", q::Clock::now() - datagram->added_tp);

        m_tx.send_buffer.resize(tx_send_buffer_size + datagram->data.size());
        auto dst_it = m_tx.send_buffer.begin() + tx_send_buffer_size;

        std::copy(datagram->data.begin(), datagram->data.end(), dst_it);
        return true;
    }
    return false;
}

auto RCP::compute_next_transit_datagram() -> bool
{
    size_t merged = 0;
    m_tx.send_buffer.resize(m_tx.send_buffer_header_size);

    {
        std::lock_guard<std::mutex> lg(m_tx.internal_queues.mutex);
        if (m_tx.internal_queues.connection_res && add_datagram_to_send_buffer(m_tx.internal_queues.connection_res))
        {
            m_tx.internal_queues.connection_res.reset();
            merged++;
//                QLOGI("Sending connection res datagram {}", static_cast<int>(get_header<Con_Header>(datagram->data).seq));
        }
        else if (m_tx.internal_queues.connection_req && add_datagram_to_send_buffer(m_tx.internal_queues.connection_req))
        {
            m_tx.internal_queues.connection_req.reset();
            merged++;
//                QLOGI("Sending connection_req datagram {}", static_cast<int>(get_header<connection_req_Header>(datagram->data).seq));
        }

        //next the confirmations
        {
            auto& queue = m_tx.internal_queues.confirmations;
            while (!queue.empty() && add_datagram_to_send_buffer(queue.front()))
            {
//                    QLOGI("Sending confirmation for {} datagrams", static_cast<int>(get_header<Packets_Confirmed_Header>(datagram->data).count));
                merged++;
                queue.pop_front();
            }
        }
    }

    //next normal packets
    {
        std::lock_guard<std::mutex> lg(m_tx.packet_queue_mutex);

        m_tx.reinsert_queue.clear();

        auto now = q::Clock::now();

        //now go through the queue and see what we can pack together and send
        auto& queue = m_tx.packet_queue;
        for (auto it = queue.begin(); it != queue.end();)
        {
            auto& datagram = *it;
            if (datagram->params.cancel_after.count() > 0 && now - datagram->added_tp >= datagram->params.cancel_after)
            {
                it = queue.erase(it);
                continue;
            }

            if (now - datagram->sent_tp >= MIN_RESEND_DURATION && add_datagram_to_send_buffer(datagram))
            {
                merged++;
                datagram->sent_tp = now;
                datagram->sent_count++;

//                auto& header = get_header<Packet_Header>(datagram->data.data());
//                QLOGI("SENDING {}: pk {} fr {}, ch {}", queue.size(), int(header.id), header.fragment_idx, static_cast<int>(header.channel_idx));

                //do I have to insert it back?
                if (datagram->params.is_reliable == true || datagram->sent_count < datagram->params.unreliable_retransmit_count)
                {
                    m_tx.reinsert_queue.push_back(std::move(datagram));
                }

                it = queue.erase(it);
                continue;
            }

            ++it;
        }

        //now reinsert datagrams
        for (auto& datagram: m_tx.reinsert_queue)
        {
            queue.insert(std::upper_bound(queue.begin(), queue.end(), datagram, tx_packet_datagram_predicate), std::move(datagram));
        }
        m_tx.reinsert_queue.clear();
    }

//    if (merged > 1)
//    {
//        QLOGI("Merged: {}", merged);
//    }

    return m_tx.send_buffer.size() > m_tx.send_buffer_header_size;
}

void RCP::send_datagram()
{
    if (m_is_sending.exchange(true))
    {
        //was already sending, return
        //QLOGI("send blocked {}", q::Clock::now());
        return;
    }

    if (!compute_next_transit_datagram())
    {
        m_is_sending = false;
        return;
    }

    m_socket.async_send(m_tx.send_buffer.data(), m_tx.send_buffer.size());
}

void RCP::handle_send(RCP_Socket::Result)
{
    m_is_sending = false;

    send_datagram();
}

void RCP::handle_receive(uint8_t* data, size_t size)
{
    if (size > 0)
    {
        process_incoming_data(data, size);
    }
}

void RCP::add_fragment_confirmation(uint8_t channel_idx, uint32_t id, uint16_t fragment_idx)
{
    std::lock_guard<std::mutex> lg(m_tx.confirmations_mutex);

    TX::Confirmation conf;
    conf.channel_idx = channel_idx;
    conf.id = id;
    conf.fragment_idx = fragment_idx;
    m_tx.confirmations.push_back(conf);
}

void RCP::add_packet_confirmation(uint8_t channel_idx, uint32_t id)
{
    std::lock_guard<std::mutex> lg(m_tx.confirmations_mutex);

    TX::Confirmation conf;
    conf.channel_idx = channel_idx;
    conf.id = id;
    conf.fragment_idx = FRAGMENT_IDX_ALL;
    m_tx.confirmations.push_back(conf);
}

void RCP::send_pending_confirmations()
{
    {
        std::lock_guard<std::mutex> lg(m_tx.confirmations_mutex);

        //first throw away the ones we sent enough times
        while (!m_tx.confirmations.empty() && m_tx.confirmations.front().sent_count >= TX::MAX_CONFIRMATION_SEND_COUNT)
        {
            m_tx.confirmations.pop_front();
        }

        size_t header_size = sizeof(Confirmations_Header);

        size_t max_count = math::min<size_t>((m_mtu - header_size) / sizeof(Confirmations_Header::Data), 255u);

        //now take them and pack them in a datagram
        size_t pidx = 0;
        while (pidx < m_tx.confirmations.size())
        {
            size_t count = math::min(m_tx.confirmations.size() - pidx, max_count);

            TX::Datagram_ptr datagram = acquire_tx_datagram(header_size, header_size + count * sizeof(Confirmations_Header::Data)); //channel_idx:1 + id:3);

            auto& header = get_header<Confirmations_Header>(datagram->data.data());
            header.type = TYPE_CONFIRMATIONS;
            header.count = static_cast<uint8_t>(count);

            Confirmations_Header::Data* data_ptr = reinterpret_cast<Confirmations_Header::Data*>(datagram->data.data() + header_size);
            for (size_t i = 0; i < count; i++, pidx++, data_ptr++)
            {
                auto& res = m_tx.confirmations[pidx];
                res.sent_count++;
                data_ptr->all = 0;
                data_ptr->id = res.id;
                data_ptr->fragment_idx = res.fragment_idx;
                data_ptr->channel_idx = res.channel_idx;
            }

            prepare_to_send_datagram(*datagram);

            {
                std::lock_guard<std::mutex> ilg(m_tx.internal_queues.mutex);
                m_tx.internal_queues.confirmations.push_back(datagram);
            }
        }
    }

    send_datagram();
}

void RCP::send_packet_connect_req()
{
    {
        std::lock_guard<std::mutex> lg(m_tx.internal_queues.mutex);

        m_tx.internal_queues.connection_req = acquire_tx_datagram(sizeof(Connect_Req_Header));
        auto& header = get_header<Connect_Req_Header>(m_tx.internal_queues.connection_req->data.data());
        header.version = VERSION;
        header.type = TYPE_CONNECT_REQ;

        prepare_to_send_datagram(*m_tx.internal_queues.connection_req);
    }

    send_datagram();
}
void RCP::send_packet_connect_res(Connect_Res_Header::Response response)
{
    {
        std::lock_guard<std::mutex> lg(m_tx.internal_queues.mutex);

        m_tx.internal_queues.connection_res = acquire_tx_datagram(sizeof(Connect_Res_Header));
        auto& header = get_header<Connect_Res_Header>(m_tx.internal_queues.connection_res->data.data());
        header.version = VERSION;
        header.response = response;
        header.type = TYPE_CONNECT_RES;

        prepare_to_send_datagram(*m_tx.internal_queues.connection_res);
    }
    send_datagram();
}

void RCP::process_incoming_data(uint8_t* data_ptr, size_t data_size)
{
    QASSERT(data_ptr && data_size > 0);
    uint8_t* start_ptr = data_ptr;
    uint8_t const* end_ptr = start_ptr + data_size;
    QASSERT(start_ptr);
    while (start_ptr + sizeof(Header) <= end_ptr)
    {
        size_t available_size = size_t(end_ptr - start_ptr);
        size_t h_size = get_header_size(start_ptr, available_size);
        if (h_size == 0)
        {
            QLOGW("Unknown header.");
            return;
        }
        if (h_size > available_size)
        {
            QLOGW("Not enough data for header: {} / {}.", h_size, available_size);
            return;
        }

        auto& header = get_header<Header>(start_ptr);
        size_t size = header.size;
        if (size < h_size || size > available_size)
        {
            QLOGW("Wrong size in the header: {} out of max {} bytes.", size, end_ptr - start_ptr);
            return;
        }

        m_global_stats.rx_total_datagrams++;

        auto crc1 = header.crc;
        header.crc = 0;
        auto crc2 = compute_crc(start_ptr, size);
        if (crc1 != crc2)
        {
            m_global_stats.rx_corrupted_datagrams++;
            auto loss = m_global_stats.rx_corrupted_datagrams * 100 / m_global_stats.rx_total_datagrams;

            QLOGW("Crc is wrong. {} != {}. Packet loss: {.2}", crc1, crc2, loss);
        }
        else
        {
            m_global_stats.rx_datagrams++;

            if (!m_connection.is_connected)
            {
                switch (header.type)
                {
                case Type::TYPE_CONNECT_REQ: process_connect_req_data(start_ptr, size); break;
                case Type::TYPE_CONNECT_RES: process_connect_res_data(start_ptr, size); break;
                default: QLOGI("Disconnected: Ignoring datagram type {}", static_cast<int>(header.type)); break;
                }
            }
            else
            {
                switch (header.type)
                {
                case Type::TYPE_PACKET: process_packet_data(start_ptr, size); break;
                case Type::TYPE_CONFIRMATIONS: process_confirmations_data(start_ptr, size); break;
                case Type::TYPE_CONNECT_REQ: process_connect_req_data(start_ptr, size); break;
                case Type::TYPE_CONNECT_RES: QLOGW("Ignoring connection response while connected."); break;
                default: QASSERT(0); break;
                }
            }
        }

        start_ptr += size;
    }
}

auto RCP::rx_packet_id_predicate(RX::Packet_Queue::Item const& item1, uint32_t id) -> bool
{
    return item1.first < id;
}

void RCP::process_packet_data(uint8_t* data_ptr, size_t data_size)
{
    QASSERT(data_ptr && data_size > 0);
    auto const& header = get_header<Packet_Header>(data_ptr);

    auto channel_idx = header.channel_idx;
    auto fragment_idx = header.fragment_idx;
    auto id = header.id;

    auto& queue = m_rx.packet_queues[channel_idx];
    std::lock_guard<std::mutex> lg(queue.mutex);

    auto& packets = queue.packets;

    auto last_packet_id = m_rx.last_packet_ids[channel_idx];
    if (id <= last_packet_id)
    {
        m_global_stats.rx_zombie_packets++;
        if (header.flag_needs_confirmation)
        {
            add_packet_confirmation(channel_idx, id);
        }
        m_global_stats.rx_dropped_packets++;
    }
    else
    {
        RX::Packet_ptr packet;

        auto iter = std::lower_bound(packets.begin(), packets.end(), id, rx_packet_id_predicate);
        if (iter != packets.end() && iter->first == id)
        {
            packet = iter->second;
        }
        else
        {
            packet = m_rx.acquire_packet();
            packets.insert(iter, std::make_pair(id, packet));
        }

        if (packet->fragments[fragment_idx]) //we already have the fragment
        {
            m_global_stats.rx_duplicated_fragments++;
            //QLOGW("Duplicated fragment {} for packet {}.", fragment_idx, id);
        }
        else
        {
            m_global_stats.rx_fragments++;
            packet->received_fragment_count++;
            packet->any_header = header;
            if (fragment_idx == 0)
            {
                packet->main_header = get_header<Packet_Main_Header>(data_ptr);
            }
            auto datagram = acquire_rx_datagram(data_size);
            std::copy(data_ptr, data_ptr + data_size, datagram->data.begin());
            packet->fragments[fragment_idx] = std::move(datagram);
        }

        if (header.flag_needs_confirmation)
        {
            //if we received everything, tell the sender to stop sending this packet
            if (packet->fragments[0] && packet->received_fragment_count == packet->main_header.fragment_count)
            {
                add_packet_confirmation(channel_idx, id);
            }
            else
            {
                add_fragment_confirmation(channel_idx, id, fragment_idx);
            }
        }
    }

    //send_pending_confirmations();

//    if (channel_idx == 4)
//    {
//        QLOGI("Received fragment {} for packet {}: {}/{} received", fragment_idx, id, packet->received_fragment_count,
//              packet->fragments[0] ? static_cast<size_t>(packet->main_header.fragment_count) : 0u);
//    }
}
void RCP::process_confirmations_data(uint8_t* data_ptr, size_t data_size)
{
    QASSERT(data_ptr && data_size > 0);
    auto const& header = get_header<Confirmations_Header>(data_ptr);

    size_t count = header.count;
    QASSERT(count > 0);

    QASSERT(data_size == sizeof(Confirmations_Header) + sizeof(Confirmations_Header::Data)*count);

    Confirmations_Header::Data* conf = reinterpret_cast<Confirmations_Header::Data*>(data_ptr + sizeof(Confirmations_Header));
    Confirmations_Header::Data* conf_end = conf + count;

    //sort the confirmations ascending so we can search fast
    std::sort(conf, conf_end);//, [](Fragments_Res_Header::Data const& a, Fragments_Res_Header::Data const& b) { return a.all < b.all; });

    //confirm packet datagrams
    {
        std::lock_guard<std::mutex> lg(m_tx.packet_queue_mutex);

        auto& queue = m_tx.packet_queue;
        for (auto it = queue.begin(); it != queue.end();)
        {
            auto& datagram = *it;
            auto const& hdr = get_header<Packet_Header>(datagram->data.data());
            Confirmations_Header::Data key;

            key.all = 0;
            key.channel_idx = hdr.channel_idx;
            key.id = hdr.id;
            key.fragment_idx = hdr.fragment_idx;

            auto lb = std::lower_bound(conf, conf_end, key);
            if (lb != conf_end && *lb == key)
            {
//                if (hdr.channel_idx == 20)
//                {
//                    QLOGI("Confirming fragment {} for packet {}: {} / {}", hdr.fragment_idx, hdr.id, q::Clock::now() - datagram->added_tp, q::Clock::now() - datagram->sent_tp);
//                }
                it = queue.erase(it);
                m_global_stats.tx_confirmed_fragments++;
                continue;
            }

            key.fragment_idx = FRAGMENT_IDX_ALL;

            lb = std::lower_bound(conf, conf_end, key);
            //if (lb != conf_end && lb->channel_idx == key.channel_idx && lb->id == key.id)
            if (lb != conf_end && *lb == key)
            {
//                if (hdr.channel_idx == 20)
//                {
//                    QLOGI("Confirming packet {}: {} / {}", hdr.id, q::Clock::now() - datagram->added_tp, q::Clock::now() - datagram->sent_tp);
//                }
                it = queue.erase(it);
                m_global_stats.tx_confirmed_fragments++;
                continue;
            }

            ++it;
        }
    }
}

void RCP::process_connect_req_data(uint8_t* data_ptr, size_t data_size)
{
    QASSERT(data_ptr && data_size > 0);
    auto& header = get_header<Connect_Req_Header>(data_ptr);

    Connect_Res_Header::Response response;
    if (header.version != VERSION)
    {
        auto v = VERSION;
        QLOGW("Connection refused due to version mismatch: local version {} != remove version {}", v, header.version);
        response = Connect_Res_Header::Response::VERSION_MISMATCH;
    }
    else
    {
        //we received a connection request, so the other end already reset its connection. Time to reset the local one as well
        disconnect();
        connect();

        response = Connect_Res_Header::Response::OK;
    }

    send_packet_connect_res(response);
}
void RCP::process_connect_res_data(uint8_t* data_ptr, size_t data_size)
{
    if (m_connection.is_connected)
    {
        QLOGI("Ignoring connection response.");
        return;
    }

    QASSERT(data_ptr && data_size > 0);
    auto& header = get_header<Connect_Res_Header>(data_ptr);

    if (header.response == Connect_Res_Header::Response::OK)
    {
        //we received a connection response, so the other end already reset its connection. Time to reset the local one as well
        disconnect();
        connect();
    }
    else
    {
        QLOGW("Connection refused: remote version {}, response {}", header.version, header.response);
    }
}

void RCP::purge()
{
    //purge confirmations
    {
        std::lock_guard<std::mutex> lg(m_tx.confirmations_mutex);
        m_tx.confirmations.clear();
    }

    {
        std::lock_guard<std::mutex> lg(m_tx.internal_queues.mutex);
        m_tx.internal_queues.connection_req.reset();
        m_tx.internal_queues.connection_res.reset();
        m_tx.internal_queues.confirmations.clear();
    }

    {
        std::lock_guard<std::mutex> lg(m_tx.packet_queue_mutex);
        m_tx.packet_queue.clear();
//        m_tx.in_transit_datagram.reset();
    }

    for (size_t i = 0; i < MAX_CHANNELS; i++)
    {
        auto& queue = m_rx.packet_queues[i];

        std::lock_guard<std::mutex> lg(queue.mutex);
        queue.packets.clear();

        m_rx.last_packet_ids[i] = 0;
    };

    std::fill(m_last_id.begin(), m_last_id.end(), 0);
}

void RCP::disconnect()
{
    m_connection.is_connected = false;

    QLOGI("Disconncting.");
    purge();
}
void RCP::connect()
{
    if (m_connection.is_connected)
    {
        QLOGI("Already connected.");
        return;
    }

    QLOGI("Connected.");

    purge();
    m_connection.is_connected = true;

    //...
}


}
