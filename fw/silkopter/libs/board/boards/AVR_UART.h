#pragma once

#define UART_BUFFER_SIZE 32 //has to be power of two
#define UART_BUFFER_MASK (UART_BUFFER_SIZE - 1)

#include <stdint.h>
#include <stddef.h>

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
		 volatile uint8_t* ucsrc);
		 
	//Selects between blocking writes and non-blocking
	//NOTE: non-blocking writes will skip data when the buffer is full and result ERR_TX_OVERFLOW
	//By default the UART is blocking
	//This can be called anytime
	void set_blocking(bool blocking); 
	bool is_blocking() const;
	
    void begin(uint32_t baud);

	size_t get_data_size() const;
	bool has_data() const;
	uint8_t read_byte();
	size_t read(uint8_t* buf, size_t size);

    size_t write_c_str(const char* buf);
    size_t write(uint8_t const* buf, size_t size);
	
	void flush();
	
	Error get_last_error() const;
	
public:
	struct Buffer
	{
		Buffer() : head(0), tail(0) {}
		volatile uint8_t data[UART_BUFFER_SIZE];
		volatile uint8_t head;
		volatile uint8_t tail;
	};

	Buffer m_tx_buffer;
	Buffer m_rx_buffer;
	volatile Error m_last_error;
	
	static const uint8_t MAX_UARTS = 8;

	static UART* s_uart_ptrs[MAX_UARTS];

private:
	volatile uint8_t& m_ubrrh;
	volatile uint8_t& m_ubrrl;
	volatile uint8_t& m_ucsra;
	volatile uint8_t& m_ucsrb;
	volatile uint8_t& m_ucsrc;
	

	bool m_is_blocking;
	bool m_is_open;
};

//////////////////////////////////////////////////////////////////////////

#define UART_ISR(id)											\
ISR(USART##id##_RX_vect)										\
{																\
	auto& buffer = board::UART::s_uart_ptrs[id]->m_rx_buffer;	\
	uint8_t data = UDR##id;										\
	uint8_t tmphead = (buffer.head + 1) & UART_BUFFER_MASK;		\
	if (tmphead == buffer.tail)									\
	{															\
		board::UART::s_uart_ptrs[id]->m_last_error = UART::Error::RX_OVERFLOW;	\
	}															\
	else														\
	{															\
		buffer.head = tmphead;									\
		buffer.data[tmphead] = data;							\
	}															\
}																\
ISR(USART##id##_UDRE_vect)										\
{																\
	auto& buffer = board::UART::s_uart_ptrs[id]->m_tx_buffer;	\
	if (buffer.head == buffer.tail)								\
	{															\
		UCSR##id##B &= ~_BV(UDRIE##id);							\
		return;													\
	}															\
	uint8_t tmptail = (buffer.tail + 1) & UART_BUFFER_MASK;		\
	buffer.tail = tmptail;										\
	UDR##id = buffer.data[tmptail];								\
}

}

		