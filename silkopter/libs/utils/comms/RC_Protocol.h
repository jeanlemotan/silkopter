#pragma once

#include <deque>
#include <vector>
#include <atomic>
#include "RC_Phy.h"
#include "utils/Clock.h"


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
        std::vector<uint8_t> payload;
    };

    typedef std::function<void(RX_Packet const&)> RX_Callback;

    RC_Protocol(RC_Phy& phy, RX_Callback rx_callback);

    bool init();

    size_t get_mtu() const;

    typedef std::function<size_t(uint8_t* data, uint8_t& packet_type)> TX_Callback;
    void add_periodic_packet(Clock::duration period, TX_Callback tx_callback);

    void send_packet(uint8_t packet_type, uint8_t const* data, size_t size);

    void process();

public:
    size_t compute_tx_data(uint8_t* data);
    void process_rx_data(util::comms::RC_Phy::RX_Data const& data);

    RC_Phy& m_phy;

    struct Periodic_Packet
    {
        Clock::time_point last_tp = Clock::now();
        Clock::duration period;
        TX_Callback tx_callback;
    };
    std::mutex m_periodic_packets_mutex;
    std::vector<Periodic_Packet> m_periodic_packets;

    struct TX_Packet
    {
        uint8_t packet_type = 0;
        size_t offset = 0;
        std::vector<uint8_t> payload;
    };

    std::mutex m_tx_packet_queue_mutex;
    std::deque<TX_Packet> m_tx_packet_queue;

    std::atomic_uint m_crt_sent_packet_index = { 1 };
    std::atomic_uint m_received_packet_index = { 1 };

#pragma pack(push, 1)

    struct Header
    {
        uint16_t packet_type : 6;
        uint16_t packet_index : 5;
        uint16_t last_received_packet_index : 5;

        static const size_t MAX_PACKET_TYPE = 59;
        static const size_t MAX_PACKET_INDEX = 32;
        static const size_t MAX_PACKER_INDEX_MASK = MAX_PACKET_INDEX - 1;

        enum Internal_Packet_Type : uint8_t
        {
            EMPTY_PACKET = MAX_PACKET_TYPE + 1,
            RESET_PACKET,
            MID_PACKET,
            LAST_PACKET,
        };
    };

#pragma pack(pop)

    RX_Callback m_rx_callback;

    std::mutex m_incoming_packet_mutex;
    RX_Packet m_incoming_packet;
};

}
}
