#pragma once

#define UART_BUFFER_SIZE 32 //has to be power of two
#define UART_BUFFER_MASK (UART_BUFFER_SIZE - 1)
#define UART_COUNT 4

#include <stdint.h>

namespace board
{
	
class UART
{
public:
	UART(uint8_t port);
	
	//Selects between blocking writes and non-blocking
	//NOTE: non-blocking writes will skip data when the buffer is full and result ERR_TX_OVERFLOW
	//By default the UART is blocking
	//This can be called anytime
	void set_blocking(bool blocking); 
	bool is_blocking() const;
	
    void begin(uint32_t baud);

	enum class Error
	{
		NONE,
		RX_OVERFLOW,
		RX_UNDERFLOW,
		TX_OVERFLOW,
	};

	size_t get_data_size() const;
	bool has_data() const;
	uint8_t read_byte();
	bool read(uint8_t* buf, uint8_t size);
	
    bool write(const char* buf);
    bool write(const uint8_t* buf, size_t size);
	bool write_byte(uint8_t b);
	
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

	volatile uint8_t& m_ubrrh;
	volatile uint8_t& m_ubrrl;
	volatile uint8_t& m_ucsra;
	volatile uint8_t& m_ucsrb;
	volatile uint8_t& m_ucsrc;
	
	volatile Error m_last_error;

	bool m_blocking;
};

extern UART uart0;
extern UART uart1;
extern UART uart2;
extern UART uart3;

}
		