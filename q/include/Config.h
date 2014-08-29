#pragma once
	
//#define Q_USE_EXTERNAL_ALLOC
//#define Q_USE_NED_ALLOC
//#define Q_THREADING

#ifdef _WIN32
#	define Q_WINDOWS
#	ifndef NOMINMAX
#		define NOMINMAX //to avoid clashing with glm
#	endif
#	if defined(Q_POSIX_API)
#		undef Q_POSIX_API
#	endif
#endif

#ifdef ANDROID
#	define Q_ANDROID
#	define NOMINMAX //to avoid clashing with glm
#	define Q_POSIX_API
#endif


#if defined(__APPLE__) || defined(MACOSX)
#	define Q_IOS
#	define Q_POSIX_API
#endif

