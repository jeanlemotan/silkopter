#pragma once

namespace board
{
namespace rc_in
{
	extern void init();
	
	static const uint8_t MAX_CHANNEL_COUNT = 8;
	
	//returns the value of channel ch
	extern int16_t get_channel(uint8_t ch);
	
	//returns the value of all channels up to min(size, get_channel_count())
	extern void get_channels(int16_t* dst, uint8_t size);
}
}