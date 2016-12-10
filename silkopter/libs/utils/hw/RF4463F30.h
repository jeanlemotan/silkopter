#pragma once

#include <chrono>
#include <string>
#include "Si4463.h"

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

    bool write_tx_fifo(void const* data, uint8_t size);

    bool begin_tx(size_t size, uint8_t channel);
    bool end_tx();

    bool tx(size_t size, uint8_t channel);

    bool begin_rx(uint8_t channel);
    bool end_rx(size_t& size, std::chrono::high_resolution_clock::duration timeout);

    bool rx(size_t& size, uint8_t channel, std::chrono::high_resolution_clock::duration timeout);

    bool read_rx_fifo(void* data, size_t& size);

    int8_t get_input_dBm();

private:
    bool init();
    void shutdown();
    void reset();

    bool m_is_initialized = false;
    bool m_tx_started = false;
    Si4463 m_chip;
};

}
}
