#pragma once

#ifdef Q_POSIX_API
#   include <signal.h>
#endif

///////////////////////////////////////////////////////////////////////////////
//

namespace q
{
	namespace debug
	{
#define DBG_STRING std::string

		//The assert handler is called whenever an assert fails and shoud present a message to the user.
		// 	condition contains a stringified condition that failed
		//	msg is any user supplied message. Can be NULL
		//	file is the name of the file the assert failed
		//	line is the line where the assert failed
		typedef	void (*Assert_handler)(const char* condition, const char* file, int line, const char* msg);
		void set_assert_handler(Assert_handler);


		///////////////////////////////////////////////////////////////////////
		//for internal use only
		namespace detail
		{
			//this is responsible of formatting the messages and calling the handler
			void call_handler(const char* condition, const char* file, int line, const char* msg);

			//uset to mark/test once asserts as being taken or not
			bool is_assert_enabled(const char* file, int line);
			void set_assert_enabled(const char* file, int line, bool enabled);
		}

        void dump_stacktrace();
	}

    extern bool volatile_true();
    extern bool volatile_false();
}

#define QUNUSED(x) (void)sizeof(x)

#ifndef NDEBUG

	#define QASSERT(condition)																\
		do 																					\
		{  																					\
			if (!(condition))																\
			{																				\
				q::debug::detail::call_handler(#condition, __FILE__, __LINE__, nullptr);	\
			}																				\
        } while(q::volatile_false())

	#define QASSERT_MSG(condition, fmt, ...)											\
		do 																				\
		{																				\
			if (!(condition)) 															\
			{																			\
				DBG_STRING _msg__;														\
                q::util::format_emplace(_msg__, fmt, ##__VA_ARGS__);							\
				q::debug::detail::call_handler(#condition, __FILE__, __LINE__, _msg__.c_str());	\
			}																			\
        } while(q::volatile_false())

	#define QASSERT_ONCE(condition)														\
		do 																				\
		{																				\
			/* checking the condition first and then the GetAssertOnce, as chances are that the condition is cheaper */ \
			if (!(condition) && q::debug::detail::is_assert_enabled(__FILE__, __LINE__))	\
			{																			\
				q::debug::detail::set_assert_enabled(__FILE__, __LINE__, false);		\
				q::debug::detail::call_handler(#condition, __FILE__, __LINE__, nullptr);			\
			}																			\
        } while(q::volatile_false())

	#define QASSERT_MSG_ONCE(condition, fmt, ...)										\
		do 																				\
		{																				\
			/* checking the condition first and then the GetAssertOnce, as chances are that the condition is cheaper */ \
			if (!(condition) && q::debug::detail::is_assert_enabled(__FILE__, __LINE__)) 	\
			{																			\
				q::debug::detail::set_assert_enabled(__FILE__, __LINE__, false);				\
				DBG_STRING _msg__;														\
				q::util::format(_msg__, fmt, ##__VA_ARGS__);							\
				q::debug::detail::call_handler(#condition, __FILE__, __LINE__, _msg__.c_str());	\
			}																			\
        } while(q::volatile_false())

#else

	#define QASSERT(condition) QUNUSED(condition)
    #define QASSERT_MSG(condition, fmt, ...)

	#define QASSERT_ONCE(condition) QUNUSED(condition)
    #define QASSERT_MSG_ONCE(condition, fmt, ...)

#endif

//////////////////////////////////////////////////////////////////////////

#define PANIC()																	\
do 																				\
{																				\
	q::debug::detail::call_handler("PANIC", __FILE__, __LINE__, nullptr);/*this never returns*/\
} while(q::volatile_false())

#define PANIC_MSG(fmt, ...)														\
do 																				\
{																				\
	q::FString<128> __msg;														\
	q::util::format(__msg, fmt, ##__VA_ARGS__);									\
	q::debug::detail::call_handler("PANIC", __FILE__, __LINE__, __msg.c_str());/*this never returns*/\
} while(q::volatile_false())

//////////////////////////////////////////////////////////////////////////

#define TRACE() q::quick_logf("trace@ {}:{}", __FILE__, __LINE__)

//////////////////////////////////////////////////////////////////////////


#if _MSC_VER >= 1600
	#define QSTATIC_ASSERT(exp) static_assert(exp, #exp)
#endif

#ifndef QSTATIC_ASSERT
	#define QSTATIC_ASSERT(exp) typedef char __QCOMPILE_ASSERT__[(exp)?1:-1] __attribute__ ((unused))
#endif

#if defined Q_WINDOWS
#	define QBREAK() __debugbreak()
#elif defined Q_POSIX_API
#   if defined NDEBUG
#       define QBREAK()
#   else
#       define QBREAK() do { if (q::volatile_true()) { raise(SIGTRAP); } } while (q::volatile_false())
#   endif
#else
#	define QBREAK() assert(0)
#endif

 
//
///////////////////////////////////////////////////////////////////////////////


