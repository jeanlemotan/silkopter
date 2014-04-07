#include "Config.h"

#if BOARD_TYPE == SIM

#include "debug/debug.h"
#include "board/board.h"
#include "util/Scope_Sync.h"
#include <chrono>
#include <thread>

namespace board
{
namespace clock
{

static bool s_is_initialized = false;
std::chrono::high_resolution_clock::time_point s_start;

void init() 
{
	if (s_is_initialized)
	{
		return;
	}
	s_is_initialized = true;

	s_start = std::chrono::high_resolution_clock::now();
}

chrono::time_us now_us()
{
	ASSERT(s_is_initialized);

	auto us = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - s_start);
	return chrono::time_us(us.count());
}

chrono::time_ms now_ms() 
{
	auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - s_start);
	return chrono::time_ms(ms.count());
}

void delay(chrono::micros _us)
{
	std::this_thread::sleep_for(std::chrono::microseconds(_us.count));
}

void delay(chrono::millis _ms)
{
	std::this_thread::sleep_for(std::chrono::milliseconds(_ms.count));
}


}
}
#endif
