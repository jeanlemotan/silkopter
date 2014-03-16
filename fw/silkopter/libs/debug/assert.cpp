#include <avr/interrupt.h>
#include "debug/assert.h"
#include "board/UART.h"

namespace debug
{
	
static bool s_is_initialized = false;	
static board::UART* s_uart = nullptr;
	
void init(board::UART* uart)
{
	if (s_is_initialized)
	{
		return;
	}
	
	s_uart = uart;
	
	s_is_initialized = true;
}


namespace detail
{
	
void handle_assert(const char* condition, const char* file, int line, const char* msg)
{
	if (s_uart)
	{
		s_uart->set_blocking(true);
		s_uart->write("\nAssertion failed");
		s_uart->write("\n\tCondition:");
		s_uart->write(condition ? condition : "N/A");
		s_uart->write("\n\tFile:");
		s_uart->write(file ? file : "N/A");
		s_uart->write("\n\tLine:");
		util::FString<32> str;
		util::format(str, "{0}", line);
		s_uart->write(str.c_str());
		if (msg)
		{
			s_uart->write("\n\tMessage:");
			s_uart->write(msg);
		}
		s_uart->write("\nThe board will now freeze.");
		s_uart->flush();
	}
	//make sure nothing else happens
	cli();

	//freeze
	while (true);
}

}
}