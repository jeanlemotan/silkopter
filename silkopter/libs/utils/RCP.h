#pragma once

#include <deque>
#include <zlib.h>
#include <boost/intrusive_ptr.hpp>

namespace util
{
    namespace detail
    {
        struct Pool_Item_Base
        {
            std::function<void(Pool_Item_Base* p)> gc;
            std::atomic_size_t count = {0};
        };

        template<class T> struct Pool
        {
            std::function<void(T&)> release;
            typedef boost::intrusive_ptr<T> Ptr;
            Pool();
            auto acquire() -> Ptr;
        private:
            std::function<void(Pool_Item_Base*)> m_garbage_collector;
            struct Items
            {
                std::vector<std::unique_ptr<T>> items;
                std::mutex mutex;
            };
            std::shared_ptr<Items> m_pool;
        };
    }


    //socket interface
    class RCP_Socket
    {
    public:
        virtual ~RCP_Socket() {}

        enum class Result
        {
            OK,
            ERROR,
            RECONNECTED,
        };

        virtual auto process() -> Result = 0;

        std::function<void(uint8_t const* data, size_t size)> receive_callback;
        std::function<void(Result)> send_callback;

        virtual void async_send(uint8_t const* data, size_t size) = 0;
    };

    //Reliable Channel Protocol
    class RCP : q::util::Noncopyable
    {
    public:
        RCP(RCP_Socket& socket);

        void reconnect();
        auto is_connected() const -> bool;

        struct Send_Params
        {
            size_t mtu = 1420;
            int8_t importance = 0; //Higher means higher priority. Can be negative
            bool is_reliable = true;
            bool is_compressed = true;
            bool cancel_previous_data = false; //if true, new packets cancel old-unsent packets
            uint8_t unreliable_retransmit_count = 0; //for unreliable channels, retransmit data this many times to increase the chances of arrival. Zero means just the main transmission and no retransmit
            q::Clock::duration bump_priority_after = q::Clock::duration{0}; //zero means never
            q::Clock::duration cancel_after = q::Clock::duration{0}; //zero means never
        };
        void set_send_params(uint8_t channel_idx, Send_Params const& params);
        auto get_send_params(uint8_t channel_idx) const -> Send_Params const&;

        struct Receive_Params
        {
            q::Clock::duration max_receive_time = q::Clock::duration{0}; //how long to wait for late packets. Zero meand the global defaults
        };
        void set_receive_params(uint8_t channel_idx, Receive_Params const& params);
        void set_global_receive_params(Receive_Params const& params);

        //sending with default params
        auto send(uint8_t channel_idx, uint8_t const* data, size_t size) -> bool;
        auto try_sending(uint8_t channel_idx, uint8_t const* data, size_t size) -> bool;

        //sending with overriden params
        auto send(uint8_t channel_idx, Send_Params const& params, uint8_t const* data, size_t size) -> bool;
        auto try_sending(uint8_t channel_idx, Send_Params const& params, uint8_t const* data, size_t size) -> bool;

        auto receive(uint8_t channel_idx, std::vector<uint8_t>& data) -> bool;

        void process();

    private:

        auto _send_locked(uint8_t channel_idx, Send_Params const& params, uint8_t const* data, size_t size) -> bool;

        static const uint8_t VERSION = 1;
        const q::Clock::duration RECONNECT_BEACON_TIMEOUT = std::chrono::milliseconds(500);

        enum Type
        {
            TYPE_PACKET             =   0,
            TYPE_FRAGMENTS_RES      =   1,
            TYPE_PACKETS_RES        =   2,
            TYPE_PING               =   3,
            TYPE_PONG               =   4,
            TYPE_CONNECT_REQ        =   5,
            TYPE_CONNECT_RES        =   6,
        };

        static const size_t MAX_CHANNELS = 32;
        static const size_t MAX_FRAGMENTS = 250;

        const q::Clock::duration PING_TIMEOUT = std::chrono::milliseconds(500);
        static const size_t PING_MIN_AVERAGE_SAMPLES = 4;

#pragma pack(push, 1)
        struct Header
        {
            uint32_t crc;
            uint8_t type : 4;
        };

        struct Packet_Header : public Header
        {
            uint32_t id : 24;
            uint32_t channel_idx : 5;
            uint32_t flag_needs_confirmation : 1;
            uint32_t flag_is_compressed : 1;
            uint8_t fragment_idx;
        };

        struct Packet_Main_Header : public Packet_Header
        {
            uint32_t packet_size : 24;
            uint32_t fragment_count : 8;
        };

        struct Fragments_Res_Header : public Header
        {
            static const size_t MAX_PACKED = 100;
            uint8_t count;
        };

        struct Packets_Res_Header : public Header
        {
            static const size_t MAX_PACKED = 100;
            uint8_t count;
        };

        struct Ping_Header : public Header
        {
            uint16_t seq;
        };

        struct Pong_Header : public Header
        {
            uint16_t seq;
        };

        struct Connect_Req_Header : public Header
        {
            uint8_t version;
        };
        struct Connect_Res_Header : public Header
        {
            uint8_t version;

            enum class Response : uint8_t
            {
                OK = 0,
                VERSION_MISMATCH
            };

            Response response;
        };

#pragma pack(pop)

        RCP_Socket& m_socket;

        typedef std::vector<uint8_t> Buffer_t;

        static const int MAX_PRIORITY = 127;
        static const int MIN_PRIORITY = -127;

        //static const size_t MAX_IN_TRANSIT_DATAGRAMS = 5;

        struct TX
        {
            struct Channel_Data
            {
                std::mutex send_mutex;
                std::vector<uint8_t> compression_buffer;
            };
            std::array<Channel_Data, MAX_CHANNELS> channel_data;

            struct Datagram : public detail::Pool_Item_Base
            {
                Send_Params params;
                q::Clock::time_point added_tp = q::Clock::time_point(q::Clock::duration{0});
                q::Clock::time_point sent_tp = q::Clock::time_point(q::Clock::duration{0});
                std::atomic_bool is_in_transit = {false};
                size_t sent_count = 0; //how many times it was sent - for unreliable only
                Buffer_t data;
            };
            typedef detail::Pool<Datagram>::Ptr Datagram_ptr;

            detail::Pool<Datagram> datagram_pool;
            Datagram_ptr acquire_datagram(size_t data_size);

            //this is a list of fragment and packet responses
            //they are accumulated in a vector and then sent repeatedly for a number of times
            static const uint8_t MAX_FRAGMENT_RES_SEND_COUNT = 8;
            struct Fragment_Res
            {
                uint32_t id = 0;
                uint8_t channel_idx = 0;
                uint8_t fragment_idx = 0;
                uint8_t sent_count = 0;
            };
            std::mutex fragments_res_mutex;
            std::deque<Fragment_Res> fragments_res;

            static const uint8_t MAX_PACKET_RES_SEND_COUNT = 8;
            struct Packet_Res
            {
                uint32_t id = 0;
                uint8_t channel_idx = 0;
                uint8_t sent_count = 0;
            };
            std::mutex packets_res_mutex;
            std::deque<Packet_Res> packets_res;
            //--------------------------------------------


            typedef std::vector<Datagram_ptr> Send_Queue;

            struct Internal_Queues
            {
                std::mutex mutex;
                Datagram_ptr connection_req;
                Datagram_ptr connection_res;
                Datagram_ptr ping;
                Datagram_ptr pong;

                Send_Queue fragments_res;
                Send_Queue packets_res;
            } internal_queues;

            std::mutex packet_queue_mutex;
            Send_Queue packet_queue;

            Datagram_ptr in_transit_datagram;
        } m_tx;

        struct RX
        {
            std::vector<uint8_t> compression_buffer;

            //std::vector<uint8_t> temp_buffer;

            struct Datagram : public detail::Pool_Item_Base
            {
                Buffer_t data;
            };
            typedef detail::Pool<Datagram>::Ptr Datagram_ptr;
            detail::Pool<Datagram> datagram_pool;
            Datagram_ptr acquire_datagram(size_t data_size);

            struct Packet : public detail::Pool_Item_Base
            {
                size_t received_fragment_count = 0;
                q::Clock::time_point added_tp = q::Clock::time_point(q::Clock::duration{0});
                Packet_Main_Header main_header;
                Packet_Header any_header;
                std::array<Datagram_ptr, 256> fragments;
            };
            typedef detail::Pool<Packet>::Ptr Packet_ptr;
            detail::Pool<Packet> packet_pool;
            Packet_ptr acquire_packet();

            struct Packet_Queue
            {
                std::map<uint32_t, Packet_ptr> packets;
                std::mutex mutex;
            };

            //waiting to be received
            std::array<Packet_Queue, MAX_CHANNELS> packet_queues;
            std::array<uint32_t, MAX_CHANNELS> last_packet_ids;
        } m_rx;

        struct Ping
        {
            std::mutex mutex;
            uint16_t last_seq = 0;
            bool is_done = true;

            static constexpr size_t MAX_RTTS = 20;
            std::deque<std::pair<q::Clock::time_point, q::Clock::duration>> rtts;
            q::Clock::duration rtt = q::Clock::duration{0};
        } m_ping;


        struct Stats
        {
            size_t tx_datagrams = 0;
            //size_t tx_cancelled_datagrams = 0;
            size_t tx_confirmed_fragments = 0;
            size_t tx_confirmed_packets = 0;
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

        struct Connection
        {
            std::atomic_bool is_connected = { false };

            mutable std::mutex mutex;
            q::Clock::time_point last_sent_tp = q::Clock::time_point(q::Clock::duration{0});
        } m_connection;

        void disconnect();
        void connect();

        void purge();

        q::Clock::time_point m_init_tp = q::Clock::time_point(q::Clock::duration{0});
        std::array<std::atomic_int, MAX_CHANNELS> m_last_id;
        Stats m_global_stats;

        std::atomic_bool m_is_sending = {false};
        const q::Clock::duration MIN_RESEND_DURATION = std::chrono::milliseconds(5);
        const q::Clock::duration MAX_RESEND_DURATION = std::chrono::milliseconds(60);

        std::array<Send_Params, MAX_CHANNELS> m_send_params;
        std::array<Receive_Params, MAX_CHANNELS> m_receive_params;
        Receive_Params m_global_receive_params;

        int xxx_queued = 0;
        int xxx_on_air = 0;
        int xxx_sent = 0;

        template<class H> static auto get_header(Buffer_t const& data) -> H const&;
        template<class H> static auto get_header(Buffer_t& data) -> H&;
        static auto get_header_size(Buffer_t& data) -> size_t;
        auto compute_crc(uint8_t const* data, size_t size) -> uint32_t;
        //to avoid popping front in vectors
        //Note - I use a vector instead of a deque because the deque is too slow at iterating.
        //The pop_front can be implemented optimally for vectors if order need not be preserved.
        template<class T> static void erase_unordered(std::vector<T>& c, typename std::vector<T>::iterator it);

        void update_stats(Stats& stats, TX::Datagram const& datagram);

        void prepare_to_send_datagram(TX::Datagram& datagram);
        void add_and_send_datagram(TX::Send_Queue& queue, std::mutex& mutex, TX::Datagram_ptr const& datagram);

        auto process_packet_queue() -> TX::Send_Queue::iterator;
        auto get_next_transit_datagram() -> TX::Datagram_ptr;
        void send_datagram();

        void handle_send(RCP_Socket::Result reult);
        void handle_receive(uint8_t const* data, size_t size);

        void send_pending_fragments_res();
        void add_fragment_res(uint8_t channel_idx, uint32_t id, uint8_t fragment_idx);

        void send_pending_packets_res();
        void add_packet_res(uint8_t channel_idx, uint32_t id);

        void send_packet_ping();
        void send_packet_pong(Ping_Header const& ping);
        void process_pings();

        void send_packet_connect_req();
        void send_packet_connect_res(Connect_Res_Header::Response response);
        void process_connection();

        void process_incoming_datagram(RX::Datagram_ptr& datagram);
        void process_packet_datagram(RX::Datagram_ptr& datagram);
        void process_fragments_res_datagram(RX::Datagram_ptr& datagram);
        void process_packets_res_datagram(RX::Datagram_ptr& datagram);
        void process_ping_datagram(RX::Datagram_ptr& datagram);
        void process_pong_datagram(RX::Datagram_ptr& datagram);
        void process_connect_req_datagram(RX::Datagram_ptr& datagram);
        void process_connect_res_datagram(RX::Datagram_ptr& datagram);
   };

}


#include "RCP.inl"

