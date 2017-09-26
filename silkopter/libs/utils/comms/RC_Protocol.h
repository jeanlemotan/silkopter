#pragma once

#include <deque>
#include <vector>
#include <atomic>
#include "esp8266/Phy.h"
#include "utils/Clock.h"
#include "utils/Queue.h"
#include "utils/Pool.h"

namespace util
{
namespace comms
{

class RC_Protocol
{
public:

    struct RX_Packet
    {
        uint8_t packet_type;
        Clock::time_point rx_timepoint;
        int8_t tx_dBm = 0;
        int8_t rx_dBm = 0;
    };

    typedef std::function<void(RX_Packet const& packet, uint8_t* data, size_t size)> RX_Callback;

    RC_Protocol(Phy& phy, RX_Callback rx_callback);
    ~RC_Protocol();

    bool init();

    typedef std::vector<uint8_t> Buffer;

    typedef std::function<bool(Buffer& buffer, uint8_t& packet_type)> TX_Callback;
    void add_periodic_packet(Clock::duration period, TX_Callback tx_callback);

    void send_reliable_packet(uint8_t packet_type, void const* data, size_t size);

    void reset_session();

    void process();

public:
    void reset_session_data();

    bool compute_tx_data();
    void process_rx_data();

    Phy& m_phy;

    struct Periodic_Packet
    {
        Clock::time_point last_tp = Clock::now();
        Clock::duration period;
        TX_Callback tx_callback;
    };
    std::mutex m_periodic_packets_mutex;
    std::vector<Periodic_Packet> m_periodic_packets;

    bool m_send_reset_request = false;
    bool m_send_reset_reply = false;
    uint32_t m_session_id = 0;

    struct Reliable_Packet
    {
        uint8_t packet_type = 0;
        size_t offset = 0;
        std::vector<uint8_t> payload;
    };
    std::mutex m_reliable_packet_queue_mutex;
    std::deque<Reliable_Packet> m_reliable_packet_queue;

    std::atomic_uint m_crt_sent_reliable_packet_index = { 1 };
    std::atomic_uint m_received_reliable_packet_index = { 1 };


    std::atomic_bool m_empty_packet_needed = { false };

    RX_Callback m_rx_callback;

    struct RX_Reliable
    {
        std::vector<uint8_t> payload;
    } m_rx_reliable;

    struct RX_Periodic
    {
        std::vector<uint8_t> payload;
    } m_rx_periodic;


#pragma pack(push, 1)

    struct Header
    {
        uint16_t header_type : 2;
        uint16_t packet_type : 6;
        uint16_t last_received_reliable_packet_index : 7;
        uint16_t crc = 0;

        enum Header_Type : uint8_t
        {
            RESET,
            RELIABLE,
            PERIODIC
        };

        static const size_t MAX_PACKET_TYPE = 60;
        enum Packet_Type : uint8_t
        {
            EMPTY_PACKET = MAX_PACKET_TYPE + 1,
            RESET_REQ_PACKET,
            RESET_RES_PACKET
        };
    };

    struct Reset_Header : public Header
    {
        uint32_t session_id = 0;
    };

    struct Periodic_Header : public Header
    {
    };

    struct Reliable_Header : public Header
    {
        uint8_t last_packet : 1;
        uint8_t packet_index : 7;

        static const size_t MAX_PACKET_INDEX = 126;
        static const size_t INVALID_PACKET_INDEX = 127;
    };

#pragma pack(pop)
};

}
}
