#pragma once

#include "common/bus/IUART.h"
#include "utils/hw/UART_Dev.h"

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

class UART_Linux_Bus : public IUART_Bus
{
public:
    UART_Linux_Bus();
    ~UART_Linux_Bus();

    ts::Result<void> init(hal::IBus_Descriptor const& descriptor) override;
    std::shared_ptr<const hal::IBus_Descriptor> get_descriptor() const override;

    util::hw::IUART& get_uart() override;

private:
    std::shared_ptr<hal::UART_Linux_Descriptor> m_descriptor;
    util::hw::UART_Dev m_uart;
};

}
}
