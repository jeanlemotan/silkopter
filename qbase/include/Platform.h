#pragma once

#if defined(_WIN32)
#	define Q_WINDOWS
#	if defined(Q_POSIX_API)
#		undef Q_POSIX_API
#	endif
#elif defined(ANDROID)
#	define Q_ANDROID
#	define Q_POSIX_API
#elif defined(__APPLE__) || defined(MACOSX)
#	define Q_IOS
#	define Q_POSIX_API
#elif defined(RASPBERRY_PI)
#	define Q_RASPBERRY_PI
#	define Q_POSIX_API
#elif defined (__linux)
#	define Q_LINUX
#	define Q_POSIX_API
#endif
