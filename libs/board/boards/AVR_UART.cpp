#include "Config.h"

#if CPU_TYPE == ATMEGA_2560

#include <avr/interrupt.h>
#include <qmath.h>
#include "debug/debug.h"
#include "board/boards/avr_UART.h"

namespace board
{
	
#define UART_BAUD_SELECT(baud, xtalCpu) (((xtalCpu) + 8UL * (baud)) / (16UL * (baud)) -1UL)

AVR_UART* AVR_UART::s_uart_ptrs[MAX_UARTS] = {0};

//////////////////////////////////////////////////////////////////////////

AVR_UART::AVR_UART(uint8_t port,
		   volatile uint8_t* ubrrh,
		   volatile uint8_t* ubrrl,
		   volatile uint8_t* ucsra,
		   volatile uint8_t* ucsrb,
		   volatile uint8_t* ucsrc)
	: m_ubrrh(*ubrrh)
	, m_ubrrl(*ubrrl)
	, m_ucsra(*ucsra)
	, m_ucsrb(*ucsrb)
	, m_ucsrc(*ucsrc)
	, m_is_blocking(true)
	, m_is_open(false)
{
	ASSERT(port < MAX_UARTS);
	ASSERT(!s_uart_ptrs[port]);
	s_uart_ptrs[port] = this;
}

void AVR_UART::set_blocking(bool blocking)
{
	m_is_blocking = blocking;
}
bool AVR_UART::is_blocking() const
{
	return m_is_blocking;
}

void AVR_UART::begin(uint32_t baud)
{
	uint32_t xbaud = UART_BAUD_SELECT(baud, F_CPU);
	
    m_ubrrh = (uint8_t)(xbaud>>8);
    m_ubrrl = (uint8_t) xbaud;
	m_ucsrb &= ~_BV(UDRIE0);
    m_ucsrb |= _BV(RXEN0) | _BV(TXEN0); //transmit and receive
	m_ucsrb |= _BV(RXCIE0); //receiver interrupt
	m_ucsrc |= _BV(UCSZ01) | _BV(UCSZ00); //format
	
	m_is_open = true;
}

UART::Error AVR_UART::get_last_error() const
{
	return m_last_error;
}

size_t AVR_UART::get_data_size() const
{
	ASSERT(m_is_open);
	if (!m_is_open)
	{
		return 0;
	}
	return ((UART_BUFFER_SIZE + m_rx_buffer.head - m_rx_buffer.tail) & UART_BUFFER_MASK);
}
bool AVR_UART::has_data() const
{
	ASSERT(m_is_open);
	if (!m_is_open)
	{
		return false;
	}
	return ((UART_BUFFER_SIZE + m_rx_buffer.head - m_rx_buffer.tail) & UART_BUFFER_MASK) > 0;
}
size_t AVR_UART::read(uint8_t* buf, size_t size)
{
	ASSERT(m_is_open);
	if (!m_is_open)
	{
		return 0;
	}

	m_last_error = Error::NONE;

	if (m_is_blocking)
	{
		for (size_t i = 0; i < size; i++)
		{
			while (m_rx_buffer.head == m_rx_buffer.tail); //wait for data

		    uint8_t tmptail = (m_rx_buffer.tail + 1) & UART_BUFFER_MASK;
		    m_rx_buffer.tail = tmptail;
    		*buf++ = m_rx_buffer.data[tmptail];
		}	
	}
	else
	{
		for (size_t i = 0; i < size; i++)
		{
			if (m_rx_buffer.head == m_rx_buffer.tail)
			{
				m_last_error = Error::RX_UNDERFLOW;
			    return i;
		    }

		    uint8_t tmptail = (m_rx_buffer.tail + 1) & UART_BUFFER_MASK;
		    m_rx_buffer.tail = tmptail;
    		*buf++ = m_rx_buffer.data[tmptail];
		}	
	}

	return size;
}
	
size_t AVR_UART::write(const uint8_t* buf, size_t size)
{
	ASSERT(m_is_open);
	if (!m_is_open)
	{
		return 0;
	}
	m_last_error = Error::NONE;
	if (!buf || size == 0)
	{
		return 0;
	}

	if (m_is_blocking)
	{
		for (size_t i = 0; i < size; i++)
		{
			uint8_t tmphead = (m_tx_buffer.head + 1) & UART_BUFFER_MASK;
			if (tmphead == m_tx_buffer.tail)
			{
				m_ucsrb |= _BV(UDRIE0); //trigger the interrupt
				while (tmphead == m_tx_buffer.tail); //wait for enough room in the buffer
			}
			m_tx_buffer.data[tmphead] = *buf++;
			m_tx_buffer.head = tmphead;
		}
		m_ucsrb |= _BV(UDRIE0); //trigger the interrupt
	}
	else
	{
		for (size_t i = 0; i < size; i++)
		{
			uint8_t tmphead = (m_tx_buffer.head + 1) & UART_BUFFER_MASK;
			if (tmphead == m_tx_buffer.tail)
			{
				m_last_error = Error::TX_OVERFLOW;
				return i;
			}
			
			m_tx_buffer.data[tmphead] = *buf++;
			m_tx_buffer.head = tmphead;
			m_ucsrb |= _BV(UDRIE0);
		}
	}
	return size;
}

size_t AVR_UART::write_c_str(const char* buf)
{
	ASSERT(m_is_open);
	if (!m_is_open)
	{
		return 0;
	}
	m_last_error = Error::NONE;
	if (!buf)
	{
		return 0;
	}
	uint8_t size = 0;
	if (m_is_blocking)
	{
		while (uint8_t ch = *buf++)
		{
			uint8_t tmphead = (m_tx_buffer.head + 1) & UART_BUFFER_MASK;
			if (tmphead == m_tx_buffer.tail)
			{
				m_ucsrb |= _BV(UDRIE0); //trigger the interrupt
				while (tmphead == m_tx_buffer.tail); //wait for enough room in the buffer
			}
			m_tx_buffer.data[tmphead] = ch;
			m_tx_buffer.head = tmphead;
			size++;
		}
		m_ucsrb |= _BV(UDRIE0); //trigger the interrupt
	}
	else
	{
		while (uint8_t ch = *buf++)
		{
			uint8_t tmphead = (m_tx_buffer.head + 1) & UART_BUFFER_MASK;
			if (tmphead == m_tx_buffer.tail)
			{
				m_last_error = Error::TX_OVERFLOW;
				return size;
			}
		
			m_tx_buffer.data[tmphead] = ch;
			m_tx_buffer.head = tmphead;
			m_ucsrb |= _BV(UDRIE0);
			size++;
		}
	}
	return size;
}

void AVR_UART::flush()
{
	ASSERT(m_is_open);
	if (!m_is_open)
	{
		return;
	}
    uint8_t tmphead = m_tx_buffer.head;
	while (tmphead != m_tx_buffer.tail);
}

}

#endif
