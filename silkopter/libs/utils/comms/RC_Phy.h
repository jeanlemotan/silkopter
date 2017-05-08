#pragma once

#include <stdint.h>
#include <vector>
#include <memory>
#include <mutex>
#include <atomic>
#include <boost/thread.hpp>
#include "ISocket.h"
#include "utils/Clock.h"

namespace util
{
namespace comms
{

class RC_Phy
{
public:
    RC_Phy(bool master);
    ~RC_Phy();

    bool set_center_frequency(float center_frequency); //MHz
    void set_xtal_adjustment(float adjustment); //-100.f, 100.f

    struct RX_Data
    {
        size_t index = 0;
        size_t rate = 0;
        Clock::time_point tx_timepoint;
        Clock::time_point rx_timepoint;
        int8_t tx_dBm = 0;
        int8_t rx_dBm = 0;
        std::vector<uint8_t> payload;
    };

    typedef std::function<size_t(uint8_t* data)> TX_Callback;
    typedef std::function<void(RX_Data const& data)> RX_Callback;

    bool init(std::string const& device, uint32_t speed, uint8_t sdn_gpio, uint8_t nirq_gpio);
    void set_callbacks(TX_Callback txcb, RX_Callback rxcb);

    size_t get_mtu() const;

    void set_rate(size_t rate);

private:
    struct HW;
    std::unique_ptr<HW> m_hw;
    TX_Callback m_tx_callback;
    RX_Callback m_rx_callback;

#pragma pack(push, 1)

    struct Header
    {
        int8_t dBm = 0;
        uint16_t crc = 0;
    };

#pragma pack(pop)


    bool m_is_master = false;

    size_t m_desired_rate = 0;
    Clock::duration m_desired_duration;

    boost::thread m_thread;

    std::atomic_bool m_exit = { false };

    RX_Data m_rx_data;
    std::vector<uint8_t> m_tx_buffer;

    void master_thread_proc();
    void slave_thread_proc();

    bool read_fifo(size_t rx_size);
};


}
}
