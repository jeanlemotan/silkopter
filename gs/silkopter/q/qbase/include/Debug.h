#pragma once

///////////////////////////////////////////////////////////////////////////////
//

namespace q
{
	namespace debug
	{
		//The assert handler is called whenever an assert fails and shoud present a message to the user.
		//If it returns true, the code will break. If false, the code will fall through and continue
		// 	condition contains a stringified condition that failed
		//	msg is any user supplied message. Can be NULL
		//	file is the name of the file the assert failed
		//	line is the line where the assert failed
		typedef	bool (*Assert_handler)(const char* condition, const char* file, int line, const char* msg);
		void set_assert_handler(Assert_handler);


		///////////////////////////////////////////////////////////////////////
		//for internal use only
		namespace detail
		{
			//this is responsible of formatting the messages and calling the handler
			bool call_handler(const char* condition, const char* file, int line, const char* msg);


			//adapter for when the message is missing
			inline bool call_handler(const char* condition, const char* file, int line)
			{
				return call_handler(condition, file, line, (const char*)nullptr);
			}

			//adapter for any std-like string class
			template <class String>
			inline bool call_handler(const char* condition, const char* file, int line, const String& msg)
			{
				return call_handler(condition, file, line, msg.c_str());
			}

			//uset to mark/test once asserts as being taken or not
			bool is_assert_enabled(const char* file, int line);
			void set_assert_enabled(const char* file, int line, bool enabled);
		}
	}
}

#ifdef Q_WINDOWS
	#define QBREAK() __debugbreak() //better than assert(0) as it will break on this exact line instead of deep in the assert function.
#else 
	#define QBREAK() assert(0)
#endif 

#define QUNUSED(x) (void)sizeof(x)

#ifdef _DEBUG

	#define QASSERT(condition)																\
		do 																					\
		{  																					\
			if (!(condition))																\
			{																				\
				if (q::debug::detail::call_handler(#condition, __FILE__, __LINE__) == true)	\
				{																			\
					QBREAK(); 																\
				}																			\
			}																				\
		} while(0)

	#define QASSERT_MSG(condition, fmt, ...)											\
		do 																				\
		{																				\
			if (!(condition)) 															\
			{																			\
				q::String _msg__;														\
				q::util::format(_msg__, fmt, ##__VA_ARGS__);							\
				if (q::debug::detail::call_handler(#condition, __FILE__, __LINE__, _msg__) == true)	\
				{																		\
					QBREAK();															\
				}																		\
			}																			\
		} while(0)

	#define QASSERT_ONCE(condition)														\
		do 																				\
		{																				\
			/* checking the condition first and then the GetAssertOnce, as chances are that the condition is cheaper */ \
			if (!(condition) && q::debug::detail::is_assert_enabled(__FILE__, __LINE__)) 	\
			{																			\
				q::debug::detail::set_assert_enabled(__FILE__, __LINE__, false);			\
				if (q::debug::detail::call_handler(#condition, __FILE__, __LINE__) == true)\
				{																		\
					QBREAK();															\
				}																		\
			}																			\
		} while(0)

	#define QASSERT_MSG_ONCE(condition, fmt, ...)										\
		do 																				\
		{																				\
			/* checking the condition first and then the GetAssertOnce, as chances are that the condition is cheaper */ \
			if (!(condition) && q::debug::detail::is_assert_enabled(__FILE__, __LINE__)) 	\
			{																			\
				q::debug::detail::set_assert_enabled(__FILE__, __LINE__, false);				\
				q::String _msg__;														\
				q::util::format(_msg__, fmt, ##__VA_ARGS__);							\
				if (q::debug::detail::call_handler(#condition, __FILE__, __LINE__, _msg__) == true)	\
				{																		\
					QBREAK();															\
				}																		\
			}																			\
		} while(0)

#else

	#define QASSERT(condition) QUNUSED(condition)
	#define QASSERT_MSG(condition, fmt, ...) do { QUNUSED(condition); QUNUSED(fmt); } while(0)	

	#define QASSERT_ONCE(condition) QUNUSED(condition)
	#define QASSERT_MSG_ONCE(condition, fmt, ...) do { QUNUSED(condition); QUNUSED(fmt); } while(0)	

#endif


#if _MSC_VER >= 1600
	#define JCOMPILE_ASSERT(exp) static_assert(exp, #exp)
#endif

#ifndef JCOMPILE_ASSERT
	#define JCOMPILE_ASSERT(exp) typedef char __JCOMPILE_ASSERT__[(exp)?1:-1]
#endif
 
//
///////////////////////////////////////////////////////////////////////////////


