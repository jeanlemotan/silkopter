#include "QBaseStdAfx.h"
#include "QBase.h"

#include <set>

///////////////////////////////////////////////////////////////////////////////

#pragma warning( disable : 4996)

///////////////////////////////////////////////////////////////////////////////

namespace q
{
namespace debug
{
namespace detail
{

	static bool defaultHandler(const char* /*condition*/, const char* file, int line, const char* msg)
	{
		log(logging::Level::ERR, "Assert", file, line, msg ? String(msg) : String::null);
		return true;
	}

	static q::debug::Assert_handler sHandler = &defaultHandler;
}
}
}

///////////////////////////////////////////////////////////////////////////////

void q::debug::set_assert_handler(q::debug::Assert_handler h)
{
	q::debug::detail::sHandler = h;
}

///////////////////////////////////////////////////////////////////////////////

bool q::debug::detail::call_handler(const char* condition, const char* file, int line, const char* msg)
{
	return sHandler ? sHandler(condition, file, line, msg) : true;
}

///////////////////////////////////////////////////////////////////////////////

namespace q
{
namespace debug
{
namespace detail
{

	//the const char* is built using the __FILE__ macro so it's safe to compare the pointer
	typedef std::pair<const char*, int> AssertKey;

	struct AssertKeyCompare : std::binary_function <AssertKey, AssertKey, bool> 
	{
		bool operator() (const AssertKey& x, const AssertKey& y) const 
		{
			return x.first < y.first || (x.first == y.first && x.second < y.second);
		}
	};

	static std::set<AssertKey, AssertKeyCompare> sDisabledAsserts;

}
}
}

bool q::debug::detail::is_assert_enabled(const char* file, int line)
{
	QASSERT(file);
	if (!file)
	{
		return false;
	}
	auto key = std::make_pair(file, line);
	return sDisabledAsserts.find(key) == sDisabledAsserts.end();
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
		sDisabledAsserts.erase(key);
	}
	else
	{
		sDisabledAsserts.insert(key);
	}
}


