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
		s_uart->set_buffered(false);
		cli();
		s_uart->write(F_STR("\n#"));
		if (msg)
		{
			s_uart->write(reinterpret_cast<uint8_t const*>(msg), strlen(msg));
		}
		if (file)
		{
			s_uart->printf(F_STR("\n@ {}:{}"), file, line);
		}
		if (condition)
		{
			s_uart->printf(F_STR("\n{}"), condition);
		}
	}

	//make sure nothing else happens
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
		s_uart->write(F_STR("\nTrace: "));
		if (msg)
		{
			s_uart->write(reinterpret_cast<uint8_t const*>(msg), strlen(msg));
		}
		if (file)
		{
			util::format(*s_uart, F_STR(" @ {}:{}"), file, line);
		}
		s_uart->flush();
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