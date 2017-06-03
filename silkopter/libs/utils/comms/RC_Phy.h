#pragma once

#include <stdint.h>
#include <vector>
#include <memory>
#include <mutex>
#include <atomic>
#include <boost/thread.hpp>
#include "utils/Clock.h"
#include "utils/hw/ISPI.h"
#include "utils/Queue.h"

namespace util
{
namespace comms
{

class RC_Phy
{
public:
    RC_Phy(bool master);
    ~RC_Phy();

    bool set_channel(uint8_t channel);
    void set_xtal_adjustment(float adjustment); //-1.f, 1.f

    //how many times per second to listen
    void set_master_listen_rate(size_t rate);

    struct RX_Data
    {
        size_t index = 0;
        size_t rate = 0;
        Clock::time_point tx_timepoint;
        Clock::time_point rx_timepoint;
        int8_t tx_dBm = 0;
        int8_t rx_dBm = 0;
    };

    typedef Queue<std::vector<uint8_t>>::Buffer Buffer;

    typedef std::function<bool(Buffer& buffer, bool& more_data)> TX_Callback;
    typedef std::function<void(RX_Data const& data, Buffer& buffer)> RX_Callback;

    bool init(hw::ISPI& spi, uint8_t sdn_gpio, uint8_t nirq_gpio);
    void set_callbacks(TX_Callback txcb, RX_Callback rxcb);

    size_t get_mtu() const;

private:
    struct HW;
    std::unique_ptr<HW> m_hw;
    TX_Callback m_tx_callback;
    RX_Callback m_rx_callback;

#pragma pack(push, 1)

    struct Master_Header
    {
        uint8_t more_data : 1;
        uint8_t dBm : 7; //negated, clamped to 0 - 127
    };
    struct Slave_Header
    {
        uint8_t dBm : 7; //negated, clamped to 0 - 127
    };

#pragma pack(pop)


    bool m_is_master = false;

    //size_t m_desired_rate = 0;
    //Clock::duration m_desired_duration;

    boost::thread m_hw_thread;
    boost::thread m_pk_tx_thread;
    boost::thread m_pk_rx_thread;

    std::atomic_bool m_exit = { false };
    std::atomic_int_fast8_t m_last_dBm = { 0 };

    std::mutex m_rx_data_mutex;
    RX_Data m_rx_data;

    Queue<std::vector<uint8_t>> m_tx_queue;
    Queue<std::vector<uint8_t>> m_rx_queue;

    Clock::duration m_master_listen_period = Clock::duration::zero();

    void master_thread_proc();
    void slave_thread_proc();
    void packet_tx_thread_proc();
    void packet_rx_thread_proc();

    bool read_fifo();
};


}
}
