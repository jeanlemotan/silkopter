#pragma once

#include "Format/Format.h"

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
	}
}

//macro used to avoid warnings about unused variables
#define UNUSED(x) (void)sizeof(x)

#ifdef _DEBUG

#	define ASSERT(condition)															\
	do 																					\
	{  																					\
		if (!(condition))																\
		{																				\
			debug::detail::handle_assert(#condition, __FILE__, __LINE__);/*this never returns*/	\
		}																				\
	} while(0)

#	define ASSERT_MSG(condition, fmt, ...)											\
	do 																				\
	{																				\
		if (!(condition)) 															\
		{																			\
			FString<128> msg;														\
			format(msg, fmt, ##__VA_ARGS__);										\
			debug::detail::handle_assert(#condition, __FILE__, __LINE__, msg)/*this never returns*/\
		}																			\
	} while(0)

#else

#	define ASSERT(condition) UNUSED(condition)
#	define ASSERT_MSG(condition, fmt, ...) do { UNUSED(condition); UNUSED(fmt); } while(0)

#	define ASSERT_ONCE(condition) QUNUSED(condition)
#	define ASSERT_MSG_ONCE(condition, fmt, ...) do { UNUSED(condition); UNUSED(fmt); } while(0)

#endif

///////////////////////////////////////////////////////////////////////////////




