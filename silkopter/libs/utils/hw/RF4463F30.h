#pragma once

#include <chrono>
#include <string>
#include <vector>
#include <mutex>
#include "Si4463.h"
#include "utils/Clock.h"

namespace util
{
namespace hw
{

class RF4463F30
{
public:
    RF4463F30();
    ~RF4463F30();

    bool init(hw::ISPI& spi, uint8_t sdn_gpio, uint8_t nirq_gpio);

    bool set_channel(uint8_t channel);
    uint8_t get_channel() const;
    void set_xtal_adjustment(float adjustment); //-1 - 1. Default is 0;

    enum class FIFO_Mode
    {
        SPLIT,
        HALF_DUPLEX
    };

    bool set_fifo_mode(FIFO_Mode mode);

    uint8_t* get_tx_fifo_payload_ptr(size_t fifo_size);

    size_t get_rx_fifo_payload_size() const;
    uint8_t* get_rx_fifo_payload_ptr();

    bool tx(Clock::duration timeout);

    enum class RX_Result
    {
        OK,
        TIMEOUT,
        CRC_FAILED,
        RX_FAILED,
        FIFO_FAILED
    };

    RX_Result rx(size_t max_expected_size, Clock::duration packet_timeout, Clock::duration payload_timeout);
    RX_Result resume_rx(Clock::duration packet_timeout, Clock::duration payload_timeout);

    int8_t get_input_dBm();

private:
    bool init();
    void shutdown();
    void reset();

    size_t get_fifo_capacity() const;

    bool m_is_initialized = false;
    bool m_tx_started = false;
    uint8_t m_channel = 0;
    Si4463 m_chip;

    FIFO_Mode m_fifo_mode = FIFO_Mode::SPLIT;
    std::vector<uint8_t> m_tx_fifo;
    uint8_t m_tx_fifo_threshold = 0;

    std::vector<uint8_t> m_rx_fifo;
    uint8_t m_rx_fifo_threshold = 0;

    mutable std::recursive_mutex m_mutex;
};

}
}
