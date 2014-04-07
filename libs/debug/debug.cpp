#include <avr/interrupt.h>
#include "debug/debug.h"
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


namespace detail
{
	
void handle_assert(const char* condition, const char* file, int line, const char* msg)
{
	board::scheduler::stop();
	if (s_uart)
	{
		s_uart->set_blocking(true);
		s_uart->write_c_str("\n#");
		if (msg)
		{
			s_uart->write_c_str(msg);
		}
		if (file)
		{
			util::String<512> str;
			util::format(str, "\n@ {0}:{1}", file, line);
			s_uart->write_c_str(str.c_str());
		}
		if (condition)
		{
			s_uart->write('\n');
			s_uart->write_c_str(condition ? condition : "N/A");
		}

		s_uart->write_c_str("\nThe board will now freeze...");
		s_uart->flush();
	}
	//make sure nothing else happens
	cli();

	//freeze
	while (true);
}

void trace(const char* file, int line, const char* msg)
{
	if (s_uart)
	{
		auto blocking = s_uart->is_blocking();
		s_uart->set_blocking(true);
		s_uart->write_c_str("\nTrace: ");
		if (msg)
		{
			s_uart->write_c_str(msg);
		}
		if (file)
		{
			util::String<512> str;
			util::format(str, " @ {0}:{1}", file, line);
			s_uart->write_c_str(str.c_str());
		}
		s_uart->flush();
		s_uart->set_blocking(blocking);
	}
}

void print(const char* msg)
{
	if (s_uart && msg)
	{
		auto blocking = s_uart->is_blocking();
		s_uart->set_blocking(true);
		s_uart->write_c_str(msg);
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
	util::String<128> msg;
	util::format(msg, "\nScope {0}:{1} took {2}", m_file, m_line, d);
	print(msg.c_str());
}


}
}