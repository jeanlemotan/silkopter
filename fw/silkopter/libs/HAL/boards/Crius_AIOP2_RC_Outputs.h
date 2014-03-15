#pragma once

namespace hal
{
	
class RC_Outputs
{
public:
	RC_Outputs();
	
	static const uint8_t MAX_CHANNEL_COUNT = 8;

	//channel values go from 0 to 1024!!!
	//It's stored as a signed integer to avoid wrap when doing math
	typedef int16_t Value;
	
	//sets the value of channel ch
	Value set_channel(uint8_t ch) const;
	
	//sets the value for all channels up to min(size, get_channel_count())
	void set_channels(const Value* src, uint8_t size) const;
	
	//sets the frequency of a channel in hertz
	void set_frequency(uint8_t ch, uint16_t hz);

	//sets the frequency for all channels up to min(size, get_channel_count())
	void set_frequencies(const uint16_t* hz, uint8_t size);
	
private:
};

extern RC_Outputs rc_outputs;

}