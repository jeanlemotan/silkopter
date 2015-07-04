#pragma once

#include "RCP.h"

namespace util
{

class RCP_RFMON_Socket : public RCP_Socket
{
public:
    RCP_RFMON_Socket(std::string const& interface, uint8_t id);
    ~RCP_RFMON_Socket();

    auto process() -> Result;

    auto start() -> bool;

    void async_send(uint8_t const* data, size_t size);

private:

    auto prepare_filter() -> bool;
    void prepare_radiotap_header(size_t rate_hz);
    void prepare_tx_packet_header(uint8_t* buffer);
    auto process_rx_packet() -> bool;


    struct Impl;
    std::unique_ptr<Impl> m_impl;
    std::string m_interface;
    uint8_t m_id = 0;
    bool m_exit = false;
    std::thread m_rx_thread;
    std::thread m_tx_thread;
};

}
