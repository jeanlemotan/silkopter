#pragma once

#include "common/bus/IUART.h"

namespace silk
{
namespace bus
{

class UART_Linux : public IUART
{
public:
    UART_Linux(ts::Type_System const& ts);
    ~UART_Linux();

    bool init(std::shared_ptr<ts::IValue> descriptor) override;
    std::shared_ptr<const ts::IValue> get_descriptor() const override;

    void lock();
    auto try_lock() -> bool;
    void unlock();

    auto read(uint8_t* data, size_t max_size) -> size_t;
    auto write(uint8_t const* data, size_t size) -> bool;

    void send_break();

private:
    bool init(std::string const& dev, uint32_t baud);
    void close();

    std::shared_ptr<ts::IValue> m_descriptor;
    std::string m_dev;

    int m_fd = -1;
    std::recursive_mutex m_mutex;
    std::vector<uint8_t> m_buffer;
};

}
}
