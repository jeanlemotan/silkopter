#pragma once

#include <vector>
#include <string>
#include <boost/thread.hpp>

struct fec_t;

namespace util
{
namespace comms
{

class Video_Streamer
{
public:

    struct TX_Descriptor
    {
    };
    Video_Streamer(std::string const& interface, TX_Descriptor const& descriptor);


    struct RX_Descriptor
    {
        q::Clock::duration max_latency = std::chrono::milliseconds(500);
        q::Clock::duration reset_duration = std::chrono::milliseconds(1000);
    };
    Video_Streamer(std::vector<std::string> const& interfaces, RX_Descriptor const& descriptor);

    ~Video_Streamer();

    void process();

    bool init(uint32_t coding_k, uint32_t coding_n);

    void send(void const* data, size_t size, math::vec2u16 const& resolution);

    std::function<void(void const* data, size_t size, math::vec2u16 const& resolution)> on_data_received;

    size_t get_mtu() const;

    static std::vector<std::string> enumerate_interfaces();

    struct PCap;
    struct RX;
    struct TX;

private:

    bool prepare_pcap(std::string const& interface, PCap& pcap);

    bool prepare_filter(PCap& pcap);
    void prepare_radiotap_header(size_t rate_hz);
    void prepare_tx_packet_header(uint8_t* buffer);
    bool process_rx_packet(PCap& pcap);

    void tx_thread_proc();
    void rx_thread_proc();

    std::string m_tx_interface;
    std::vector<std::string> m_rx_interfaces;

    bool m_is_tx = false;

    TX_Descriptor m_tx_descriptor;
    RX_Descriptor m_rx_descriptor;

    uint32_t m_coding_k = 1;
    uint32_t m_coding_n = 2;

    struct Impl;
    std::unique_ptr<Impl> m_impl;
    bool m_exit = false;
    boost::thread m_thread;

    fec_t* m_fec = nullptr;
    std::array<uint8_t const*, 16> m_fec_src_datagram_ptrs;
    std::array<uint8_t*, 32> m_fec_dst_datagram_ptrs;

    size_t m_transport_datagram_size = 0;
    size_t m_streaming_datagram_size = 0;
    size_t m_payload_size = 0;

    size_t m_datagram_header_offset = 0;
    size_t m_payload_offset = 0;
};


}
}
