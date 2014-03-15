#pragma once

namespace hal
{
	
class RC_Outputs
{
public:
	RC_Outputs();
	
	static const uint8_t MAX_CHANNEL_COUNT = 8;

	//sets the value of channel ch
	void set_channel(uint8_t ch, int16_t val);
	
	//sets the value for all channels up to min(size, get_channel_count())
	void set_channels(const int16_t* src, uint8_t size);
	
	void set_all_channels(int16_t val);
	
	//sets the frequency of a channel in hertz
	void set_frequency(uint8_t ch, uint16_t hz);

	//sets the frequency for all channels up to min(size, get_channel_count())
	void set_frequencies(const uint16_t* hz, uint8_t size);
	void set_frequencies(uint16_t hz);
	
private:
};

extern RC_Outputs rc_outputs;

}