#include "Config.h"

#if BOARD_TYPE == SIM

#include <_qmath.h>
#include "debug/debug.h"
#include "board/boards/Sim/Sim_UART.h"

namespace board
{
	
//////////////////////////////////////////////////////////////////////////

Sim_UART::Sim_UART(uint8_t port)
	: m_port_idx(port)
{
}

void Sim_UART::set_blocking(bool blocking)
{
	m_is_blocking = blocking;
}
bool Sim_UART::is_blocking() const
{
	return m_is_blocking;
}

void Sim_UART::set_buffered(bool buffered)
{

}
bool Sim_UART::is_buffered() const
{
	return true;
}


void Sim_UART::begin(uint32_t baud)
{
	m_is_open = true;
}
void Sim_UART::end()
{
	m_is_open = false;
}
UART::Error Sim_UART::get_last_error() const
{
	return UART::Error::NONE;
}

size_t Sim_UART::get_data_size() const
{
	ASSERT(m_is_open);
	if (!m_is_open)
	{
		return 0;
	}
	return 0;
}
bool Sim_UART::has_data() const
{
	ASSERT(m_is_open);
	if (!m_is_open)
	{
		return false;
	}
	return false;
}
size_t Sim_UART::read(uint8_t* buf, size_t size)
{
	ASSERT(m_is_open);
	if (!m_is_open)
	{
		return 0;
	}

	return 0;
}

size_t Sim_UART::write(util::Flash_String const& str)
{
	return 0;
}

size_t Sim_UART::write(const uint8_t* buf, size_t size)
{
	ASSERT(m_is_open);
	if (!m_is_open)
	{
		return 0;
	}
	if (!buf || size == 0)
	{
		return 0;
	}

	return 0;
}

void Sim_UART::flush()
{
	ASSERT(m_is_open);
	if (!m_is_open)
	{
		return;
	}
}

}

#endif
