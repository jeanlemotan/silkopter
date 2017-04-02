#include "FCStdAfx.h"
#include "bus/UART_BB_Bus.h"

#include "hal.def.h"

namespace silk
{
namespace bus
{

UART_BB_Bus::UART_BB_Bus()
    : m_descriptor(new hal::UART_BB_Descriptor())
{
}

UART_BB_Bus::~UART_BB_Bus()
{
}

ts::Result<void> UART_BB_Bus::init(hal::IBus_Descriptor const& descriptor)
{
    auto specialized = dynamic_cast<hal::UART_BB_Descriptor const*>(&descriptor);
    if (!specialized)
    {
        return make_error("Wrong descriptor type");
    }
    *m_descriptor = *specialized;

    return m_uart.init(specialized->get_rx_pin(), specialized->get_baud(), specialized->get_invert());
}

std::shared_ptr<const hal::IBus_Descriptor> UART_BB_Bus::get_descriptor() const
{
    return m_descriptor;
}

util::hw::IUART& UART_BB_Bus::get_uart()
{
    return m_uart;
}

}
}
