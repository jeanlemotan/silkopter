#pragma once

#include <boost/optional.hpp>

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
        void set_reliable_receive_params(Receive_Params const& params);
        void set_unreliable_receive_params(Receive_Params const& params);

        void send(uint8_t channel_idx, uint8_t const* data, size_t size);
        bool receive(uint8_t channel_idx, std::vector<uint8_t>& data);

        void process();

    private:

        enum Type
        {
            TYPE_1ST_PIECE      =   0,
            TYPE_PIECE          =   1,
            TYPE_CANCEL         =   2,
            TYPE_CONFIRM        =   3,
            TYPE_PING           =   4,
            TYPE_PONG           =   5,
        };

#pragma pack(push, 1)
        struct Header
        {
            uint32_t id : 24;
            uint32_t piece_idx : 8;
            uint8_t flag_is_reliable : 1;
            uint8_t type : 3;
            uint8_t channel_idx;
            uint32_t crc;
        };

        struct Piece_1st_Header : Header
        {
            uint16_t piece_size;
            uint32_t total_data_size : 24;
            uint32_t piece_count : 8;
        };

        struct Piece_Header : Header
        {
        };

        struct Cancel_Header : Header
        {
        };

        struct Confirm_Header : Header
        {
            uint32_t confirmed_ids; //bit X confirms datagram id + X
        };

        struct Ping_Header : Header
        {
            uint32_t time_point; //ms
        };
        struct Pong_Header : Header
        {
            uint32_t time_point; //ms
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

        struct TX
        {
            struct Datagram
            {
                Send_Params params;
                float priority = 0;
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
            std::deque<Datagram_ptr> send_queue;
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
                size_t received_piece_count = 0;
                q::Clock::time_point added;
                Piece_1st_Header header;
                std::array<Datagram_ptr, 256> pieces;
            };
            typedef std::shared_ptr<Packet> Packet_ptr;
            Pool<Packet> packet_pool;

            Packet_ptr acquire_packet()
            {
                auto packet = packet_pool.acquire();
                packet->received_piece_count = 0;
                packet->added = q::Clock::now();
            }
            void release_packet(Packet_ptr& packet)
            {
                if (packet)
                {
                    for (auto& f: packet->pieces)
                    {
                        release_datagram(f);
                        f.reset();
                    }
                    packet_pool.release(packet);
                }
                packet.reset();
            }

            //waiting to be received
            typedef std::map<uint32_t, Packet_ptr> Packet_Queue;
            std::array<Packet_Queue, 256> receive_queues;

            std::array<uint32_t, 256> last_packet_ids = {0};
        } m_rx;

        //sent pings
        std::deque<Ping_Header> m_pings;
        std::mutex m_pings_mutex;
        boost::circular_buffer<q::Clock::duration> m_rtts;
        q::Clock::duration m_rtt;


        struct Stats
        {
            size_t packets_sent = 0;
            size_t packets_received = 0;
            std::array<size_t, 16> sent_per_type = {0};
            std::array<size_t, 16> received_per_type = {0};
            size_t bytes_sent = 0;
            size_t bytes_received = 0;
            size_t payload_bytes_sent = 0;
            size_t payload_bytes_received = 0;
        };

        size_t m_pings_sent;
        size_t m_pongs_received;

        std::atomic<uint32_t> m_last_id = {0};
        std::array<Stats, 256> m_channel_stats;
        Stats m_global_stats;

        std::atomic_bool m_is_sending = {false};
        const q::Clock::duration MIN_RESEND_DURATION = std::chrono::milliseconds(2);

        size_t m_mtu = 1024;
        boost::asio::ip::udp::endpoint m_destination;

        std::array<Send_Params, 256> m_send_params;
        std::array<Receive_Params, 256> m_receive_params;
        Receive_Params m_reliable_receive_params;
        Receive_Params m_unreliable_receive_params;



        template<class H>
        H& get_header(Buffer_t& data)
        {
            if (data.size() < sizeof(H))
            {
                data.resize(sizeof(H));
            }
            auto& h = *reinterpret_cast<H*>(data.data());
            return h;
        }
        size_t get_header_size(Buffer_t& data)
        {
            if (data.size() < sizeof(Header))
            {
                QASSERT(0);
                return 0;
            }
            auto& header = get_header<Header>(data);
            switch (header.type)
            {
            case Type::TYPE_1ST_PIECE: return sizeof(Piece_1st_Header);
            case Type::TYPE_PIECE: return sizeof(Piece_Header);
            case Type::TYPE_CONFIRM: return sizeof(Confirm_Header);
            case Type::TYPE_CANCEL: return sizeof(Cancel_Header);
            case Type::TYPE_PING: return sizeof(Ping_Header);
            case Type::TYPE_PONG: return sizeof(Pong_Header);
            }
            QASSERT(0);
            return 0;
        }

        void process_send_queue()
        {
//            std::lock_guard<std::mutex> lg(m_tx.to_send_mutex);

            auto now = q::Clock::now();

            //eliminate old pieces
            m_tx.send_queue.erase(std::remove_if(m_tx.send_queue.begin(), m_tx.send_queue.end(),
                    [&](const TX::Datagram_ptr& f)
                    {
                        return f->params.cancel_after.count() > 0 &&
                               now - f->added >= f->params.cancel_after;
                    }), m_tx.send_queue.end());

            //calculate priorities
            for (auto& datagram: m_tx.send_queue)
            {
                if (now - datagram->sent < std::min(MIN_RESEND_DURATION, m_rtt))
                {
                    datagram->priority = -1000;
                }
                else
                {
                    float priority = static_cast<float>(datagram->params.importance) * 0.0078125f;
                    if (datagram->params.bump_priority_after.count() > 0 &&
                            now - datagram->added >= datagram->params.bump_priority_after)
                    {
                        priority += 0.5f;
                    }

                    datagram->priority = priority;
                }
                auto& header = get_header<Header>(datagram->data);
                if (header.type == TYPE_CONFIRM)
                {
                    datagram->priority += 1.f;
                }
            }

            std::nth_element(m_tx.send_queue.begin(), m_tx.send_queue.begin(), m_tx.send_queue.end(),
                            [](const TX::Datagram_ptr& f0, const TX::Datagram_ptr& f1)
                            {
                                return f0->priority > f1->priority;
                            });
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

                process_send_queue();
                if (m_tx.send_queue.empty())
                {
                    return;
                }

                datagram = m_tx.send_queue.front();
                if (datagram->priority < -1.f)
                {
                    return;
                }

                //if the fragmjent is not reliable remove it from the queue. Otherwise leave it there to be resent
                if (!datagram->params.is_reliable)
                {
                    m_tx.send_queue.pop_front();
                }
            }
            QASSERT(datagram);

            Header const& header = get_header<Header>(datagram->data);
            Stats& stats = m_channel_stats[header.channel_idx];
            stats.bytes_sent += datagram->data.size();
            stats.sent_per_type[header.type] ++;
            if (header.type == Type::TYPE_1ST_PIECE)
            {
                stats.packets_sent++;
            }

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
            m_rx.incoming_queue.push_back(rx_datagram);
            m_is_sending = false;
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

        void send_confirm(Header const& header)
        {
            auto datagram = m_tx.acquire_datagram(sizeof(Confirm_Header));
            datagram->added = q::Clock::now();

            auto& cheader = get_header<Confirm_Header>(datagram->data);
            cheader.id = header.id;
            cheader.piece_idx = header.piece_idx;
            cheader.channel_idx = header.channel_idx;
            cheader.flag_is_reliable = false;
            cheader.type = TYPE_CONFIRM;

            auto crc = q::util::compute_murmur_hash32(datagram->data.data(), datagram->data.size());
            cheader.crc = crc;

            add_and_send_datagram(datagram);
        }

        void send_cancel(Header const& header)
        {
            auto datagram = m_tx.acquire_datagram(sizeof(Cancel_Header));

            datagram->added = q::Clock::now();

            auto& cheader = get_header<Cancel_Header>(datagram->data);
            cheader.id = header.id;
            cheader.piece_idx = header.piece_idx;
            cheader.channel_idx = header.channel_idx;
            cheader.flag_is_reliable = false;
            cheader.type = TYPE_CANCEL;

            auto crc = q::util::compute_murmur_hash32(datagram->data.data(), datagram->data.size());
            cheader.crc = crc;

            add_and_send_datagram(datagram);
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

            auto crc1 = header.crc;
            header.crc = 0;
            auto crc2 = q::util::compute_murmur_hash32(datagram->data.data(), datagram->data.size());
            if (crc1 != crc2)
            {
                SILK_WARNING("Crc is wrong. {} != {}", crc1, crc2);
                m_rx.release_datagram(datagram);
                return;
            }

            switch (header.type)
            {
            case Type::TYPE_1ST_PIECE: process_piece_datagram(datagram); break;
            case Type::TYPE_PIECE: process_piece_datagram(datagram); break;
            case Type::TYPE_CONFIRM: process_confirm_datagram(datagram); break;
            case Type::TYPE_CANCEL: process_cancel_datagram(datagram); break;
            case Type::TYPE_PING: process_ping_datagram(datagram); break;
            case Type::TYPE_PONG: process_pong_datagram(datagram); break;
            }
        }

        void process_piece_datagram(RX::Datagram_ptr& datagram)
        {
            QASSERT(datagram);
            auto& header = get_header<Header>(datagram->data);

            auto ch = header.channel_idx;
            if (header.id <= m_rx.last_packet_ids[ch])
            {
                SILK_WARNING("Blast from the past - datagram {} for packet id {}.", static_cast<size_t>(header.piece_idx), static_cast<size_t>(header.id));

                if (header.flag_is_reliable)
                {
                    send_cancel(header);
                }

                m_rx.release_datagram(datagram);
                return;
            }

            if (header.flag_is_reliable)
            {
                send_confirm(header);
            }

            auto& queue = m_rx.receive_queues[ch];
            auto& packet = queue[header.id];
            if (!packet)
            {
                packet = m_rx.acquire_packet();
            }

            if (packet->pieces[header.piece_idx]) //we already have the piece
            {
                SILK_WARNING("Duplicated piece {} for packet id {}.", static_cast<size_t>(header.piece_idx), static_cast<size_t>(header.id));
                m_rx.release_datagram(datagram);
                return;
            }

            packet->received_piece_count++;
            if (header.type == TYPE_1ST_PIECE)
            {
                packet->header = get_header<Piece_1st_Header>(datagram->data);
            }
            packet->pieces[header.piece_idx] = datagram;
        }
        void process_confirm_datagram(RX::Datagram_ptr const& datagram)
        {
            QASSERT(datagram);
            auto& header = get_header<Confirm_Header>(datagram->data);

            auto id = header.id;

        }
        void process_cancel_datagram(RX::Datagram_ptr const& datagram)
        {
            QASSERT(datagram);
            auto& header = get_header<Cancel_Header>(datagram->data);
        }
        void process_ping_datagram(RX::Datagram_ptr const& datagram)
        {
            QASSERT(datagram);
            auto& header = get_header<Ping_Header>(datagram->data);
        }
        void process_pong_datagram(RX::Datagram_ptr const& datagram)
        {
            QASSERT(datagram);
            auto& header = get_header<Pong_Header>(datagram->data);
        }


   };

    inline RUDP::RUDP(Socket_t& socket)
        : m_socket(socket)
    {
        auto hsz =sizeof(Header);
        hsz =sizeof(Piece_1st_Header);
        hsz =sizeof(Piece_Header);
        hsz =sizeof(Cancel_Header);
        hsz =sizeof(Confirm_Header);
        hsz =sizeof(Ping_Header);
        hsz == hsz;
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
        if (!data || size == 0)
        {
            QASSERT(0);
            return;
        }

        size_t piece_size = m_mtu;
        size_t left = size;
        size_t piece_count = ((size - 1) / m_mtu) + 1;
        if (piece_count > 255)
        {
            SILK_ERR("Too many pieces: {}. Ignoring mtu.", piece_count);
            piece_count = 255;
            piece_size = ((size - 1) / piece_count) + 1;
        }

        auto now = q::Clock::now();

        for (size_t i = 0; i < piece_count; i++)
        {
            auto piece = m_tx.acquire_datagram(sizeof(Header) + 512);

            auto& params = m_send_params[channel_idx];
            piece->params = params;
            piece->added = now;

            QASSERT(left > 0);
            auto f_size = math::min(piece_size, left);

            size_t h_size = 0;
            if (i == 0)
            {
                h_size = sizeof(Piece_1st_Header);
                auto& f_header = get_header<Piece_1st_Header>(piece->data);
                f_header.piece_size = piece_size;
                f_header.total_data_size = size;
                f_header.piece_count = piece_count;
            }
            else
            {
                h_size = sizeof(Piece_Header);
                auto& f_header = get_header<Piece_Header>(piece->data);
            }
            QASSERT(h_size > 0);

            piece->data.resize(h_size + f_size);
            std::copy(data, data + f_size, piece->data.begin() + h_size);

            auto& header = get_header<Header>(piece->data);
            header.id = ++m_last_id;
            header.piece_idx = i;
            header.flag_is_reliable = params.is_reliable;
            header.type = (i == 0) ? TYPE_1ST_PIECE : TYPE_PIECE;
            header.channel_idx = channel_idx;

            auto crc = q::util::compute_murmur_hash32(piece->data.data(), piece->data.size());
            header.crc = crc;

            data += f_size;
            left -= f_size;

            add_and_send_datagram(piece);
        }
        send_datagram();
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



        send_datagram();

    }

}
