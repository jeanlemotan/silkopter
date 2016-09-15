#pragma once

#include <vector>
#include <string>
#include <boost/thread.hpp>

namespace util
{
namespace comms
{

class Video_Streamer
{
public:

    struct Master_Descriptor
    {
        uint32_t coding_k;
        uint32_t coding_n;
    };
    Video_Streamer(std::string const& interface, Master_Descriptor const& descriptor);


    struct Slave_Descriptor
    {
        uint32_t coding_k = 1;
        uint32_t coding_n = 2;
        q::Clock::duration max_latency = std::chrono::milliseconds(100);
        q::Clock::duration reset_duration = std::chrono::milliseconds(1000);
    };
    Video_Streamer(std::string const& interface, Slave_Descriptor const& descriptor);

    ~Video_Streamer();

    void process();

    bool init();

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

    struct Impl;
    std::unique_ptr<Impl> m_impl;
    bool m_exit = false;
    boost::thread m_thread;
};


}
}
