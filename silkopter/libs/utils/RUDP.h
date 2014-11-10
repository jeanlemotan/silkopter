#pragma once

namespace util
{

    class RUDP : q::util::Noncopyable
    {
    public:
        typedef boost::asio::ip::udp::socket Socket_t;

        RUDP(Socket_t& socket);

        void set_mtu(size_t mtu);
        void set_destination(boost::asio::ip::udp::endpoint destination);

        void start();

        struct Send_Params
        {
            int8_t importance = 0; //Higher means higher priority. Can be negative
            bool is_reliable = true;
            q::Clock::duration bump_priority_after = q::Clock::duration{0}; //zero means never
            q::Clock::duration cancel_after = q::Clock::duration{0}; //zero means never
        };
        void set_send_params(uint8_t channel_idx, Send_Params const& params);

        struct Receive_Params
        {
            q::Clock::duration max_receive_time = q::Clock::duration{0}; //how long to wait for late packets. Zero meand the global defaults
        };
        void set_receive_params(uint8_t channel_idx, Receive_Params const& params);
        void set_global_receive_params(Receive_Params const& params);

        void send(uint8_t channel_idx, uint8_t const* data, size_t size);
        template<class T> bool receive(uint8_t channel_idx, T& data);

        void process();

    private:

        enum Type
        {
            TYPE_PACKET         =   0,
            TYPE_PACKET_REPLY   =   1,
            TYPE_PING           =   2,
        };

        static const size_t MAX_CHANNELS = 32;

#pragma pack(push, 1)
        struct Header
        {
            uint32_t id : 24;
            uint32_t channel_idx : 5;
            uint8_t flag_is_reliable : 1;
            uint8_t type : 2;
            uint32_t crc;
        };

        struct Packet_Header : Header
        {
            uint8_t fragment_idx;
        };

        struct Packet_Main_Header : Packet_Header
        {
            uint32_t packet_size : 24;
            uint32_t fragment_count : 8;
        };

        struct Packet_Reply_Header : Header
        {
            uint8_t fragment_idx;
            uint8_t flag_is_cancelled : 1;
        };

        struct Ping_Header : Header
        {
            uint32_t time_point; //ms
            uint8_t flag_is_pong : 1;
        };

#pragma pack(pop)

        Socket_t& m_socket;

        typedef std::vector<uint8_t> Buffer_t;

        template<class T>
        struct Pool
        {
            std::vector<std::shared_ptr<T>> pool;
            std::mutex mutex;

            std::shared_ptr<T> acquire()
            {
                std::lock_guard<std::mutex> lg(mutex);
                if (!pool.empty())
                {
                    auto item = pool.back();
                    pool.pop_back();
                    return item;
                }
                else
                {
                    return std::make_shared<T>();
                }
            }
            void release(std::shared_ptr<T> const& item)
            {
                std::lock_guard<std::mutex> lg(mutex);
                pool.push_back(item);
            }
        };

        static const int MAX_PRIORITY = 127;
        static const int MIN_PRIORITY = -127;
        static const int DENY_PRIORITY = -128;

        struct TX
        {
            struct Datagram
            {
                Send_Params params;
                int priority = 0;
                q::Clock::time_point added;
                q::Clock::time_point sent;
                Buffer_t data;
            };
            typedef std::shared_ptr<Datagram> Datagram_ptr;

            Pool<Datagram> datagram_pool;

            Datagram_ptr acquire_datagram(size_t data_size)
            {
                auto datagram = datagram_pool.acquire();
                datagram->added = q::Clock::time_point(q::Clock::duration{0});
                datagram->sent = q::Clock::time_point(q::Clock::duration{0});
                datagram->priority = 0;
                datagram->data.resize(0); //this will force zero-ing the data
                datagram->data.resize(data_size);
                return datagram;
            }
            void release_datagram(Datagram_ptr& datagram)
            {
                if (datagram)
                {
                    datagram_pool.release(datagram);
                }
                datagram.reset();
            }

            //waiting to be sent
            std::vector<Datagram_ptr> send_queue;
            std::mutex send_queue_mutex;
        } m_tx;

        struct RX
        {
            std::array<uint8_t, 32768> temp_buffer;

            struct Datagram
            {
                Buffer_t data;
            };
            typedef std::shared_ptr<Datagram> Datagram_ptr;
            Pool<Datagram> datagram_pool;

            Datagram_ptr acquire_datagram(size_t data_size)
            {
                auto datagram = datagram_pool.acquire();
                datagram->data.resize(0); //this will force zero-ing the data
                datagram->data.resize(data_size);
                return datagram;
            }
            void release_datagram(Datagram_ptr& datagram)
            {
                if (datagram)
                {
                    datagram_pool.release(datagram);
                }
                datagram.reset();
            }

            std::vector<Datagram_ptr> incoming_queue;
            std::mutex incoming_queue_mutex;

            struct Packet
            {
                size_t received_fragment_count = 0;
                q::Clock::time_point added;
                Packet_Main_Header main_header;
                Packet_Header any_header;
                std::array<Datagram_ptr, 256> fragments;
            };
            typedef std::shared_ptr<Packet> Packet_ptr;
            Pool<Packet> packet_pool;

            Packet_ptr acquire_packet()
            {
                auto packet = packet_pool.acquire();
                packet->received_fragment_count = 0;
                packet->added = q::Clock::now();
                return packet;
            }
            void release_packet(Packet_ptr& packet)
            {
                if (packet)
                {
                    for (auto& f: packet->fragments)
                    {
                        release_datagram(f);
                        f.reset();
                    }
                    packet_pool.release(packet);
                }
                packet.reset();
            }

            //waiting to be received
            std::array<std::map<uint32_t, Packet_ptr>, MAX_CHANNELS> incomplete_queues;
            std::array<std::deque<Packet_ptr>, MAX_CHANNELS> complete_queues;
            std::array<uint32_t, MAX_CHANNELS> last_packet_ids = {0};
        } m_rx;

        //sent pings
        std::deque<Ping_Header> m_pings;
        std::mutex m_pings_mutex;
        boost::circular_buffer<q::Clock::duration> m_rtts;
        q::Clock::duration m_rtt;


        struct Stats
        {
            size_t tx_datagrams = 0;
            size_t tx_resent_datagrams = 0;
            size_t tx_cancelled_datagrams = 0;
            size_t tx_confirmed_datagrams = 0;
            size_t tx_cancelled_packets = 0;
            size_t tx_packets = 0;
            size_t tx_bytes = 0;
            size_t tx_pings = 0;
            size_t tx_pongs = 0;

            size_t rx_datagrams = 0;
            size_t rx_corrupted_datagrams = 0;
            size_t rx_good_datagrams = 0;
            size_t rx_zombie_datagrams = 0;
            size_t rx_duplicated_datagrams = 0;
            size_t rx_packets = 0;
            size_t rx_dropped_packets = 0;
            size_t rx_bytes = 0;
            size_t rx_pings = 0;
            size_t rx_pongs = 0;
        };

        size_t m_pings_sent;
        size_t m_pongs_received;

        std::atomic<uint32_t> m_last_id = {0};
        Stats m_global_stats;

        std::atomic_bool m_is_sending = {false};
        const q::Clock::duration MIN_RESEND_DURATION = std::chrono::milliseconds(10);

        size_t m_mtu = 1024;
        boost::asio::ip::udp::endpoint m_destination;

        std::array<Send_Params, MAX_CHANNELS> m_send_params;
        std::array<Receive_Params, MAX_CHANNELS> m_receive_params;
        Receive_Params m_global_receive_params;


        template<class H> static H const& get_header(Buffer_t const& data)
        {
            QASSERT(data.size() >= sizeof(H));
            auto const& h = *reinterpret_cast<H const*>(data.data());
            return h;
        }
        template<class H> static H& get_header(Buffer_t& data)
        {
            QASSERT(data.size() >= sizeof(H));
            auto& h = *reinterpret_cast<H*>(data.data());
            return h;
        }

        static size_t get_header_size(Buffer_t& data)
        {
            if (data.size() < sizeof(Header))
            {
                QASSERT(0);
                return 0;
            }
            auto const& header = get_header<Header>(data);
            switch (header.type)
            {
            case Type::TYPE_PACKET:
            {
                auto const& pheader = get_header<Packet_Header>(data);
                return (pheader.fragment_idx == 0) ? sizeof(Packet_Main_Header) : sizeof(Packet_Header);
            }
            case Type::TYPE_PACKET_REPLY: return sizeof(Packet_Reply_Header);
            case Type::TYPE_PING: return sizeof(Ping_Header);
            }
            QASSERT(0);
            return 0;
        }

        std::vector<TX::Datagram_ptr>::iterator process_send_queue()
        {
//            std::lock_guard<std::mutex> lg(m_tx.to_send_mutex);

            if (m_tx.send_queue.empty())
            {
                return m_tx.send_queue.end();
            }

            auto now = q::Clock::now();

            auto min_resend_duration = std::max(MIN_RESEND_DURATION, m_rtt / 10);

            auto best_it = m_tx.send_queue.begin();

            //calculate priorities
            for (auto it = m_tx.send_queue.begin(); it != m_tx.send_queue.end();)
            {
                auto const& dg = *it;
                auto const& params = dg->params;

                //eliminate old datagrams
                if (params.cancel_after.count() > 0 && now - dg->added >= params.cancel_after)
                {
                    m_tx.send_queue.erase(it);
                    continue;
                }

                if (now - dg->sent < min_resend_duration)
                {
                    dg->priority = DENY_PRIORITY;
                }
                else
                {
                    int priority = params.importance;
                    if (params.bump_priority_after.count() > 0 && now - dg->added >= params.bump_priority_after)
                    {
                        priority += 63; //bump it by half
                    }

                    dg->priority = priority;
                }
                auto& header = get_header<Header>(dg->data);
                if (header.type == TYPE_PACKET_REPLY)
                {
                    dg->priority = MAX_PRIORITY;
                }

                {
                    auto const& best = *best_it;
                    auto age0 = now - dg->sent;
                    auto age1 = now - best->sent;
                    if ((dg->priority > best->priority) || (dg->priority == best->priority && age0 > age1))
                    {
                        best_it = it;
                    }
                }
                ++it;
            }
            return best_it;

//            std::nth_element(m_tx.send_queue.begin(), m_tx.send_queue.begin(), m_tx.send_queue.end(),
//                            [now](const TX::Datagram_ptr& d0, const TX::Datagram_ptr& d1)
//                            {
//                                auto age0 = now - d0->sent;
//                                auto age1 = now - d1->sent;
//                                return (d0->priority > d1->priority) ||
//                                       (d0->priority == d1->priority && age0 > age1);
//                            });
        }

        void add_and_send_datagram(TX::Datagram_ptr const& datagram)
        {
            if (!datagram)
            {
                QASSERT(0);
                return;
            }
            //add to the queue
            {
                std::lock_guard<std::mutex> lg(m_tx.send_queue_mutex);
                m_tx.send_queue.push_back(datagram);
            }
            send_datagram();
        }

        void update_stats(Stats& stats, TX::Datagram const& datagram)
        {
            auto const& header = get_header<Header>(datagram.data);
            stats.tx_datagrams++;
            stats.tx_resent_datagrams += datagram.sent.time_since_epoch().count() == 0 ? 0 : 1;
            stats.tx_bytes += datagram.data.size();
            if (header.type == TYPE_PACKET)
            {
                auto const& pheader = get_header<Packet_Header>(datagram.data);
                if (pheader.fragment_idx == 0)
                {
                    stats.tx_packets++;
                }
            }
        }

        //to avoid popping front in vectors
        //Note - I use a vector instead of a deque because the deque is too slow at iterating.
        //The pop_front can be implemented optimally for vectors if order need not be preserved.
        template<class T> static void pop_unordered(std::vector<T>& c, typename std::vector<T>::iterator it)
        {
            QASSERT(!c.empty());
            if (c.size() == 1)
            {
                QASSERT(it == c.begin());
                c.clear();
            }
            else
            {
                std::swap(*it, c.back());
                c.pop_back();
            }
        }

        void send_datagram()
        {
            if (m_is_sending.exchange(true))
            {
                //was already sending, return
                return;
            }

            TX::Datagram_ptr datagram;
            {
                std::lock_guard<std::mutex> lg(m_tx.send_queue_mutex);

                auto it = process_send_queue();
                if (it == m_tx.send_queue.end())
                {
                    return;
                }

                datagram = *it;
                if (datagram->priority < MIN_PRIORITY)
                {
                    return;
                }

                //if the datagram is not reliable remove it from the queue. Otherwise leave it there to be resent
                auto const& header = get_header<Header>(datagram->data);
                if (!header.flag_is_reliable)
                {
                    pop_unordered(m_tx.send_queue, it);
                }
            }
            QASSERT(datagram);

            auto const& header = get_header<Header>(datagram->data);
            update_stats(m_global_stats, *datagram);

            datagram->sent = q::Clock::now();

//            m_socket.async_send_to(boost::asio::buffer(datagram->data.data(), datagram->data.size()),
//                                   m_destination,
//                                   boost::bind(&RUDP::handle_send, this,
//                                   datagram,
//                                   boost::asio::placeholders::error));


            //---test
            auto rx_datagram = m_rx.acquire_datagram(datagram->data.size());
            rx_datagram->data = datagram->data;
            //process_incoming_datagram(rx_datagram);
            std::thread([this,rx_datagram]()
            {
                std::lock_guard<std::mutex> lg(m_rx.incoming_queue_mutex);
                m_rx.incoming_queue.push_back(rx_datagram);
                m_is_sending = false;
            }).detach();
        }

        void handle_send(std::shared_ptr<TX::Datagram> datagram,
                         const boost::system::error_code& /*error*/)
        {
            if (!datagram->params.is_reliable)
            {
                m_tx.release_datagram(datagram);
            }

            m_is_sending = false;

            send_datagram();
        }

        void handle_receive(const boost::system::error_code& error, std::size_t bytes_transferred)
        {
            if (error)
            {
                if (error != boost::asio::error::eof)
                {
                    SILK_ERR("Error on socket receive: {}", error.message());
                    m_socket.close();
                }
            }
            else
            {
                if (bytes_transferred > 0)
                {
                    auto datagram = m_rx.acquire_datagram(bytes_transferred);
                    std::copy(m_rx.temp_buffer.begin(), m_rx.temp_buffer.begin() + bytes_transferred, datagram->data.begin());

                    //add it to the queue. It will be processed laterz in the process function
                    {
                        std::lock_guard<std::mutex> lg(m_rx.incoming_queue_mutex);
                        m_rx.incoming_queue.push_back(datagram);
                    }
                }
                start();
            }
        }

        void send_packet_reply(Packet_Header const& header, bool is_cancelled)
        {
            auto datagram = m_tx.acquire_datagram(sizeof(Packet_Reply_Header));
            datagram->added = q::Clock::now();

            auto& cheader = get_header<Packet_Reply_Header>(datagram->data);
            cheader.id = header.id;
            cheader.channel_idx = header.channel_idx;
            cheader.flag_is_reliable = false;
            cheader.type = TYPE_PACKET_REPLY;
            cheader.fragment_idx = header.fragment_idx;
            cheader.flag_is_cancelled = is_cancelled;

            auto crc = q::util::compute_murmur_hash32(datagram->data.data(), datagram->data.size());
            cheader.crc = crc;

            add_and_send_datagram(datagram);
        }
        void send_packet_reply_cancel(Packet_Header const& header)
        {
            send_packet_reply(header, true);
        }
        void send_packet_reply_confirm(Packet_Header const& header)
        {
            send_packet_reply(header, false);
        }

        void process_incoming_datagram(RX::Datagram_ptr& datagram)
        {
            QASSERT(datagram);
            auto& header = get_header<Header>(datagram->data);
            size_t h_size = get_header_size(datagram->data);
            if (h_size == 0)
            {
                SILK_WARNING("Unknonw header.");
                m_rx.release_datagram(datagram);
                return;
            }

            m_global_stats.rx_datagrams++;

            auto crc1 = header.crc;
            header.crc = 0;
            auto crc2 = q::util::compute_murmur_hash32(datagram->data.data(), datagram->data.size());
            if (crc1 != crc2)
            {
                m_global_stats.rx_corrupted_datagrams++;
                auto loss = m_global_stats.rx_corrupted_datagrams * 100 / m_global_stats.rx_datagrams;

                SILK_WARNING("Crc is wrong. {} != {}. Packet loss: {.2}", crc1, crc2, loss);
                m_rx.release_datagram(datagram);
                return;
            }
            m_global_stats.rx_good_datagrams++;

            switch (header.type)
            {
            case Type::TYPE_PACKET: process_packet_datagram(datagram); break;
            case Type::TYPE_PACKET_REPLY: process_packet_reply_datagram(datagram); break;
            case Type::TYPE_PING: process_ping_datagram(datagram); break;
            }
        }

        void process_packet_datagram(RX::Datagram_ptr& datagram)
        {
            QASSERT(datagram);
            auto const& header = get_header<Packet_Header>(datagram->data);

            auto ch = header.channel_idx;
            auto fragment_idx = header.fragment_idx;
            auto id = header.id;
            if (id <= m_rx.last_packet_ids[ch])
            {
                m_global_stats.rx_zombie_datagrams++;

                SILK_WARNING("Blast from the past - datagram {} for packet id {}.", fragment_idx, id);

                if (header.flag_is_reliable)
                {
                    send_packet_reply_cancel(header);
                }

                m_rx.release_datagram(datagram);
                return;
            }

            if (header.flag_is_reliable)
            {
                send_packet_reply_confirm(header);
            }

            auto& queue = m_rx.incomplete_queues[ch];
            auto& packet = queue[id];
            if (!packet)
            {
                packet = m_rx.acquire_packet();
            }

            if (packet->fragments[fragment_idx]) //we already have the fragment
            {
                m_global_stats.rx_duplicated_datagrams++;

                SILK_WARNING("Duplicated fragment {} for packet {}.", fragment_idx, id);
                m_rx.release_datagram(datagram);
                return;
            }

            packet->received_fragment_count++;
            packet->any_header = header;
            if (fragment_idx == 0)
            {
                packet->main_header = get_header<Packet_Main_Header>(datagram->data);
            }
            packet->fragments[fragment_idx] = std::move(datagram);

            //HH SILK_INFO("Received fragment {} for packet {}: {}/{} received", fragment_idx, id, packet->received_fragment_count, static_cast<size_t>(packet->main_header.fragment_count));
        }
        void process_packet_reply_datagram(RX::Datagram_ptr& datagram)
        {
            QASSERT(datagram);
            auto const& header = get_header<Packet_Reply_Header>(datagram->data);

            auto id = header.id;
            auto channel_idx = header.channel_idx;
            if (header.flag_is_cancelled)
            {
                auto size_before = m_tx.send_queue.size();

                std::lock_guard<std::mutex> lg(m_tx.send_queue_mutex);
                m_tx.send_queue.erase(std::remove_if(m_tx.send_queue.begin(), m_tx.send_queue.end(),
                        [id, channel_idx](const TX::Datagram_ptr& d)
                        {
                            auto const& hdr = get_header<Header>(d->data);
                            return (hdr.type != TYPE_PACKET_REPLY && hdr.id == id && hdr.channel_idx == channel_idx);
                        }), m_tx.send_queue.end());

                auto size_after = m_tx.send_queue.size();
                QASSERT(size_before >= size_after);
                if (size_before > size_after)
                {
                    m_global_stats.tx_cancelled_datagrams += size_before - size_after;
                    m_global_stats.tx_cancelled_packets ++;
                    SILK_INFO("Cancelling packet {}: {} fragments removed", id, size_before - size_after);
                }
            }
            else //confirmation
            {
                auto fragment_idx = header.fragment_idx;
                auto size_before = m_tx.send_queue.size();

                std::lock_guard<std::mutex> lg(m_tx.send_queue_mutex);
                for (auto it = m_tx.send_queue.begin(); it != m_tx.send_queue.end(); ++it)
                {
                    auto const& d = (*it);
                    auto const& hdr = get_header<Header>(d->data);
                    if (hdr.type != TYPE_PACKET_REPLY && hdr.id == id && hdr.channel_idx == channel_idx)
                    {
                        QASSERT(hdr.type == TYPE_PACKET);
                        auto const& phdr = get_header<Packet_Header>(d->data);
                        if (phdr.fragment_idx == fragment_idx)
                        {
                            m_tx.send_queue.erase(it);
                            break;
                        }
                    }
                }

                auto size_after = m_tx.send_queue.size();
                QASSERT(size_before == size_after + 1 || size_before == size_after);
                if (size_before >= size_after)
                {
                    m_global_stats.tx_confirmed_datagrams++;
                    //HH SILK_INFO("Confirming fragment {} for packet {}", fragment_idx, id);
                }
            }

            m_rx.release_datagram(datagram);
        }
        void process_ping_datagram(RX::Datagram_ptr& datagram)
        {
            QASSERT(datagram);
            auto& header = get_header<Ping_Header>(datagram->data);

            if (header.flag_is_pong)
            {
                m_global_stats.tx_pongs++;
            }
            else
            {
                m_global_stats.tx_pings++;
            }

            m_rx.release_datagram(datagram);
        }

   };

    inline RUDP::RUDP(Socket_t& socket)
        : m_socket(socket)
    {
        auto hsz =sizeof(Header);
        hsz =sizeof(Packet_Main_Header);
        hsz =sizeof(Packet_Header);
        hsz =sizeof(Packet_Reply_Header);
        hsz =sizeof(Ping_Header);
        hsz == hsz;

        m_global_receive_params.max_receive_time = std::chrono::seconds(5);
    }


    inline void RUDP::start()
    {
        m_socket.async_receive(boost::asio::buffer(m_rx.temp_buffer.data(), m_rx.temp_buffer.size()),
                                boost::bind(&RUDP::handle_receive, this,
                                boost::asio::placeholders::error,
                                boost::asio::placeholders::bytes_transferred));
    }

    inline void RUDP::send(uint8_t channel_idx, uint8_t const* data, size_t size)
    {
        if (!data || size == 0 || channel_idx >= MAX_CHANNELS)
        {
            QASSERT(0);
            return;
        }

        if (size >= (1 << 24))
        {
            SILK_ERR("Packet too big: {}.", size);
            return;
        }

        size_t max_fragment_size = m_mtu;
        size_t left = size;
        size_t fragment_count = ((size - 1) / m_mtu) + 1;
        if (fragment_count > 255)
        {
            SILK_WARNING("Too many fragments: {}. Ignoring mtu.", fragment_count);
            fragment_count = 255;
            max_fragment_size = ((size - 1) / fragment_count) + 1;
        }

        auto now = q::Clock::now();

        auto id = ++m_last_id;
        for (size_t i = 0; i < fragment_count; i++)
        {
            QASSERT(left > 0);
            auto fragment_size = math::min(max_fragment_size, left);

            auto datagram_size = ((i == 0) ? sizeof(Packet_Main_Header) : sizeof(Packet_Header)) + fragment_size;
            auto fragment = m_tx.acquire_datagram(datagram_size);

            auto& params = m_send_params[channel_idx];
            fragment->params = params;
            fragment->added = now;


            size_t h_size = 0;
            if (i == 0)
            {
                h_size = sizeof(Packet_Main_Header);
                auto& f_header = get_header<Packet_Main_Header>(fragment->data);
                f_header.packet_size = size;
                f_header.fragment_count = fragment_count;
            }
            else
            {
                h_size = sizeof(Packet_Header);
                auto& f_header = get_header<Packet_Header>(fragment->data);
            }
            QASSERT(h_size > 0);

            fragment->data.resize(h_size + fragment_size);
            std::copy(data, data + fragment_size, fragment->data.begin() + h_size);

            auto& header = get_header<Packet_Header>(fragment->data);
            header.id = id;
            header.channel_idx = channel_idx;
            header.flag_is_reliable = params.is_reliable;
            header.type = TYPE_PACKET;
            header.fragment_idx = i;

            auto crc = q::util::compute_murmur_hash32(fragment->data.data(), fragment->data.size());
            header.crc = crc;
            //HH SILK_INFO("Sending fragment {} for packet {}", header.fragment_idx, id);

            data += fragment_size;
            left -= fragment_size;

            add_and_send_datagram(fragment);
        }
        send_datagram();
    }

    template<class T>
    inline bool RUDP::receive(uint8_t channel_idx, T& data)
    {
        if (channel_idx >= MAX_CHANNELS)
        {
            QASSERT(0);
            return false;
        }
        auto& q = m_rx.complete_queues[channel_idx];
        if (q.empty())
        {
            return false;
        }
        auto& packet = q.front();
        QASSERT(packet);

        auto const& main_header = packet->main_header;
        data.resize(main_header.packet_size);
        size_t off = 0;
        for (size_t i = 0; i < main_header.fragment_count; i++)
        {
            auto const& fragment = packet->fragments[i];
            auto header_size = ((i == 0) ? sizeof(Packet_Main_Header) : sizeof(Packet_Header));
            QASSERT(fragment->data.size() > header_size);
            auto payload_size = fragment->data.size() - header_size;
            QASSERT(off + payload_size <= main_header.packet_size);

            std::copy(fragment->data.begin() + header_size, fragment->data.end(), data.begin() + off);
            off += payload_size;
        }
        QASSERT(off == main_header.packet_size);

        m_rx.release_packet(packet);
        q.pop_front();
        return true;
    }

    inline void RUDP::process()
    {
        std::lock_guard<std::mutex> lg(m_rx.incoming_queue_mutex);
        for (auto& f: m_rx.incoming_queue)
        {
            process_incoming_datagram(f);
            QASSERT(f == nullptr);
        }
        m_rx.incoming_queue.clear();

        //process complete packages
        {
            auto now = q::Clock::now();

            for (size_t ch = 0; ch < m_rx.incomplete_queues.size(); ch++)
            {
                auto& q = m_rx.incomplete_queues[ch];
                if (q.empty())
                {
                    continue;
                }
                auto& packet = q.begin()->second;
                QASSERT(packet);

                auto id = packet->any_header.id;
                QASSERT(id > 0);

                //no header yet or not all packages received?
                if (!packet->fragments[0] ||
                        packet->received_fragment_count < packet->main_header.fragment_count)
                {
//                    auto const& params = m_receive_params[ch];
//                    auto max_receive_time = params.max_receive_time.count() > 0 ? params.max_receive_time : m_global_receive_params.max_receive_time;
//                    if (now - packet->added >= max_receive_time)
//                    {
//                        SILK_WARNING("Canceling packet {}", id);
//                        send_packet_reply_cancel(packet->any_header);
//                        m_rx.release_packet(packet);
//                        q.erase(q.begin());
//                        m_rx.last_packet_ids[ch] = id;
//                        m_global_stats.rx_dropped_packets++;
//                    }

                    //SILK_WARNING("Still waiting for packet {}: {}/{} received", id, packet->received_fragment_count, static_cast<size_t>(packet->main_header.fragment_count));
                    continue;
                }
                QASSERT(packet->fragments[0]);

                SILK_INFO("Received packet {}", id);

                m_rx.complete_queues[ch].push_back(std::move(packet));
                q.erase(q.begin());
                m_rx.last_packet_ids[ch] = id;
                m_global_stats.rx_packets++;
            }
        }

        send_datagram();
    }

}
