#include "Config.h"

#if BOARD_TYPE == SIM

#include "board/board.h"
#include "debug/debug.h"
#include <_qmath.h>
#include "util/Scope_Sync.h"

namespace board
{
namespace scheduler
{

static bool s_is_initialized = false;
static Frequency s_frequency;

void init(Frequency freq)
{
	if (s_is_initialized)
	{
		return;
	}
	s_is_initialized = true;
	clock::init();
}

void register_callback(Callback cb, void* user_data)
{
}

void stop() 
{
}

bool is_in_callback() 
{
	return false;
}

Frequency get_frequency()
{
	return s_frequency;	
}
uint16_t get_frequency_hz()
{
	return static_cast<uint16_t>(s_frequency);
}

}
}

#endif
