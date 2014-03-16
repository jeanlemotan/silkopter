#include <avr/interrupt.h>
#include <qmath.h>

#include "Debug/Assert.h"
#include "board/boards/Crius_AIOP2/UART.h"

#if BOARD_TYPE == CRIUS_AIOP2

using namespace board;

#define UART_BAUD_SELECT(baud, xtalCpu) (((xtalCpu) + 8UL * (baud)) / (16UL * (baud)) -1UL)

static UART* s_uarts[UART_COUNT] = {0};

static volatile uint8_t* s_ubrrh[UART_COUNT] = { &UBRR0H, &UBRR1H, &UBRR2H, &UBRR3H };
static volatile uint8_t* s_ubrrl[UART_COUNT] = { &UBRR0L, &UBRR1L, &UBRR2L, &UBRR3L };
static volatile uint8_t* s_ucsra[UART_COUNT] = { &UCSR0A, &UCSR1A, &UCSR2A, &UCSR3A };
static volatile uint8_t* s_ucsrb[UART_COUNT] = { &UCSR0B, &UCSR1B, &UCSR2B, &UCSR3B };
static volatile uint8_t* s_ucsrc[UART_COUNT] = { &UCSR0C, &UCSR1C, &UCSR2C, &UCSR3C };

//////////////////////////////////////////////////////////////////////////

//Function: UART Receive Complete interrupt
//Purpose:  called when the UART has received a character

#define UART_ISR(id)											\
ISR(USART##id##_RX_vect)										\
{																\
	auto& buffer = s_uarts[id]->m_rx_buffer;					\
	uint8_t data = UDR##id;										\
	uint8_t tmphead = (buffer.head + 1) & UART_BUFFER_MASK;		\
	if (tmphead == buffer.tail)									\
	{															\
		s_uarts[id]->m_last_error = UART::Error::RX_OVERFLOW;	\
	}															\
	else														\
	{															\
		buffer.head = tmphead;									\
		buffer.data[tmphead] = data;							\
	}															\
}																\
ISR(USART##id##_UDRE_vect)										\
{																\
	auto& buffer = s_uarts[id]->m_tx_buffer;					\
	if (buffer.head == buffer.tail)								\
	{															\
		UCSR##id##B &= ~_BV(UDRIE##id);							\
		return;													\
	}															\
	uint8_t tmptail = (buffer.tail + 1) & UART_BUFFER_MASK;		\
	buffer.tail = tmptail;										\
	UDR##id = buffer.data[tmptail];								\
}

UART_ISR(0)
UART_ISR(1)
UART_ISR(2)
UART_ISR(3)


//////////////////////////////////////////////////////////////////////////

UART::UART(uint8_t port)
	: m_ubrrh(*s_ubrrh[port])
	, m_ubrrl(*s_ubrrl[port])
	, m_ucsra(*s_ucsra[port])
	, m_ucsrb(*s_ucsrb[port])
	, m_ucsrc(*s_ucsrc[port])
	, m_blocking(true)
{
	ASSERT(!s_uarts[port]);
	s_uarts[port] = this;
}

void UART::set_blocking(bool blocking)
{
	m_blocking = blocking;
}

void UART::begin(uint32_t baud)
{
	uint32_t xbaud = UART_BAUD_SELECT(baud, F_CPU);
	
    m_ubrrh = (uint8_t)(xbaud>>8);
    m_ubrrl = (uint8_t) xbaud;
	m_ucsrb &= ~_BV(UDRIE0);
    m_ucsrb |= _BV(RXEN0) | _BV(TXEN0); //transmit and receive
	m_ucsrb |= _BV(RXCIE0); //receiver interrupt
	m_ucsrc |= _BV(UCSZ01) | _BV(UCSZ00); //format
}

UART::Error UART::get_last_error() const
{
	return m_last_error;
}

size_t UART::get_data_size() const
{
	return (UART_BUFFER_MASK + m_rx_buffer.head - m_rx_buffer.tail) & UART_BUFFER_MASK;
}
bool UART::has_data() const
{
	return (UART_BUFFER_MASK + m_rx_buffer.head - m_rx_buffer.tail) & UART_BUFFER_MASK;
}
uint8_t UART::read_byte()
{
    if (m_rx_buffer.head == m_rx_buffer.tail)
	{
		m_last_error = Error::RX_UNDERFLOW;
	    return 0;
    }
	m_last_error = Error::NONE;
    
    uint8_t tmptail = (m_rx_buffer.tail + 1) & UART_BUFFER_MASK;
    m_rx_buffer.tail = tmptail;
    return m_rx_buffer.data[tmptail];
}
bool UART::read(uint8_t* buf, uint8_t size)
{
	return 0;
}
	
bool UART::write(const uint8_t* buf, size_t size)
{
	if (!buf || size == 0)
	{
		m_last_error = Error::NONE;
		return true;
	}

	if (m_blocking)
	{
		do
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
		while(--size > 0);
		m_ucsrb |= _BV(UDRIE0); //trigger the interrupt
	}
	else
	{
		do
		{
			uint8_t tmphead = (m_tx_buffer.head + 1) & UART_BUFFER_MASK;
			if (tmphead == m_tx_buffer.tail)
			{
				m_last_error = Error::TX_OVERFLOW;
				return false;
			}
			
			m_tx_buffer.data[tmphead] = *buf++;
			m_tx_buffer.head = tmphead;
			m_ucsrb |= _BV(UDRIE0);
		}
		while(--size > 0);
	}
	m_last_error = Error::NONE;
	return true;
}

bool UART::write(const char* buf)
{
	if (!buf)
	{
		m_last_error = Error::NONE;
		return true;
	}
	uint8_t size = 0;
	if (m_blocking)
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
				return false;
			}
		
			m_tx_buffer.data[tmphead] = ch;
			m_tx_buffer.head = tmphead;
			m_ucsrb |= _BV(UDRIE0);
			size++;
		}
	}
	m_last_error = Error::NONE;
	return true;
}

bool UART::write_byte(uint8_t b)
{
    uint8_t tmphead = (m_tx_buffer.head + 1) & UART_BUFFER_MASK;
	if (m_blocking)
	{
		while (tmphead == m_tx_buffer.tail);
	}
    else if (tmphead == m_tx_buffer.tail)
	{
		m_last_error = Error::TX_OVERFLOW;
		return false;
	}
    
    m_tx_buffer.data[tmphead] = b;
    m_tx_buffer.head = tmphead;
    m_ucsrb |= _BV(UDRIE0);

	m_last_error = Error::TX_OVERFLOW;
	return true;
}

void UART::flush()
{
    uint8_t tmphead = m_tx_buffer.head;
	while (tmphead != m_tx_buffer.tail);
}

namespace board
{
	UART uart0(0);
	UART uart1(1);
	UART uart2(2);
	UART uart3(3);
}

#endif
