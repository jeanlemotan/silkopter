 #include "QBaseStdAfx.h"
#include "QBase.h"

#if defined Q_WINDOWS
#	define NOMINMAX
#	define NOWINRES
#	define NOMCX
#	include <windows.h>
#	include <process.h>
#elif defined Q_POSIX_API
#	include <pthread.h>
#	include <signal.h>
#	include <sched.h>
#	include <unistd.h>
#	if defined(Q_LINUX)
#		include "sys/time.h"
#	endif
#endif

using namespace q;

Clock::time_point Clock::now()
{
#if defined(Q_WINDOWS)

	LARGE_INTEGER freq;
	LARGE_INTEGER crt;
	QueryPerformanceFrequency(&freq);
	QueryPerformanceCounter(&crt);
	return time_point(duration(crt.QuadPart * static_cast<rep>(period::den) / freq.QuadPart));

#elif defined(Q_POSIX_API)

	struct timeval ptv;
	gettimeofday(&ptv, NULL);
	return time_point(duration(ptv.tv_usec + ptv.tv_sec * static_cast<rep>(period::den)));

#else
#	error "Define for your platform"
#endif
}

time_t Clock::to_time_t(time_point tp)
{
	time_t t = tp.time_since_epoch().count();
	return t;

}
Clock::time_point Clock::from_time_t(time_t t)
{
	return time_point(duration(t));
}

