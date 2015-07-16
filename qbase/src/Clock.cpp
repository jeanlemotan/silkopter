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

namespace q
{

static Clock::rep get_now()
{
#if defined(Q_WINDOWS)

    LARGE_INTEGER freq;
    LARGE_INTEGER crt;
    QueryPerformanceFrequency(&freq);
    QueryPerformanceCounter(&crt);
    return Clock::rep(crt.QuadPart * static_cast<Clock::rep>(Clock::period::den) / freq.QuadPart);

#elif defined(Q_POSIX_API)

    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return Clock::rep(ts.tv_nsec + ts.tv_sec * static_cast<Clock::rep>(Clock::period::den));

#else
#	error "Define for your platform"
#endif
}

static Clock::rep s_start_time_point = get_now();


Clock::time_point Clock::now()
{
    Clock::rep now = get_now();
    return time_point(duration(now - s_start_time_point));
}

time_t Clock::to_time_t(time_point tp)
{
    time_t t = std::chrono::duration_cast<std::chrono::seconds>(tp.time_since_epoch()).count();
	return t;

}
Clock::time_point Clock::from_time_t(time_t t)
{
    return time_point(std::chrono::duration_cast<duration>(std::chrono::seconds(t)));
}

}
