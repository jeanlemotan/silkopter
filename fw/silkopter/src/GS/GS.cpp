#include "GS/GS.h"

using namespace silk;

GS::GS(board::UART& full_uart)
	: m_full_uart(full_uart)
	, m_compact_uart(nullptr)
	, m_full_protocol(full_uart)
{
	full_uart.set_blocking(true);
}
GS::GS(board::UART& full_uart, board::UART& compact_uart)
	: m_full_uart(full_uart)
	, m_compact_uart(&compact_uart)
	, m_full_protocol(full_uart)
	, m_compact_protocol(compact_uart)
{
	full_uart.set_blocking(true);
	compact_uart.set_blocking(true);
}

void GS::process()
{
	
}


