#pragma once

#if defined _WIN32 || defined _WIN64
#	pragma warning(disable: 4201) //disable the nameless struct/union warning on W4
#	pragma warning(disable: 4324) //disable the padding warning on W4
#	pragma warning(disable: 4127) //disable the constant expression in conditional that pops up in the assert macro
#endif

#include "QBaseStdAfx.h"

#include "Platform.h"

#if !defined Q_AVR
#	include "Types.h"
#	include "Clock.h"
#endif

#include "util/Noncopyable.h"
#include "FString.h"
#include "Debug.h"

#if !defined Q_AVR
#	include "mem/auto_buffer.h"
#endif

#include "util/Format.h"
#include "util/Util.h"
#include "util/Flag_Set.h"

#if !defined Q_AVR
#	include "Interned_String.h"
#endif

#include "Log.h"

#if !defined Q_AVR
#	include "Logger.h"
#	include "Console_Logger.h"
#	include "mem/FSBAllocator.hh"
#	include "mem/Embedded_Allocator.h"
#endif

#include "util/murmurhash.h"
#include "util/Circular_Buffer.h"
#include "util/crc.h"

#if !defined Q_AVR
#	include "util/qrtti.h"
#	include "util/Singleton.h"
#	include "util/Class_Factory.h"
#endif

#include "util/Encoder.h"

#if !defined Q_AVR
#	include "util/File_System_Utils.h"
#endif

#include "util/Hash.h"
#include "util/Rand.h"
#include "util/Tween.h"
#include "util/Delayer.h"
#include "util/Util.h"
#include "util/Iterators.h"

#if !defined Q_AVR
#	include "util/Signals.h"
#	include "thread/Task.h"
#	include "thread/Worker_Thread.h"
#	include "thread/Scheduler.h"
#endif



