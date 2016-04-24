#pragma once

#include "common/bus/IUART.h"

namespace silk
{
namespace bus
{

class UART_BBang : public IUART
{
public:
    UART_BBang(ts::IDeclaration_Scope const& scope);
    ~UART_BBang();

    bool init(std::shared_ptr<ts::IValue> descriptor) override;
    std::shared_ptr<const ts::IValue> get_descriptor() const override;

    void lock();
    auto try_lock() -> bool;
    void unlock();

    auto read(uint8_t* data, size_t max_size) -> size_t;
    auto write(uint8_t const* data, size_t size) -> bool;

    void send_break();

private:
    bool init(size_t rx_pin, size_t baud, bool invert);
    void close();

    std::shared_ptr<ts::IValue> m_descriptor;

    std::recursive_mutex m_mutex;

    bool m_is_initialized = false;
};

}
}
