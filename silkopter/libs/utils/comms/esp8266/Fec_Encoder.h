#pragma once

#include <vector>
#include <string>
#include <atomic>
#include <thread>
#include "Queue.h"

struct fec_t;

class Fec_Encoder
{
public:

    Fec_Encoder();
    ~Fec_Encoder();

    typedef std::chrono::high_resolution_clock Clock;

    static const uint8_t MAX_CODING_K = 16;
    static const uint8_t MAX_CODING_N = 32;
    static const size_t PAYLOAD_OVERHEAD = 6;

    struct Descriptor
    {
        uint8_t coding_k = 12;
        uint8_t coding_n = 20;
        size_t mtu = 1376;
        size_t max_enqueued_packets = 100;
    };

    struct TX_Descriptor : public Descriptor
    {
    };

    struct RX_Descriptor : public Descriptor
    {
        //Clock::duration max_latency = std::chrono::milliseconds(500);
        Clock::duration reset_duration = std::chrono::milliseconds(1000);
    };

    bool init_tx(TX_Descriptor const& descriptor);
    bool init_rx(RX_Descriptor const& descriptor);

    //add the received, encoded packets here
    bool add_rx_packet(void const* data, size_t size, bool block);

    //async, the decoded packets will be ready here
    std::function<void(void const* data, size_t size)> on_rx_data_decoded;

    //add un-encoded packets to be sent here
    bool add_tx_packet(void const* data, size_t size, bool block);

    //async, encoded packets will be ready here
    std::function<void(void const* data, size_t size)> on_tx_data_encoded;

    size_t get_mtu() const;

    struct RX;
    struct TX;

private:

    bool init();

    void tx_thread_proc();
    void rx_thread_proc();

    bool m_is_tx = false;

    TX_Descriptor m_tx_descriptor;
    RX_Descriptor m_rx_descriptor;

    uint8_t m_coding_k = 1;
    uint8_t m_coding_n = 2;

    struct Impl;
    std::unique_ptr<Impl> m_impl;
    bool m_exit = false;
    std::thread m_thread;

    fec_t* m_fec = nullptr;
    std::array<uint8_t const*, MAX_CODING_K> m_fec_src_datagram_ptrs;
    std::array<uint8_t*, MAX_CODING_N> m_fec_dst_datagram_ptrs;

    size_t m_transport_datagram_size = 0;
    size_t m_streaming_datagram_size = 0;
    size_t m_payload_size = 0;

    size_t m_datagram_header_offset = 0;
    size_t m_payload_offset = 0;

    std::atomic_int m_best_input_dBm = { 0 };
    std::atomic_int m_latched_input_dBm = { 0 };

    size_t m_video_stats_rate = 0;
    size_t m_video_stats_data_accumulated = 0;
    Clock::time_point m_video_stats_last_tp = Clock::now();
};

