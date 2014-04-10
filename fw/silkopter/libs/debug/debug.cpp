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


namespace detail
{

#ifdef __AVR__

void handle_assert(const char* condition, const char* file, int line, const char* msg)
{
	board::scheduler::stop();
	if (s_uart)
	{
		s_uart->set_blocking(true);
		s_uart->write_c_str(util::FString<6>(F_STR("\n#")).c_str());
		if (msg)
		{
			s_uart->write_c_str(msg);
		}
		if (file)
		{
			util::FString<512> str;
			util::format(str, F_STR("\n@ {}:{}"), file, line);
			s_uart->write_c_str(str.c_str());
		}
		if (condition)
		{
			s_uart->write('\n');
			s_uart->write_c_str(condition ? condition : util::FString<6>(F_STR("N/A")).c_str());
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
		s_uart->write_c_str(util::FString<16>(F_STR("\nTrace: ")).c_str());
		if (msg)
		{
			s_uart->write_c_str(msg);
		}
		if (file)
		{
			util::FString<512> str;
			util::format(str, F_STR(" @ {}:{}"), file, line);
			s_uart->write_c_str(str.c_str());
		}
		s_uart->flush();
		s_uart->set_blocking(blocking);
	}
}

void print(const char* msg, size_t size)
{
	if (s_uart && msg && size > 0)
	{
		auto blocking = s_uart->is_blocking();
		s_uart->set_blocking(true);
		s_uart->write(reinterpret_cast<uint8_t const*>(msg), size);
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