#pragma once

#include <stdint.h>
#include <stddef.h>
#include "board/UART.h"

namespace board
{

class Sim_UART : public UART
{
public:
	Sim_UART(uint8_t port);
		 
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
	
private:
	uint8_t m_port_idx = 0;
	bool m_is_blocking = true;
	bool m_is_open = false;
};

}

		