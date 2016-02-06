#include "Config.h"

#if defined CPU_TYPE && CPU_TYPE == CPU_ATMEGA_2560

#include <avr/interrupt.h>
#include <_qmath.h>
#include "board/boards/AVR_UART.h"

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
		   volatile uint8_t* ucsrc,
		   volatile uint8_t* udr)
	: m_ubrrh(*ubrrh)
	, m_ubrrl(*ubrrl)
	, m_ucsra(*ucsra)
	, m_ucsrb(*ucsrb)
	, m_ucsrc(*ucsrc)
	, m_udr(*udr)
{
	QASSERT(port < MAX_UARTS);
	QASSERT(!s_uart_ptrs[port]);
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
void AVR_UART::set_buffered(bool buffered)
{
	if (!buffered)
	{
		flush();
		m_ucsrb &= ~(_BV(UDRIE0));
	}
	m_is_buffered = buffered;
}
bool AVR_UART::is_buffered() const
{
	return m_is_buffered;
}

void AVR_UART::begin(uint32_t baud, size_t tx_buffer_size, size_t rx_buffer_size)
{
	if (m_baud == baud)
	{
		return;
	}

    if (tx_buffer_size > 256 || !math::is_pot(tx_buffer_size))
    {
        PANIC();
    }
    if (rx_buffer_size > 256 || !math::is_pot(rx_buffer_size))
    {
        PANIC();
    }

	end();
	
    m_rx_buffer.size = rx_buffer_size;
    m_rx_buffer.mask = rx_buffer_size - 1;
    m_rx_buffer.data = reinterpret_cast<uint8_t*>(malloc(rx_buffer_size));

    m_tx_buffer.size = tx_buffer_size;
    m_tx_buffer.mask = tx_buffer_size - 1;
    m_tx_buffer.data = reinterpret_cast<uint8_t*>(malloc(tx_buffer_size));

    m_rx_buffer.head = m_rx_buffer.tail = 0;
    m_tx_buffer.head = m_tx_buffer.tail = 0;
	
	uint32_t xbaud = 0;
	bool use_u2x = true;

		// If the user has supplied a new baud rate, compute the new UBRR value.
#if F_CPU == 16000000UL
	// hardcoded exception for compatibility with the bootloader shipped
	// with the Duemilanove and previous boards and the firmware on the 8U2
	// on the Uno and Mega 2560.
	if (baud == 57600)
	{
		use_u2x = false;
	}
#endif

	if (use_u2x) 
	{
		m_ucsra = 1 << U2X0;
		xbaud = (F_CPU / 4 / baud - 1) / 2;
	} 
	else 
	{
		m_ucsra = 0;
		xbaud = (F_CPU / 8 / baud - 1) / 2;
	}

	m_ubrrh = (uint8_t)(xbaud>>8);
	m_ubrrl = (uint8_t) xbaud;

	m_ucsrb &= ~_BV(UDRIE0);
    m_ucsrb |= _BV(RXEN0) | _BV(TXEN0); //transmit and receive
	m_ucsrb |= _BV(RXCIE0); //receiver interrupt
	m_ucsrc |= _BV(UCSZ01) | _BV(UCSZ00); //format
	
	m_baud = baud;
}

void AVR_UART::end()
{
	m_ucsrb &= ~(_BV(UDRIE0) | _BV(RXEN0) | _BV(TXEN0) | _BV(RXCIE0));

	free((void*)(m_rx_buffer.data));
	m_rx_buffer.data = nullptr;
	free((void*)(m_tx_buffer.data));
	m_tx_buffer.data = nullptr;
	
	m_baud = 0;
}

UART::Error AVR_UART::get_last_error() const
{
	return m_last_error;
}

size_t AVR_UART::get_data_size() const
{
	QASSERT(m_baud > 0);
	if (m_baud == 0)
	{
		return 0;
	}
    return ((m_rx_buffer.size + m_rx_buffer.head - m_rx_buffer.tail) & m_rx_buffer.mask);
}
bool AVR_UART::has_data() const
{
	QASSERT(m_baud > 0);
	if (m_baud == 0)
	{
		return false;
	}
    return ((m_rx_buffer.size + m_rx_buffer.head - m_rx_buffer.tail) & m_rx_buffer.mask) > 0;
}
size_t AVR_UART::read(uint8_t* buf, size_t size)
{
	QASSERT(m_baud > 0);
	if (m_baud == 0)
	{
		return 0;
	}

	m_last_error = Error::NONE;

	if (m_is_blocking)
	{
		for (size_t i = 0; i < size; i++)
		{
            while (m_rx_buffer.head == m_rx_buffer.tail); //wait for data

            uint8_t tmptail = (m_rx_buffer.tail + 1) & m_rx_buffer.mask;
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

            uint8_t tmptail = (m_rx_buffer.tail + 1) & m_rx_buffer.mask;
            m_rx_buffer.tail = tmptail;
            *buf++ = m_rx_buffer.data[tmptail];
        }
	}

	return size;
}
	
size_t AVR_UART::write(const uint8_t* buf, size_t size)
{
	QASSERT(m_baud > 0);
	if (m_baud == 0)
	{
		return 0;
	}
	m_last_error = Error::NONE;
	if (!buf || size == 0)
	{
		return 0;
	}
	
	if (m_is_blocking && !m_is_buffered)
	{
		for (size_t i = 0; i < size; i++)
		{
			while (!(m_ucsra & _BV(UDRE0)));
			m_udr = *buf++;
		}
	}
	else
    {
		size_t left = size;
		while (left > 0)
        {
            uint8_t tail = m_tx_buffer.tail;
            uint8_t start = (m_tx_buffer.head + 1) & m_tx_buffer.mask;
            uint8_t end = start > tail ? m_tx_buffer.size : tail;
            size_t fit = std::min<size_t>(end - start, left);
            if (fit > 0)
            {
                memcpy((uint8_t*)m_tx_buffer.data + start, buf, fit);
                m_tx_buffer.head = (m_tx_buffer.head + fit) & m_tx_buffer.mask;
                m_ucsrb |= _BV(UDRIE0); //trigger the interrupt
                left -= fit;
                buf += fit;
            }
            else
            {
				if (!m_is_blocking)
				{
					m_last_error = Error::TX_OVERFLOW;
					return size - left;
				}
			}
		}
	}
	return size;
}

size_t AVR_UART::write(util::Flash_String const& str)
{
	size_t size = 0;
	for (auto it = str.begin(); *it != 0; ++it, ++size)
	{
		if (write(*it) == 0)
		{
			return size;
		}
	}
	return size;
}

void AVR_UART::flush()
{
	QASSERT(m_baud > 0);
	if (m_baud == 0 || !m_is_buffered)
	{
		return;
	}
    uint8_t tmphead = m_tx_buffer.head;
    while (tmphead != m_tx_buffer.tail);
}

size_t AVR_UART::get_rx_data_counter() const
{
	return m_rx_data_counter;
}

}

#endif
