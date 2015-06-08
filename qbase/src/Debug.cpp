#include "QBaseStdAfx.h"
#include "QBase.h"

#if defined Q_AVR
#	include <avr/interrupt.h>
#	include "board/board.h"
#else
#	include <set>
#   if defined _MSC_VER
#       pragma warning( disable : 4996)
#   endif
#endif

///////////////////////////////////////////////////////////////////////////////

namespace q
{
namespace debug
{
namespace detail
{

    static void default_handler(const char* condition, const char* file, int line, const char* msg)
	{
#if defined Q_AVR
		board::scheduler::stop();

		if (stderr)
		{
			board::UART* uart = reinterpret_cast<board::UART*>(fdev_get_udata(stderr));
			if (uart)
			{
				uart->set_blocking(true);
				uart->set_buffered(false);
			}

			cli();
			q::util::format(*stderr, F_STR("\n#"));
			if (msg)
			{
				q::util::format(*stderr, F_STR("{}"), msg);
			}
			if (file)
			{
				q::util::format(*stderr, F_STR("\n@ {}:{}"), file, line);
			}
			if (condition)
			{
				q::util::format(*stderr, F_STR("\n{}"), condition);
			}
		}
		//make sure nothing else happens
		//freeze
		while (true);
#else
        quick_logf("Assert '{}' in {}:{} :: {}", condition ? condition : "N/A", file, line, msg ? msg : "N/A");
        dump_stacktrace();
		QBREAK();
#endif
	}

	static q::debug::Assert_handler s_handler = &default_handler;
}
}
}

///////////////////////////////////////////////////////////////////////////////

void q::debug::set_assert_handler(q::debug::Assert_handler h)
{
	q::debug::detail::s_handler = h;
}

///////////////////////////////////////////////////////////////////////////////

void q::debug::detail::call_handler(const char* condition, const char* file, int line, const char* msg)
{
	if (s_handler)
	{
		s_handler(condition, file, line, msg);
	}
}

///////////////////////////////////////////////////////////////////////////////

namespace q
{
namespace debug
{
namespace detail
{

#if !defined Q_AVR
	//the const char* is built using the __FILE__ macro so it's safe to compare the pointer
	typedef std::pair<const char*, int> Assert_Key;

	struct Assert_Key_Compare : std::binary_function <Assert_Key, Assert_Key, bool> 
	{
		bool operator() (const Assert_Key& x, const Assert_Key& y) const 
		{
			return x.first < y.first || (x.first == y.first && x.second < y.second);
		}
	};

	static std::set<Assert_Key, Assert_Key_Compare> s_disabled_asserts;
#endif
}
}
}

#if !defined Q_AVR
bool q::debug::detail::is_assert_enabled(const char* file, int line)
{
	QASSERT(file);
	if (!file)
	{
		return false;
	}
	auto key = std::make_pair(file, line);
	return s_disabled_asserts.find(key) == s_disabled_asserts.end();
}

///////////////////////////////////////////////////////////////////////////////

void q::debug::detail::set_assert_enabled(const char* file, int line, bool enabled)
{
	QASSERT(file);
	if (!file)
	{
		return;
	}
	auto key = std::make_pair(file, line);
	if (enabled)
	{
		s_disabled_asserts.erase(key);
	}
	else
	{
		s_disabled_asserts.insert(key);
	}
}

#if defined Q_POSIX_API
#   include <execinfo.h>
#endif

void q::debug::dump_stacktrace()
{
#if defined Q_POSIX_API
    void* trace_ptrs[256] = { nullptr };
    int count = backtrace(trace_ptrs, 256);
    char** func_names = backtrace_symbols(trace_ptrs, count);

    for(int i = 0; i < count; i++)
    {
        q::quick_logf("{}: {}", i, func_names[i]);
    }

    // Free the string pointers
    free(func_names);
#endif
}

bool q::volatile_true()
{
    return true;
}

bool q::volatile_false()
{
    return false;
}


#endif



