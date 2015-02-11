#pragma once

#include "common/node/IUART.h"

namespace silk
{
namespace bus
{

class UART_Linux : public node::IUART
{
public:
    UART_Linux(q::String const& name);
    ~UART_Linux();

    auto get_name() const -> q::String const&;

    auto open(q::String const& device, size_t baud) -> bool;
    void close();

    void lock();
    auto try_lock() -> bool;
    void unlock();

    auto read(uint8_t* data, size_t max_size) -> size_t;
    auto write(uint8_t const* data, size_t size) -> bool;

private:
    q::String m_name;
    q::String m_device;
    int m_fd = -1;
    std::recursive_mutex m_mutex;
    std::vector<uint8_t> m_buffer;
};

DECLARE_CLASS_PTR(UART_Linux);


}
}

