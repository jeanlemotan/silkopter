#pragma once

#include "IUART.h"
#include <string>
#include <vector>
#include <atomic>

namespace util
{
namespace hw
{

class UART_BB : public IUART
{
public:
    UART_BB();
    ~UART_BB();

    ts::Result<void> init(uint32_t rx_gpio, uint32_t baud, bool inverted_signalling);

    size_t read(uint8_t* data, size_t max_size) override;
    bool write(uint8_t const* data, size_t size) override;

    void send_break() override;

private:
    void close();

    uint32_t m_rx_gpio = 0;

    mutable std::atomic_bool m_is_used = { false };

    bool m_is_initialized = true;
};

}
}
