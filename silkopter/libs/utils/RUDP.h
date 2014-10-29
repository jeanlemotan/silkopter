#pragma once

#include <boost/optional.hpp>

namespace util
{

    class RUDP : q::util::Noncopyable
    {
    public:
        typedef boost::asio::ip::udp::socket Socket_t;

        //RUDP(Socket_t& socket) : m_socket(socket) {}
        RUDP()
        {
            auto hsz =sizeof(Header);
            hsz =sizeof(Fragment_1st_Header);
            hsz =sizeof(Fragment_Header);
            hsz =sizeof(Fragment_Cancel_Header);
            hsz =sizeof(Confirmation_Header);
            hsz =sizeof(Ping_Header);
            hsz == hsz;
        }

        void start()
        {
//            m_socket.async_read(boost::asio::buffer(m_rx_temp_buffer.data(), m_rx_temp_buffer.size()),
//                                    boost::bind(&RUDP::handle_receive, this,
//                                    boost::asio::placeholders::error,
//                                    boost::asio::placeholders::bytes_transferred));
        }

        struct Send_Params
        {
            uint8_t channel_idx = 0;
            int32_t priority = 0; //Higher means higher priority. Can be negative
            bool is_reliable = true;
            q::Clock::duration bump_priority_after = q::Clock::duration{0}; //zero means never
            q::Clock::duration cancel_after = q::Clock::duration{0}; //zero means never
        };

        void send(Send_Params const& params, uint8_t const* data, size_t size);


        struct Receive_Params
        {
            uint8_t channel_idx = 0;
            q::Clock::duration max_receive_time = q::Clock::duration{0}; //how long to wait for late packets
        };

        bool receive(Receive_Params const& params, std::vector<uint8_t>& data);

    private:

#pragma pack(push, 1)
        struct Header
        {
            uint32_t id : 24;
            uint32_t fragment_idx : 8;
            uint8_t flag_is_reliable : 1;
            uint8_t flag_is_1st_fragment : 1;
            uint8_t flag_is_fragment : 1;
            uint8_t flag_is_cancel_fragments : 1;
            uint8_t flag_ping : 1;
            uint8_t flag_pong : 1;
            uint8_t channel_idx;
            uint16_t header_crc;
        };

        struct Fragment_1st_Header : Header
        {
            uint16_t fragment_size;
            uint16_t fragment_crc;
            uint32_t total_data_crc;
            uint32_t total_data_size : 24;
            uint32_t fragment_count : 8;
        };

        struct Fragment_Header : Header
        {
            uint16_t fragment_crc;
        };

        struct Fragment_Cancel_Header : Header
        {
        };

        struct Confirmation_Header : Header
        {
        };

        struct Ping_Header : Header
        {
            uint32_t time_point; //ms
        };
#pragma pack(pop)

//        Socket_t& m_socket;

        struct TX
        {
            typedef std::vector<uint8_t> Buffer_t;

            struct Fragment
            {
                Send_Params params;
                q::Clock::time_point added_time_point;
                q::Clock::time_point sent_time_point;
                Buffer_t data;
            };
            typedef std::shared_ptr<Fragment> Fragment_ptr;

            std::vector<Fragment_ptr> fragment_pool;
            std::mutex fragment_pool_mutex;

            struct Queue
            {
                int8_t priority = 0;
                std::deque<Fragment_ptr> fragments;
                std::mutex mutex;
            };

            //waiting to be sent
            std::vector<Queue> to_send;
            std::mutex to_send_mutex;

            //waiting for confirmation
            std::deque<Fragment_ptr> waiting;
            std::mutex waiting_mutex;
        } m_tx;

        struct
        {
            typedef std::vector<uint8_t> Buffer_t;

            Buffer_t buffer;
            std::mutex buffer_mutex;
            std::array<uint8_t, 512> temp_buffer;
        } m_rx;

        //sent pings
        std::deque<Ping_Header> m_pings;
        std::mutex m_pings_mutex;


        struct Stats
        {
            size_t packets_sent = 0;
            size_t packets_received = 0;
            size_t packets_resent = 0;
            size_t confirmations_sent = 0;
            size_t confirmations_received = 0;
            size_t bytes_sent = 0;
            size_t bytes_received = 0;
            size_t payload_bytes_sent = 0;
            size_t payload_bytes_received = 0;
        };

        size_t m_pings_sent;
        size_t m_pongs_received;

        std::array<uint32_t, 256> m_channel_ids = {0};
        std::array<Stats, 256> m_channel_stats;
        Stats m_global_stats;

        TX::Fragment_ptr get_new_fragment()
        {
            std::lock_guard<std::mutex> lg(m_tx.fragment_pool_mutex);
            TX::Fragment_ptr fragment;
            if (!m_tx.fragment_pool.empty())
            {
                fragment = m_tx.fragment_pool.back();
                m_tx.fragment_pool.pop_back();
            }
            else
            {
                fragment = std::make_shared<TX::Fragment>();
            }
            auto& data = fragment->data;
            data.reserve(32);
            data.resize(0); //this wioll force zero-ing the data
            data.resize(sizeof(Header));
            return fragment;
        }

        void handle_send(std::shared_ptr<TX::Buffer_t> buffer_ptr,
                         const boost::system::error_code& /*error*/,
                         std::size_t /*bytes_transferred*/)
        {
//            std::lock_guard<std::mutex> lg(m_tx_buffer_pool_mutex);
//            m_tx_buffer_pool.push_back(buffer_ptr);
        }

        void handle_receive(const boost::system::error_code& error, std::size_t bytes_transferred)
        {
//            if (error)
//            {
//                m_socket.close();
//            }
//            else
//            {
//                if (bytes_transferred > 0)
//                {
//                    std::lock_guard<std::mutex> lg(m_rx_mutex);
//                    auto off = m_rx_buffer.size();
//                    m_rx_buffer.resize(off + bytes_transferred);
//                    std::copy(m_rx_temp_buffer.begin(), m_rx_temp_buffer.begin() + bytes_transferred, m_rx_buffer.begin() + off);
//                }
//                start();
//            }
        }

        void send_to_socket(std::shared_ptr<std::vector<uint8_t>> buffer, size_t size)
        {
//            if (!buffer->empty())
//            {
//                m_socket.async_write_some(boost::asio::buffer(*buffer, size),
//                    boost::bind(&This_t::handle_send, this,
//                    buffer,
//                    boost::asio::placeholders::error,
//                    boost::asio::placeholders::bytes_transferred));
//            }
        }

        //////////////////////////////////////////////////////////////////////////

//        std::shared_ptr<TX_Buffer_t> get_tx_buffer()
//        {
//            std::lock_guard<std::mutex> lg(m_tx_buffer_pool_mutex);
//            std::shared_ptr<TX_Buffer_t> buffer_ptr;
//            if (!m_tx_buffer_pool.empty())
//            {
//                buffer_ptr = m_tx_buffer_pool.back();
//                m_tx_buffer_pool.pop_back();
//            }
//            else
//            {
//                buffer_ptr = std::make_shared<TX_Buffer_t>();
//            }
//            buffer_ptr->reserve(32);
//            if (buffer_ptr->size() < HEADER_SIZE)
//            {
//                buffer_ptr->resize(HEADER_SIZE);
//            }
//            return buffer_ptr;
//        }

   };

}
