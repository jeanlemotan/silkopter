#pragma once

#if defined _MSC_VER
#	pragma warning(disable: 4201) //disable the nameless struct/union warning on W4
#	pragma warning(disable: 4324) //disable the padding warning on W4
#	pragma warning(disable: 4127) //disable the constant expression in conditional that pops up in the assert macro
#endif

#include "QBaseStdAfx.h"
#include "Platform.h"
#include "Types.h"
#include "util/Noncopyable.h"
#include "FString.h"
#include "Debug.h"
#include "mem/auto_buffer.h"
#include "util/Format.h"
#include "util/Util.h"
#include "util/Flag_Set.h"
#include "Log.h"
#include "Logger.h"
#include "Console_Logger.h"
#include "mem/FSBAllocator.hh"
#include "mem/Embedded_Allocator.h"
#include "util/murmurhash.h"
#include "util/Circular_Buffer.h"
#include "util/crc.h"
#include "util/Encoder.h"
#include "util/File_System_Utils.h"
#include "util/Hash.h"
#include "util/Tween.h"
#include "util/Delayer.h"
#include "util/Util.h"
#include "util/Iterators.h"
#include "util/Signals.h"
