#pragma once

#include "common/bus/IUART.h"

namespace silk
{
namespace hal
{
struct UART_Linux_Descriptor;
}
}

namespace silk
{
namespace bus
{

class UART_Linux : public IUART
{
public:
    UART_Linux();
    ~UART_Linux();

    bool init(hal::IBus_Descriptor const& descriptor) override;
    std::shared_ptr<const hal::IBus_Descriptor> get_descriptor() const override;

    void lock();
    auto try_lock() -> bool;
    void unlock();

    auto read(uint8_t* data, size_t max_size) -> size_t;
    auto write(uint8_t const* data, size_t size) -> bool;

    void send_break();

private:
    bool init(std::string const& dev, int baud_id);
    void close();

    std::shared_ptr<hal::UART_Linux_Descriptor> m_descriptor;
    std::string m_dev;

    int m_fd = -1;
    std::recursive_mutex m_mutex;
    std::vector<uint8_t> m_buffer;
};

}
}
