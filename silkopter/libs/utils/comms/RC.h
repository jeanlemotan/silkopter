#pragma once

#include <stdint.h>
#include <vector>
#include <memory>
#include <mutex>
#include <atomic>
#include <boost/thread.hpp>
#include "ISocket.h"

namespace util
{
namespace comms
{

class RC
{
public:
    RC(bool master);
    ~RC();

    bool init(std::string const& device, uint32_t speed, uint8_t sdn_gpio, uint8_t nirq_gpio);

    size_t get_mtu() const;

    void set_rate(size_t rate);

    void set_tx_data(void const* data, size_t size);

    struct Data
    {
        size_t rate = 0;
        q::Clock::time_point tx_timepoint;
        q::Clock::time_point rx_timepoint;
        int8_t tx_dBm = 0;
        int8_t rx_dBm = 0;
        std::vector<uint8_t> rx_data;
    };

    void get_rx_data(Data& data) const;

private:
    struct HW;
    std::unique_ptr<HW> m_hw;


#pragma pack(push, 1)

    struct Header
    {
        int8_t dBm = 0;
        uint16_t crc = 0;
    };

#pragma pack(pop)


    bool m_is_master = false;

    size_t m_desired_rate = 0;
    q::Clock::duration m_desired_duration;

    boost::thread m_thread;

    std::atomic_bool m_exit = { false };

    mutable std::mutex m_mutex;
    std::vector<uint8_t> m_tx_buffer;
    Data m_rx_data;

    std::vector<uint8_t> m_rx_buffer;

    void master_thread_proc();
    void slave_thread_proc();

    bool read_fifo(size_t rx_size);
};


}
}
