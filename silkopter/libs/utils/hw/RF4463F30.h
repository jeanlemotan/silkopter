#pragma once

#include <chrono>
#include <string>
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

    bool init(std::string const& device, uint32_t speed, uint8_t sdn_gpio, uint8_t nirq_gpio);

    bool set_channel(uint8_t channel);
    uint8_t get_channel() const;
    void set_xtal_adjustment(float adjustment); //-1 - 1. Default is 0;

    bool write_tx_fifo(void const* data, uint8_t size);

    bool begin_tx(size_t size);
    bool end_tx();

    bool tx(size_t size);

    bool begin_rx();
    bool end_rx(size_t& size, Clock::duration timeout);

    bool rx(size_t& size, Clock::duration timeout);

    bool read_rx_fifo(void* data, size_t& size);

    int8_t get_input_dBm();

private:
    bool init();
    void shutdown();
    void reset();

    bool m_is_initialized = false;
    bool m_tx_started = false;
    uint8_t m_channel = 0;
    Si4463 m_chip;
};

}
}
