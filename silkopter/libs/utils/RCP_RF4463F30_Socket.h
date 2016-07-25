#pragma once

#include "RCP.h"
#include <vector>
#include <string>
#include <boost/thread.hpp>

namespace util
{

class RCP_RF4463F30_Socket : public RCP_Socket
{
public:
    RCP_RF4463F30_Socket(std::string const& device, uint32_t speed);
    ~RCP_RF4463F30_Socket();

    Result process() override;

    bool start();

    void async_send(void const* data, size_t size) override;

    size_t get_mtu() const override;

    bool lock() override;
    void unlock() override;

private:
    bool initialize();

    std::atomic_bool m_send_in_progress = {false};

    struct Impl;
    std::unique_ptr<Impl> m_impl;
    std::string m_device;
    uint32_t m_speed = 0;
    bool m_exit = false;
    boost::thread m_rx_thread;
    boost::thread m_tx_thread;
    uint8_t m_id = 0;

    bool m_is_initialized = false;
};

}
