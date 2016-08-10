#pragma once

#include "RCP.h"
#include <vector>
#include <string>
#include <chrono>
#include <boost/thread.hpp>

namespace util
{

class RCP_RF4463F30_Socket : public RCP_Socket
{
public:
    RCP_RF4463F30_Socket(std::string const& device, uint32_t speed, bool master);
    ~RCP_RF4463F30_Socket();

    Result process() override;

    bool start();

    void async_send(void const* data, size_t size) override;

    size_t get_mtu() const override;

    bool lock() override;
    void unlock() override;

private:
    std::atomic_bool m_is_locked = {false};

    void master_thread_proc();
    void slave_thread_proc();

    struct Impl;
    std::unique_ptr<Impl> m_impl;
    std::string m_device;
    uint32_t m_speed = 0;
    bool m_exit = false;
    bool m_is_master = false;
    boost::thread m_thread;
    q::Clock::time_point m_last_tx_tp = q::Clock::now();
    q::Clock::time_point m_last_rx_tp = q::Clock::now();
    uint8_t m_id = 0;

    bool m_is_initialized = false;
};

}
