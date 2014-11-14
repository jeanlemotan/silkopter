#pragma once

#include <algorithm>

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
            TYPE_PACKET             =   0,
            TYPE_PACKETS_CONFIRMED  =   1,
            TYPE_PACKETS_CANCELLED  =   2,
            TYPE_PING               =   3,
            TYPE_PONG               =   4,
        };

        static const size_t MAX_CHANNELS = 32;

#pragma pack(push, 1)
        struct Header
        {
            uint8_t type : 4;
            uint32_t crc;
        };

        struct Packet_Header : Header
        {
            uint32_t id : 24;
            uint32_t channel_idx : 5;
            uint32_t flag_is_reliable : 1;
            uint8_t fragment_idx;
        };

        struct Packet_Main_Header : Packet_Header
        {
            uint32_t packet_size : 24;
            uint32_t fragment_count : 8;
        };

        struct Packets_Confirmed_Header : Header
        {
            static const size_t MAX_PACKED = 20;
            uint8_t count;
        };

        struct Packets_Cancelled_Header : Header
        {
            static const size_t MAX_PACKED = 20;
            uint8_t count;
        };

        struct Ping_Header : Header
        {
            uint32_t time_point; //ms
            uint16_t seq;
        };

        struct Pong_Header : Header
        {
            uint32_t time_point; //ms
            uint16_t seq;
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
                    auto item = std::move(pool.back());
                    pool.pop_back();
                    return item;
                }
                else
                {
                    return std::make_shared<T>();
                }
            }
            void release(std::shared_ptr<T>&& item)
            {
                std::lock_guard<std::mutex> lg(mutex);
                pool.emplace_back(std::move(item));
            }
        };

        static const int MAX_PRIORITY = 127;
        static const int MIN_PRIORITY = -127;
        //special priorities
        static const int DENY_PRIORITY = -128;

        struct TX
        {
            struct Datagram
            {
                Send_Params params;
                int priority = 0;
                q::Clock::time_point added = q::Clock::time_point(q::Clock::duration{0});
                q::Clock::time_point sent = q::Clock::time_point(q::Clock::duration{0});
                Buffer_t data;
            };
            typedef std::shared_ptr<Datagram> Datagram_ptr;

            Pool<Datagram> datagram_pool;

            Datagram_ptr acquire_datagram(size_t data_size)
            {
                auto datagram = datagram_pool.acquire();
                datagram->data.resize(0); //this will force zero-ing the data
                datagram->data.resize(data_size);
                datagram->priority = 0;
                datagram->added = q::Clock::time_point(q::Clock::duration{0});
                datagram->sent = q::Clock::time_point(q::Clock::duration{0});
                return datagram;
            }
            void release_datagram(Datagram_ptr&& datagram)
            {
                if (datagram)
                {
                    datagram_pool.release(std::move(datagram));
                }
                datagram.reset();
            }

            std::mutex confirmatrions_mutex;
            Datagram_ptr confirmations;

            std::mutex cancellations_mutex;
            Datagram_ptr cancellations;

            typedef std::vector<Datagram_ptr> Send_Queue;

            struct Internal_Queues
            {
                std::mutex mutex;
                Send_Queue ping;
                Send_Queue packet_comfirmation;
                Send_Queue packet_cancel;
            } internal_queues;

            std::mutex packet_queue_mutex;
            Send_Queue packet_queue;
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
            void release_datagram(Datagram_ptr&& datagram)
            {
                if (datagram)
                {
                    datagram_pool.release(std::move(datagram));
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
            void release_packet(Packet_ptr&& packet)
            {
                if (packet)
                {
                    for (auto& f: packet->fragments)
                    {
                        release_datagram(std::move(f));
                        f.reset();
                    }
                    packet_pool.release(std::move(packet));
                }
                packet.reset();
            }

            struct Packet_Queue
            {
                std::map<uint32_t, Packet_ptr> packets;
                std::mutex mutex;
            };

            //waiting to be received
            std::array<Packet_Queue, MAX_CHANNELS> packet_queues;
            std::array<uint32_t, MAX_CHANNELS> last_packet_ids = {0};
        } m_rx;

        struct Ping
        {
            uint16_t last_seq = 0;
            q::Clock::time_point last_time_point;
            std::vector<Ping_Header> sent;
            std::mutex sent_mutex;

            boost::circular_buffer<std::pair<q::Clock::time_point, q::Clock::duration>> rtts;
            q::Clock::duration rtt = q::Clock::duration{0};
        } m_ping;


        struct Stats
        {
            size_t tx_datagrams = 0;
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

        q::Clock::time_point m_init_time_point;
        std::atomic<uint32_t> m_last_id = {0};
        Stats m_global_stats;

        std::atomic_bool m_is_sending = {false};
        const q::Clock::duration MIN_RESEND_DURATION = std::chrono::milliseconds(1000);

        size_t m_mtu = 32;
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
            case Type::TYPE_PACKETS_CONFIRMED: return sizeof(Packets_Confirmed_Header);
            case Type::TYPE_PACKETS_CANCELLED: return sizeof(Packets_Cancelled_Header);
            case Type::TYPE_PING: return sizeof(Ping_Header);
            case Type::TYPE_PONG: return sizeof(Pong_Header);
            }
            QASSERT(0);
            return 0;
        }

        uint32_t compute_crc(uint8_t const* data, size_t size)
        {
            auto crc = q::util::compute_murmur_hash32(data, size);
            //crc ^= (crc >> 28);
            //crc &= 0xFFFFF;
            return crc;
        }

        TX::Send_Queue::iterator process_packet_queue()
        {
//            std::lock_guard<std::mutex> lg(m_tx.to_send_mutex);

            auto& queue = m_tx.packet_queue;
            if (queue.empty())
            {
                return queue.end();
            }

            auto now = q::Clock::now();

            auto min_resend_duration = std::max(MIN_RESEND_DURATION, m_ping.rtt / 10);

            auto best_it = queue.begin();
            auto best_age = now - (*best_it)->sent;

            //calculate priorities
            for (auto it = queue.begin(); it != queue.end();)
            {
                auto& datagram = *it;
                auto const& params = datagram->params;

                //eliminate old datagrams
                if (params.cancel_after.count() > 0 && now - datagram->added >= params.cancel_after)
                {
                    m_tx.release_datagram(std::move(datagram));
                    erase_unordered(queue, it);
                    continue;
                }

                if (now - datagram->sent < min_resend_duration)
                {
                    datagram->priority = DENY_PRIORITY;
                }
                else
                {
                    int priority = params.importance;
                    if (params.bump_priority_after.count() > 0 && now - datagram->added >= params.bump_priority_after)
                    {
                        priority += 63; //bump it by half
                    }

                    datagram->priority = priority;
                }

                {
                    auto age = now - datagram->sent;
                    if ((datagram->priority > (*best_it)->priority) || (datagram->priority == (*best_it)->priority && age > best_age))
                    {
                        best_it = it;
                        best_age = age;
                    }
                }
                ++it;
            }

            return best_it;
        }

        void prepare_to_send_datagram(TX::Datagram& datagram)
        {
            auto& header = get_header<Header>(datagram.data);
            auto crc = compute_crc(datagram.data.data(), datagram.data.size());
            header.crc = crc;

            datagram.added = q::Clock::now();
            datagram.sent = q::Clock::time_point(q::Clock::duration{0});
            datagram.priority = 0;

//            if (header.crc == 0)
//            {
//                for (auto x: datagram->data)
//                {
//                    SILK_INFO("{}", x);
//                }
//                SILK_INFO("crc: {}/{}", crc, static_cast<int>(header.crc));
//            }

//            QASSERT_MSG(header.crc != 0, "{}", crc);
        }

        void add_and_send_datagram(TX::Send_Queue& queue, std::mutex& mutex, TX::Datagram_ptr const& datagram)
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

        void update_stats(Stats& stats, TX::Datagram const& datagram)
        {
            auto const& header = get_header<Header>(datagram.data);
            stats.tx_datagrams++;
            stats.tx_bytes += datagram.data.size();
            if (header.type == TYPE_PACKET)
            {
                auto const& pheader = get_header<Packet_Header>(datagram.data);
                if (pheader.fragment_idx == 0)
                {
                    stats.tx_packets++;
                }
            }
            else if (header.type == TYPE_PING)
            {
                stats.tx_pings++;
            }
            else if (header.type == TYPE_PONG)
            {
                stats.tx_pongs++;
            }
        }

        //to avoid popping front in vectors
        //Note - I use a vector instead of a deque because the deque is too slow at iterating.
        //The pop_front can be implemented optimally for vectors if order need not be preserved.
        template<class T> static void erase_unordered(std::vector<T>& c, typename std::vector<T>::iterator it)
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

        void send_datagram()
        {
            if (m_is_sending.exchange(true))
            {
                //was already sending, return
                return;
            }

            TX::Datagram_ptr datagram;

            {
                std::lock_guard<std::mutex> lg(m_tx.internal_queues.mutex);
                //first try the pings
                {
                    auto& queue = m_tx.internal_queues.ping;
                    if (!queue.empty())
                    {
                        datagram = std::move(queue.front());
                        erase_unordered(queue, queue.begin());
                    }
                }
                //next the confirmations
                if (!datagram)
                {
                    auto& queue = m_tx.internal_queues.packet_comfirmation;
                    if (!queue.empty())
                    {
                        datagram = std::move(queue.front());
                        erase_unordered(queue, queue.begin());
                    }
                }
                //next the cancels
                if (!datagram)
                {
                    auto& queue = m_tx.internal_queues.packet_cancel;
                    if (!queue.empty())
                    {
                        datagram = std::move(queue.front());
                        erase_unordered(queue, queue.begin());
                    }
                }
            }

            //next normal packets
            if (!datagram)
            {
                auto& queue = m_tx.packet_queue;
                std::lock_guard<std::mutex> lg(m_tx.packet_queue_mutex);

                auto it = process_packet_queue();
                if (it != queue.end() && (*it)->priority >= MIN_PRIORITY)
                {
                    //if the datagram is not reliable remove it from the queue. Otherwise leave it there to be resent
                    if (!(*it)->params.is_reliable)
                    {
                        datagram = std::move(*it);
                        erase_unordered(m_tx.packet_queue, it);
                    }
                    else
                    {
                        datagram = *it;
                        datagram->sent = q::Clock::now();
                    }
                }
            }
            if (!datagram)
            {
                m_is_sending = false;
                return;
            }

            QASSERT(datagram);

            update_stats(m_global_stats, *datagram);

//            m_socket.async_send_to(boost::asio::buffer(datagram->data.data(), datagram->data.size()),
//                                   m_destination,
//                                   boost::bind(&RUDP::handle_send, this,
//                                   datagram,
//                                   boost::asio::placeholders::error));


//            SILK_INFO("Sending datagram {}", static_cast<int>(get_header<Header>(datagram->data).id));

            std::thread([this,datagram]()
            {
//                SILK_INFO("Receiving datagram {}", static_cast<int>(get_header<Header>(datagram->data).id));
                QASSERT(datagram->data.size() <= m_rx.temp_buffer.size());
                std::copy(datagram->data.begin(), datagram->data.end(), m_rx.temp_buffer.begin());
                handle_receive(boost::system::error_code(), datagram->data.size());
                handle_send(datagram, boost::system::error_code());
            }).detach();
        }

        void handle_send(std::shared_ptr<TX::Datagram> datagram,
                         const boost::system::error_code& /*error*/)
        {
            auto& header = get_header<Header>(datagram->data);
            if (header.type != TYPE_PACKET || !datagram->params.is_reliable)
            {
                m_tx.release_datagram(std::move(datagram));
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

                    process_incoming_datagram(datagram);
                    QASSERT(datagram == nullptr);
                }
                //start();
            }
        }

        void send_pending_confirmations()
        {
            std::lock_guard<std::mutex> lg(m_tx.confirmatrions_mutex);
            if (m_tx.confirmations)
            {
                auto& cheader = get_header<Packets_Confirmed_Header>(m_tx.confirmations->data);
                if (cheader.count > 0)
                {
                    cheader.type = TYPE_PACKETS_CONFIRMED;
                    add_and_send_datagram(m_tx.internal_queues.packet_comfirmation, m_tx.internal_queues.mutex, m_tx.confirmations);
                    m_tx.confirmations.reset();
                }
            }
        }

        void add_packet_confirmation(Packet_Header const& header)
        {
            std::lock_guard<std::mutex> lg(m_tx.confirmatrions_mutex);
            if (!m_tx.confirmations)
            {
                m_tx.confirmations = m_tx.acquire_datagram(sizeof(Packets_Confirmed_Header));
            }

            auto* cheader = &get_header<Packets_Confirmed_Header>(m_tx.confirmations->data);
            if (cheader->count >= Packets_Confirmed_Header::MAX_PACKED)
            {
                cheader->type = TYPE_PACKETS_CONFIRMED;
                add_and_send_datagram(m_tx.internal_queues.packet_comfirmation, m_tx.internal_queues.mutex, m_tx.confirmations);

                m_tx.confirmations = m_tx.acquire_datagram(sizeof(Packets_Confirmed_Header));
                cheader = &get_header<Packets_Confirmed_Header>(m_tx.confirmations->data);
            }

            cheader->count++;

            auto& data = m_tx.confirmations->data;
            auto off = data.size();
            data.resize(off + sizeof(uint32_t));
            //AFTER this point, the header might be broken

            uint32_t id = header.id;
            uint32_t fragment_idx = header.fragment_idx;
            uint32_t cd = (id << 8) | fragment_idx;
            std::copy(reinterpret_cast<uint8_t*>(&cd), reinterpret_cast<uint8_t*>(&cd) + sizeof(uint32_t), data.data() + off);
        }
        void send_pending_cancellations()
        {
            std::lock_guard<std::mutex> lg(m_tx.cancellations_mutex);
            if (m_tx.cancellations)
            {
                auto& cheader = get_header<Packets_Cancelled_Header>(m_tx.cancellations->data);
                if (cheader.count > 0)
                {
                    cheader.type = TYPE_PACKETS_CANCELLED;
                    add_and_send_datagram(m_tx.internal_queues.packet_cancel, m_tx.internal_queues.mutex, m_tx.cancellations);
                    m_tx.cancellations.reset();
                }
            }
        }

        void add_packet_cancellation(Packet_Header const& header)
        {
            std::lock_guard<std::mutex> lg(m_tx.cancellations_mutex);
            if (!m_tx.cancellations)
            {
                m_tx.cancellations = m_tx.acquire_datagram(sizeof(Packets_Cancelled_Header));
            }

            auto* cheader = &get_header<Packets_Cancelled_Header>(m_tx.cancellations->data);
            if (cheader->count >= Packets_Cancelled_Header::MAX_PACKED)
            {
                cheader->type = TYPE_PACKETS_CANCELLED;
                add_and_send_datagram(m_tx.internal_queues.packet_cancel, m_tx.internal_queues.mutex, m_tx.cancellations);

                m_tx.cancellations = m_tx.acquire_datagram(sizeof(Packets_Cancelled_Header));
                cheader = &get_header<Packets_Cancelled_Header>(m_tx.cancellations->data);
            }

            cheader->count++;

            auto& data = m_tx.cancellations->data;
            auto off = data.size();
            data.resize(data.size() + sizeof(uint32_t));
            //AFTER this point, the header might be broken

            uint32_t id = header.id;
            std::copy(reinterpret_cast<uint8_t*>(&id), reinterpret_cast<uint8_t*>(&id) + sizeof(uint32_t), data.data() + off);
        }

        void send_packet_ping()
        {
            auto datagram = m_tx.acquire_datagram(sizeof(Ping_Header));
            auto& header = get_header<Ping_Header>(datagram->data);
            header.seq = ++m_ping.last_seq;
            header.type = TYPE_PING;
            header.time_point = static_cast<uint32_t>(std::chrono::duration_cast<std::chrono::milliseconds>(q::Clock::now() - m_init_time_point).count());

            {
                std::lock_guard<std::mutex> lg(m_ping.sent_mutex);
                m_ping.sent.push_back(header);
            }
            add_and_send_datagram(m_tx.internal_queues.ping, m_tx.internal_queues.mutex, datagram);
        }
        void send_packet_pong(Ping_Header const& ping)
        {
            auto datagram = m_tx.acquire_datagram(sizeof(Pong_Header));
            auto& header = get_header<Pong_Header>(datagram->data);
            header.seq = ping.seq;
            header.type = TYPE_PONG;
            header.time_point = ping.time_point;
            add_and_send_datagram(m_tx.internal_queues.ping, m_tx.internal_queues.mutex, datagram);
        }

        void process_incoming_datagram(RX::Datagram_ptr& datagram)
        {
            QASSERT(datagram);
            size_t h_size = get_header_size(datagram->data);
            if (h_size == 0)
            {
                SILK_WARNING("Unknonw header.");
                m_rx.release_datagram(std::move(datagram));
                return;
            }

            auto& header = get_header<Header>(datagram->data);

            m_global_stats.rx_datagrams++;

            auto crc1 = header.crc;
            header.crc = 0;
            auto crc2 = compute_crc(datagram->data.data(), datagram->data.size());
            if (crc1 != crc2)
            {
                m_global_stats.rx_corrupted_datagrams++;
                auto loss = m_global_stats.rx_corrupted_datagrams * 100 / m_global_stats.rx_datagrams;

                SILK_WARNING("Crc is wrong. {} != {}. Packet loss: {.2}", crc1, crc2, loss);
                m_rx.release_datagram(std::move(datagram));
                return;
            }
            m_global_stats.rx_good_datagrams++;

            switch (header.type)
            {
            case Type::TYPE_PACKET: process_packet_datagram(datagram); break;
            case Type::TYPE_PACKETS_CONFIRMED: process_packets_confirmed_datagram(datagram); break;
            case Type::TYPE_PACKETS_CANCELLED: process_packets_cancelled_datagram(datagram); break;
            case Type::TYPE_PING: process_ping_datagram(datagram); break;
            case Type::TYPE_PONG: process_pong_datagram(datagram); break;
            default: QASSERT(0); m_rx.release_datagram(std::move(datagram)); break;
            }
            m_rx.release_datagram(std::move(datagram));
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
                    add_packet_cancellation(header);
                }

                m_rx.release_datagram(std::move(datagram));
                return;
            }

            if (header.flag_is_reliable)
            {
                add_packet_confirmation(header);
            }

            auto& queue = m_rx.packet_queues[ch];
            std::lock_guard<std::mutex> lg(queue.mutex);
            auto& packet = queue.packets[id];
            if (!packet)
            {
                packet = m_rx.acquire_packet();
            }

            if (packet->fragments[fragment_idx]) //we already have the fragment
            {
                m_global_stats.rx_duplicated_datagrams++;

                SILK_WARNING("Duplicated fragment {} for packet {}.", fragment_idx, id);
                m_rx.release_datagram(std::move(datagram));
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
        void process_packets_confirmed_datagram(RX::Datagram_ptr& datagram)
        {
            QASSERT(datagram);
            auto const& header = get_header<Packets_Confirmed_Header>(datagram->data);

            auto count = header.count;
            QASSERT(count > 0);

            QASSERT(datagram->data.size() == sizeof(Packets_Confirmed_Header) + sizeof(uint32_t)*count);

            //first unpack the confirmations in an array
            auto* src = datagram->data.data() + sizeof(Packets_Confirmed_Header);

            std::array<uint32_t, Packets_Confirmed_Header::MAX_PACKED> conf;
            std::copy(src, src + sizeof(uint32_t)*count, reinterpret_cast<uint8_t*>(conf.data()));
            auto conf_end = conf.begin() + count;

            //sort the confirmations ascending so we can search fast
            std::sort(conf.begin(), conf_end);

            std::lock_guard<std::mutex> lg(m_tx.packet_queue_mutex);
            auto size_before = m_tx.packet_queue.size();
            for (auto it = m_tx.packet_queue.begin(); it != m_tx.packet_queue.end();)
            {
                auto const& hdr = get_header<Packet_Header>((*it)->data);
                uint32_t x = (hdr.id << 8) | hdr.fragment_idx;
                auto lb = std::lower_bound(conf.begin(), conf_end, x);
                if (lb != conf_end && *lb == x)
                {
                    m_tx.release_datagram(std::move(*it));
                    erase_unordered(m_tx.packet_queue, it);
                    count--;
                    if (count == 0)
                    {
                        break;
                    }
                    //TODO - eliminate the element from conf as well
                }
                else
                {
                    ++it;
                }
            }


            auto size_after = m_tx.packet_queue.size();
            QASSERT(size_before > size_after);
            if (size_before >= size_after)
            {
                m_global_stats.tx_confirmed_datagrams += size_before - size_after;
                //HH SILK_INFO("Confirming fragment {} for packet {}", fragment_idx, id);
            }

            m_rx.release_datagram(std::move(datagram));
        }
        void process_packets_cancelled_datagram(RX::Datagram_ptr& datagram)
        {
            QASSERT(datagram);
            auto const& header = get_header<Packets_Cancelled_Header>(datagram->data);

            auto count = header.count;
            QASSERT(count > 0);

            QASSERT(datagram->data.size() == sizeof(Packets_Confirmed_Header) + sizeof(uint32_t)*count);

            //first unpack the confirmations in an array
            auto* src = datagram->data.data() + sizeof(Packets_Confirmed_Header);

            std::array<uint32_t, Packets_Confirmed_Header::MAX_PACKED> conf;
            std::copy(src, src + sizeof(uint32_t)*count, reinterpret_cast<uint8_t*>(conf.data()));
            auto conf_end = conf.begin() + count;

            //sort the confirmations ascending so we can search fast
            std::sort(conf.begin(), conf_end);

            auto size_before = m_tx.packet_queue.size();

            std::lock_guard<std::mutex> lg(m_tx.packet_queue_mutex);
            m_tx.packet_queue.erase(std::remove_if(m_tx.packet_queue.begin(), m_tx.packet_queue.end(),
                    [&conf, &conf_end](const TX::Datagram_ptr& d)
                    {
                        auto const& hdr = get_header<Packet_Header>(d->data);
                        auto lb = std::lower_bound(conf.begin(), conf_end, hdr.id);
                        return (hdr.type != TYPE_PACKETS_CANCELLED && lb != conf_end && *lb == hdr.id);
                    }), m_tx.packet_queue.end());

            auto size_after = m_tx.packet_queue.size();
            QASSERT(size_before >= size_after);
            if (size_before > size_after)
            {
                m_global_stats.tx_cancelled_datagrams += size_before - size_after;
                m_global_stats.tx_cancelled_packets ++;
                SILK_INFO("Cancelling packets {}: {} fragments removed", count, size_before - size_after);
            }

            m_rx.release_datagram(std::move(datagram));
        }
        void process_ping_datagram(RX::Datagram_ptr& datagram)
        {
            QASSERT(datagram);
            auto& header = get_header<Ping_Header>(datagram->data);

            m_global_stats.rx_pings++;
            send_packet_pong(header);

            m_rx.release_datagram(std::move(datagram));
        }
        void process_pong_datagram(RX::Datagram_ptr& datagram)
        {
            QASSERT(datagram);
            auto& header = get_header<Pong_Header>(datagram->data);

            m_global_stats.rx_pongs++;

            auto seq = header.seq;
            auto now = q::Clock::now();

            //find the ping and compute its rtt
            {
                std::lock_guard<std::mutex> lg(m_ping.sent_mutex);
                auto it = std::find_if(m_ping.sent.begin(), m_ping.sent.end(), [seq](Ping_Header const& p) { return p.seq == seq; });
                if (it != m_ping.sent.end())
                {
                    auto rtt = now - m_init_time_point - std::chrono::milliseconds(it->time_point);
                    m_ping.rtts.push_back(std::make_pair(now, rtt));
                    erase_unordered(m_ping.sent, it);
                    //SILK_INFO("good ping seq received: {}", seq);
                }
                else
                {
                    SILK_WARNING("invalid ping seq received: {}", seq);
                }
            }

            //calculate average rtt
            q::Clock::duration total_rtt{0};
            size_t total = 0;
            for (auto it = m_ping.rtts.begin(); it != m_ping.rtts.end(); ++it)
            {
                auto when = it->first;
                auto rtt = it->second;
                if (now - when < std::chrono::seconds(1))
                {
                    total_rtt += rtt;
                    total++;
                }
            }
            if (total > 0)
            {
                total_rtt /= total;
            }
            m_ping.rtt = total_rtt;

            static q::Clock::time_point xxx = q::Clock::now();
//                if (q::Clock::now() - xxx > std::chrono::milliseconds(1000))
            {
                xxx = q::Clock::now();
                SILK_INFO("RTT: {}", m_ping.rtt);
            }

            m_rx.release_datagram(std::move(datagram));
        }

   };

    inline RUDP::RUDP(Socket_t& socket)
        : m_socket(socket)
    {
        auto hsz =sizeof(Header);
        hsz =sizeof(Packet_Main_Header);
        hsz =sizeof(Packet_Header);

        hsz =sizeof(Packets_Confirmed_Header);
        hsz =sizeof(Ping_Header);

        m_global_receive_params.max_receive_time = std::chrono::seconds(5);

        m_init_time_point = q::Clock::now();
        m_ping.last_time_point = q::Clock::now();
        m_ping.rtts.set_capacity(100);
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
        size_t fragment_count = ((size - 1) / max_fragment_size) + 1;
        if (fragment_count > 255)
        {
            //SILK_WARNING("Too many fragments: {}. Ignoring mtu.", fragment_count);
            fragment_count = 255;
            max_fragment_size = ((size - 1) / fragment_count) + 1;
            fragment_count = ((size - 1) / max_fragment_size) + 1;
        }

        //auto now = q::Clock::now();

        {
            std::lock_guard<std::mutex> lg(m_tx.packet_queue_mutex);

            auto id = ++m_last_id;
            for (size_t i = 0; i < fragment_count; i++)
            {
                QASSERT(left > 0);
                auto fragment_size = math::min(max_fragment_size, left);

                auto datagram_size = ((i == 0) ? sizeof(Packet_Main_Header) : sizeof(Packet_Header)) + fragment_size;
                auto fragment = m_tx.acquire_datagram(datagram_size);

                auto& params = m_send_params[channel_idx];
                fragment->params = params;

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
                    //auto& f_header = get_header<Packet_Header>(fragment->data);
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

                //HH SILK_INFO("Sending fragment {} for packet {}", header.fragment_idx, id);

                data += fragment_size;
                left -= fragment_size;

                prepare_to_send_datagram(*fragment);
                m_tx.packet_queue.push_back(fragment);
            }
        }

        send_datagram();
    }

    template<class T>
    inline bool RUDP::receive(uint8_t channel_idx, T& data)
    {
        data.clear();
        if (channel_idx >= MAX_CHANNELS)
        {
            QASSERT(0);
            return false;
        }

        auto& queue = m_rx.packet_queues[channel_idx];
        std::lock_guard<std::mutex> lg(queue.mutex);
        if (queue.packets.empty())
        {
            return false;
        }
        auto packet = queue.packets.begin()->second;
        QASSERT(packet);

        auto id = packet->any_header.id;
        QASSERT(id > 0);

        auto now = q::Clock::now();

        //no header yet or not all packages received?
        if (!packet->fragments[0] || packet->received_fragment_count < packet->main_header.fragment_count)
        {
            auto const& params = m_receive_params[channel_idx];
            auto max_receive_time = params.max_receive_time.count() > 0 ? params.max_receive_time : m_global_receive_params.max_receive_time;
            if (now - packet->added >= max_receive_time)
            {
                SILK_WARNING("Canceling packet {}", id);
                add_packet_cancellation(packet->any_header);
                send_pending_cancellations();
                m_rx.release_packet(std::move(packet));
                queue.packets.erase(queue.packets.begin());
                m_rx.last_packet_ids[channel_idx] = id;
                m_global_stats.rx_dropped_packets++;
            }

            //SILK_WARNING("Still waiting for packet {}: {}/{} received", id, packet->received_fragment_count, static_cast<size_t>(packet->main_header.fragment_count));
            return false;
        }
        QASSERT(packet->fragments[0]);

        //SILK_INFO("Received packet {}", id);

        queue.packets.erase(queue.packets.begin());
        m_rx.last_packet_ids[channel_idx] = id;
        m_global_stats.rx_packets++;

        //copy the data
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

        m_rx.release_packet(std::move(packet));
        return true;
    }

    inline void RUDP::process()
    {
        send_pending_confirmations();
        send_pending_cancellations();

        auto now = q::Clock::now();
        if (now - m_ping.last_time_point > std::chrono::milliseconds(100))
        {
            m_ping.last_time_point = now;
            send_packet_ping();
        }
    }

    inline void RUDP::set_send_params(uint8_t channel_idx, Send_Params const& params)
    {
        if (channel_idx >= MAX_CHANNELS)
        {
            QASSERT(0);
            return;
        }
        m_send_params[channel_idx] = params;
    }
    inline void RUDP::set_receive_params(uint8_t channel_idx, Receive_Params const& params)
    {
        if (channel_idx >= MAX_CHANNELS)
        {
            QASSERT(0);
            return;
        }
        m_receive_params[channel_idx] = params;
    }

}
