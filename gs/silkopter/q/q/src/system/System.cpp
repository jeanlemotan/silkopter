#include "QStdAfx.h"
#include "Config.h"
#include "system/System.h"
#include <stdio.h>
#include <stdarg.h>

using namespace q;

#if defined (Q_WINDOWS)
#	pragma comment(lib, "winmm.lib")
#endif

#if defined Q_WINDOWS
#	define NOMINMAX
#	define NOWINRES
#	define NOMCX
#	include <windows.h>
#	include <process.h>
#elif defined Q_POSIX_API
#	define NOMINMAX
#	include <pthread.h>
#	include <signal.h>
#	include <sched.h>
#	include <unistd.h>
#endif

//////////////////////////////////////////////////////////////////////////

System::System()
	: m_file_system(new data::File_System)
	, m_factory(m_file_system)
{
#if defined(Q_WINDOWS)
	timeBeginPeriod(1);
#endif
}

//////////////////////////////////////////////////////////////////////////

System::~System()
{
#if defined(Q_WINDOWS)
	timeEndPeriod(1);
#endif

	m_renderer.reset();
}

//////////////////////////////////////////////////////////////////////////

void System::init(video::Renderer_ptr renderer)
{
	QASSERT(renderer);
	m_renderer = renderer;
//	thread::Scheduler::inst().init();
}

//////////////////////////////////////////////////////////////////////////

