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

    struct Master_Descriptor
    {
    };
    Video_Streamer(std::string const& interface, Master_Descriptor const& descriptor);


    struct Slave_Descriptor
    {
        q::Clock::duration max_latency = std::chrono::milliseconds(500);
        q::Clock::duration reset_duration = std::chrono::milliseconds(1000);
    };
    Video_Streamer(std::string const& interface, Slave_Descriptor const& descriptor);

    ~Video_Streamer();

    void process();

    bool init(uint32_t coding_k, uint32_t coding_n);

    void send(void const* data, size_t size, math::vec2u16 const& resolution);

    std::function<void(void const* data, size_t size, math::vec2u16 const& resolution)> on_data_received;

    size_t get_mtu() const;

    static std::vector<std::string> enumerate_interfaces();

private:

    bool prepare_filter();
    void prepare_radiotap_header(size_t rate_hz);
    void prepare_tx_packet_header(uint8_t* buffer);
    bool process_rx_packet();

    void master_thread_proc();
    void slave_thread_proc();

    std::string m_interface;
    bool m_is_master = false;
    Master_Descriptor m_master_descriptor;
    Slave_Descriptor m_slave_descriptor;

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
