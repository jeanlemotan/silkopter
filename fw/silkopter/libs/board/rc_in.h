#pragma once

namespace board
{
namespace rc_in
{
	extern void init();
	
	extern uint8_t get_channel_count();
	
	//returns the value of channel ch
	extern int16_t get_channel(uint8_t ch);
	
	//returns the value of all channels up to min(size, get_channel_count())
	extern void get_channels(int16_t* dst, uint8_t size);
}
}