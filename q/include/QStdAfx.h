#ifndef _CRT_SECURE_NO_WARNINGS
#	define _CRT_SECURE_NO_WARNINGS
#endif

#ifndef _SCL_SECURE_NO_WARNINGS
#	define _SCL_SECURE_NO_WARNINGS
#endif

//android ndk needed this
#define _GLIBCXX_USE_C99_MATH 1

#include "Config.h"

#include <unordered_map>
#include <deque>
#include <set>
#include <array>

//android ndk needed this
//#define BOOST_NO_INTRINSIC_WCHAR_T
//#define BOOST_SP_DISABLE_THREADS //was ok, but not anymore. use atomic increment ios patch instead
//#define BOOST_NO_TYPEID

#include <boost/optional.hpp>

#include "QBase.h"
#include "QData.h"
#include "_qmath.h"



