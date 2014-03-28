#pragma once

#include "util/format.h"

namespace board
{
	class UART;
}


namespace debug
{
	//call this to setup the uart used to log asserts in debug
	//pass null and asserts will not be logged (but will still freeze the board)
	extern void init(board::UART* uart);
	
	namespace detail
	{
		//this function never returns
		void handle_assert(const char* condition, const char* file, int line, const char* msg);
		void trace(const char* file, int line, const char* msg);
		void print(const char* msg);
	}
}

//macro used to avoid warnings about unused variables
#define UNUSED(x) (void)sizeof(x)

#ifndef NDEBUG

#	define ASSERT(condition)															\
	do 																					\
	{  																					\
		if (!(condition))																\
		{																				\
			debug::detail::handle_assert(#condition, __FILE__, __LINE__, nullptr);/*this never returns*/	\
		}																				\
	} while(0)

#	define ASSERT_MSG(condition, fmt, ...)											\
	do 																				\
	{																				\
		if (!(condition)) 															\
		{																			\
			util::FString<128> __msg;												\
			util::format(__msg, fmt, ##__VA_ARGS__);								\
			debug::detail::handle_assert(#condition, __FILE__, __LINE__, __msg.c_str());/*this never returns*/\
		}																			\
	} while(0)

#else

#	define ASSERT(condition) UNUSED(condition)
#	define ASSERT_MSG(condition, fmt, ...) do { UNUSED(condition); UNUSED(fmt); } while(0)

#	define ASSERT_ONCE(condition) QUNUSED(condition)
#	define ASSERT_MSG_ONCE(condition, fmt, ...) do { UNUSED(condition); UNUSED(fmt); } while(0)

#endif

//////////////////////////////////////////////////////////////////////////

#define PANIC()																	\
do 																				\
{																				\
	debug::detail::handle_assert("PANIC", __FILE__, __LINE__, nullptr);/*this never returns*/\
} while(0)

#define PANIC_MSG(fmt, ...)														\
do 																				\
{																				\
	util::FString<128> __msg;													\
	util::format(__msg, fmt, ##__VA_ARGS__);									\
	debug::detail::handle_assert("PANIC", __FILE__, __LINE__, __msg.c_str());/*this never returns*/\
} while(0)

//////////////////////////////////////////////////////////////////////////

#define PRINT(fmt, ...)							\
do												\
{												\
	util::FString<128> __msg;					\
	util::format(__msg, fmt, ##__VA_ARGS__);	\
	debug::detail::print(__msg.c_str());		\
} while(0)

//////////////////////////////////////////////////////////////////////////

#define ENABLE_TRACE

#ifdef ENABLE_TRACE										
#	define TRACE()										\
	do													\
	{													\
		debug::detail::trace(__FILE__, __LINE__, nullptr);	\
	} while(0)
#	define TRACE_MSG(fmt, ...)							\
	do													\
	{													\
		util::FString<128> __msg;						\
		util::format(__msg, fmt, ##__VA_ARGS__);		\
		debug::detail::trace(__FILE__, __LINE__, __msg.c_str());	\
	} while(0)
#else
#	define TRACE()
#	define TRACE_MSG(fmt, ...);
#endif

///////////////////////////////////////////////////////////////////////////////

namespace debug
{
namespace detail
{
	class Timed_Scope
	{
	public:
		Timed_Scope(char const* file, int line);
		~Timed_Scope();
	private:
		char const* m_file;
		int m_line;
		chrono::time_us m_start;
	};
}
}

#define TIMED_BLOCK() debug::detail::Timed_Scope __ts__(__FILE__, __LINE__);



