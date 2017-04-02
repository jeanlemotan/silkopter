#include "FCStdAfx.h"
#include "bus/UART_Linux_Bus.h"

#include "hal.def.h"

namespace silk
{
namespace bus
{

UART_Linux_Bus::UART_Linux_Bus()
    : m_descriptor(new hal::UART_Linux_Descriptor())
{
}

UART_Linux_Bus::~UART_Linux_Bus()
{
}

ts::Result<void> UART_Linux_Bus::init(hal::IBus_Descriptor const& descriptor)
{
    auto specialized = dynamic_cast<hal::UART_Linux_Descriptor const*>(&descriptor);
    if (!specialized)
    {
        return ts::Error("Wrong descriptor type");
    }
    *m_descriptor = *specialized;

    util::hw::UART_Dev::Baud baud;
    switch (m_descriptor->get_baud())
    {
    case hal::UART_Linux_Descriptor::baud_t::_9600: baud = util::hw::UART_Dev::Baud::_9600; break;
    case hal::UART_Linux_Descriptor::baud_t::_19200: baud = util::hw::UART_Dev::Baud::_19200; break;
    case hal::UART_Linux_Descriptor::baud_t::_38400: baud = util::hw::UART_Dev::Baud::_38400; break;
    case hal::UART_Linux_Descriptor::baud_t::_57600: baud = util::hw::UART_Dev::Baud::_57600; break;
    case hal::UART_Linux_Descriptor::baud_t::_115200: baud = util::hw::UART_Dev::Baud::_115200; break;
    case hal::UART_Linux_Descriptor::baud_t::_230400: baud = util::hw::UART_Dev::Baud::_230400; break;
    default:
        return ts::Error("Invalid baud requested");
    }

    return m_uart.init(m_descriptor->get_dev(), baud);
}

std::shared_ptr<const hal::IBus_Descriptor> UART_Linux_Bus::get_descriptor() const
{
    return m_descriptor;
}

util::hw::IUART& UART_Linux_Bus::get_uart()
{
    return m_uart;
}

}
}
