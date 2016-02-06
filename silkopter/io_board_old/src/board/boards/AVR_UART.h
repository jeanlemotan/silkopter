#pragma once

#include <stdint.h>
#include <stddef.h>
#include "board/UART.h"

namespace board
{

class AVR_UART : public UART
{
public:
	AVR_UART(uint8_t port,
		 volatile uint8_t* ubrrh,
		 volatile uint8_t* ubrrl,
		 volatile uint8_t* ucsra,
		 volatile uint8_t* ucsrb,
		 volatile uint8_t* ucsrc,
		 volatile uint8_t* udr);
		 
	//Selects between blocking writes and non-blocking
	//NOTE: non-blocking writes will skip data when the buffer is full and result ERR_TX_OVERFLOW
	//By default the UART is blocking
	//This can be called anytime
	void set_blocking(bool blocking); 
	bool is_blocking() const;
	
	void set_buffered(bool buffered);
	bool is_buffered() const;
	
    void begin(uint32_t baud, size_t tx_buffer_size, size_t rx_buffer_size);
	void end();

	size_t get_data_size() const;
	bool has_data() const;
	uint8_t read_byte();
	size_t read(uint8_t* buf, size_t size);

	size_t write(util::Flash_String const& str);
    size_t write(uint8_t const* buf, size_t size);
	using UART::write;
	
	void flush();
	
	Error get_last_error() const;
	
	size_t get_rx_data_counter() const;
		
public:
    struct Buffer
    {
        size_t size = 0;
        uint8_t mask = 0;

        volatile uint8_t* data = nullptr;
        volatile uint8_t head = 0;
        volatile uint8_t tail = 0;
    };

    Buffer m_tx_buffer;
    Buffer m_rx_buffer;
	volatile Error m_last_error = Error::NONE;
	
	volatile size_t m_rx_data_counter = 0;
	
	static const uint8_t MAX_UARTS = 8;

	static AVR_UART* s_uart_ptrs[MAX_UARTS];

private:
	volatile uint8_t& m_ubrrh;
	volatile uint8_t& m_ubrrl;
	volatile uint8_t& m_ucsra;
	volatile uint8_t& m_ucsrb;
	volatile uint8_t& m_ucsrc;
	volatile uint8_t& m_udr;
	
	bool m_is_blocking = true;
	bool m_is_buffered = true;
	uint32_t m_baud = 0;
};

//////////////////////////////////////////////////////////////////////////

#define UART_ISR(id)											\
ISR(USART##id##_RX_vect)										\
{																\
	uint8_t data = UDR##id;										\
	auto& buffer = board::AVR_UART::s_uart_ptrs[id]->m_rx_buffer;	\
    uint8_t tmphead = (buffer.head + 1) & buffer.mask;		\
    if (tmphead == buffer.tail)									\
	{															\
		board::AVR_UART::s_uart_ptrs[id]->m_last_error = UART::Error::RX_OVERFLOW;	\
	}															\
	else														\
	{															\
        buffer.head = tmphead;									\
        buffer.data[tmphead] = data;							\
        /*board::AVR_UART::s_uart_ptrs[id]->m_rx_data_counter++;*/	\
	}															\
}																\
ISR(USART##id##_UDRE_vect)										\
{																\
	auto& buffer = board::AVR_UART::s_uart_ptrs[id]->m_tx_buffer;	\
    if (buffer.head == buffer.tail)								\
	{															\
		UCSR##id##B &= ~_BV(UDRIE##id);							\
		return;													\
	}															\
    uint8_t tmptail = (buffer.tail + 1) & buffer.mask;		\
    buffer.tail = tmptail;										\
    UDR##id = buffer.data[tmptail];								\
}


}

