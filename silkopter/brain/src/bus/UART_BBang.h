#pragma once

#include "common/bus/IUART.h"

namespace silk
{
namespace uav
{
struct UART_BBang_Descriptor;
}
}

namespace silk
{
namespace bus
{

class UART_BBang : public IUART
{
public:
    UART_BBang();
    ~UART_BBang();

    bool init(uav::IBus_Descriptor const& descriptor) override;
    std::shared_ptr<const uav::IBus_Descriptor> get_descriptor() const override;

    void lock();
    auto try_lock() -> bool;
    void unlock();

    auto read(uint8_t* data, size_t max_size) -> size_t;
    auto write(uint8_t const* data, size_t size) -> bool;

    void send_break();

private:
    bool init(uint32_t rx_pin, uint32_t baud, bool invert);
    void close();

    std::shared_ptr<uav::UART_BBang_Descriptor> m_descriptor;

    std::recursive_mutex m_mutex;

    bool m_is_initialized = false;
};

}
}
