#pragma once

#include "board/clock.h"

namespace board
{
namespace scheduler
{
	enum class Frequency
	{
		_250_HZ		= 250,
		_500_HZ		= 500,
		_1000_HZ	= 1000,
		_2000_HZ	= 2000,
	};
	extern Frequency get_frequency();
	extern uint16_t get_frequency_hz();
	
    extern void init(Frequency freq);

    extern void stop();
//     extern void resume();
//    extern bool	is_in_callback();

	static const uint8_t MAX_CALLBACK_COUNT = 4;
	typedef void(*Callback)();
    extern void register_callback(Callback cb);

}
} 

