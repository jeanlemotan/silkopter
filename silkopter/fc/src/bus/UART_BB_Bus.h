#pragma once

#include "common/bus/IUART.h"
#include "utils/hw/UART_BB.h"

namespace silk
{
namespace hal
{
struct UART_BB_Descriptor;
}
}

namespace silk
{
namespace bus
{

class UART_BB_Bus : public IUART_Bus
{
public:
    UART_BB_Bus();
    ~UART_BB_Bus();

    ts::Result<void> init(hal::IBus_Descriptor const& descriptor) override;
    std::shared_ptr<const hal::IBus_Descriptor> get_descriptor() const override;

    util::hw::IUART& get_uart() override;

private:
    std::shared_ptr<hal::UART_BB_Descriptor> m_descriptor;
    util::hw::UART_BB m_uart;
};

}
}
