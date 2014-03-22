#pragma once

namespace board
{	
namespace pwm_out
{
	extern void init();
	
	extern uint8_t get_channel_count();

	//sets the value of channel ch
	extern void set_channel(uint8_t ch, int16_t val);
	//returns the value of channel ch
	extern int16_t get_channel(uint8_t ch);

	//sets the value for all channels up to min(size, get_channel_count())
	extern void set_channels(const int16_t* src, uint8_t size);
	//returns the value of all channels up to min(size, get_channel_count())
	extern void get_channels(int16_t* dst, uint8_t size);
	
	extern void set_all_channels(int16_t val);
	
	//sets the frequency of a channel in hertz
	extern void set_frequency(uint8_t ch, uint16_t hz);

	//sets the frequency for all channels up to min(size, get_channel_count())
	extern void set_frequencies(const uint16_t* hz, uint8_t size);
	extern void set_frequencies(uint16_t hz);
	
	extern void set_all_enabled(bool enabled);
	extern void set_enabled(uint8_t ch, bool enabled);
}
}
