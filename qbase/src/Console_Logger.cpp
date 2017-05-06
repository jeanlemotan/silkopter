#include "QBaseStdAfx.h"
#include "QBase.h"

#if defined(Q_ANDROID)
#	include <android/log.h>
#elif defined(Q_WINDOWS)
#	include <windows.h>
#endif


using namespace q;
using namespace logging;

void Console_Logger::log(Level level, const std::string& message)
{
	(void)level;

#if defined(Q_ANDROID)

    switch (level)
    {
    case Level::DBG:		__android_log_write(ANDROID_LOG_DEBUG, "", message.c_str()); break;
    case Level::INFO:		__android_log_write(ANDROID_LOG_INFO, "", message.c_str()); break;
    case Level::WARNING:	__android_log_write(ANDROID_LOG_WARN, "", message.c_str()); break;
    case Level::ERR:		__android_log_write(ANDROID_LOG_ERROR, "", message.c_str()); break;
    default:				__android_log_write(ANDROID_LOG_VERBOSE, "", message.c_str()); break;
    }

#elif defined(Q_WINDOWS) || defined(Q_IOS) || defined(Q_POSIX_API)

	puts(message.c_str());
	fflush(stdout);

#else
#	error define a print for your platform
#endif
}


