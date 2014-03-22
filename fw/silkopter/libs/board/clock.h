#pragma once

#include "util/chrono.h"

namespace board
{
namespace clock
{
	extern void init();
	
	extern chrono::time_ms now_ms();
	extern chrono::time_us now_us();
	
	extern void delay(chrono::micros us);
	extern void delay(chrono::millis ms);
}
}
