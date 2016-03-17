#pragma once

#include "RCP.h"
#include <vector>
#include <string>

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

    auto get_mtu() const -> size_t;

    auto lock() -> bool override;
    void unlock() override;

    static std::vector<std::string> enumerate_interfaces();

private:

    auto prepare_filter() -> bool;
    void prepare_radiotap_header(size_t rate_hz);
    void prepare_tx_packet_header(uint8_t* buffer);
    auto process_rx_packet() -> bool;

    std::atomic_bool m_send_in_progress = {false};

    struct Impl;
    std::unique_ptr<Impl> m_impl;
    std::string m_interface;
    bool m_exit = false;
    std::thread m_rx_thread;
    std::thread m_tx_thread;
    uint8_t m_id = 0;
};

}
