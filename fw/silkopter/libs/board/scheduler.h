#pragma once

#include "board/clock.h"

namespace board
{
namespace scheduler
{
    extern void init();

    extern void stop();
//     extern void resume();
//    extern bool	is_in_callback();

	static const uint8_t MAX_CALLBACK_COUNT = 4;
	typedef void(*Callback)();
    extern void register_callback(Callback cb);

    extern void set_callback_frequency(uint16_t timer_hz);
	extern uint16_t get_callback_frequency();
}
} 

