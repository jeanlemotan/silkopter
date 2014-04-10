#include "debug/debug.h"

#ifdef __AVR__
#	include <avr/interrupt.h>
#endif

#include "board/board.h"


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
	s_is_initialized = true;

	s_uart = uart;
}

board::UART* get_uart()
{
	return s_uart;
}

namespace detail
{

#ifdef __AVR__

void handle_assert(const char* condition, const char* file, int line, const char* msg)
{
	board::scheduler::stop();
	if (s_uart)
	{
		s_uart->set_blocking(true);
		s_uart->write(F_STR("\n#"));
		if (msg)
		{
			s_uart->write_c_str(msg);
		}
		if (file)
		{
			board::UART_Format_Adapter adapter(*s_uart);
			util::format(adapter, F_STR("\n@ {}:{}"), file, line);
		}
		if (condition)
		{
			s_uart->write('\n');
			s_uart->write(condition ? condition : "N/A");
		}

		s_uart->flush();
	}

	//make sure nothing else happens
	cli();
	//freeze
	while (true);
}

#else

	void handle_assert(const char* condition, const char* file, int line, const char* msg)
	{
		__asm
		{
			int 3
		}
	}

#endif

void trace(const char* file, int line, const char* msg)
{
	if (s_uart)
	{
		auto blocking = s_uart->is_blocking();
		s_uart->set_blocking(true);
		s_uart->write(F_STR("\nTrace: "));
		if (msg)
		{
			s_uart->write_c_str(msg);
		}
		if (file)
		{
			board::UART_Format_Adapter adapter(*s_uart);
			util::format(adapter, F_STR(" @ {}:{}"), file, line);
		}
		s_uart->flush();
		s_uart->set_blocking(blocking);
	}
}

//////////////////////////////////////////////////////////////////////////

Timed_Scope::Timed_Scope(char const* file, int line)
	: m_file(file)
	, m_line(line)
	, m_start(board::clock::now_us())
{
		
}
Timed_Scope::~Timed_Scope()
{
	auto d = board::clock::now_us() - m_start;
	debug::printf(F_STR("\nScope {}:{} > {}"), m_file, m_line, d);
}


}
}