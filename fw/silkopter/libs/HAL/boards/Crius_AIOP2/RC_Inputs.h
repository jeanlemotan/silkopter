#pragma once

namespace hal
{
	
class RC_Inputs
{
public:
	RC_Inputs();
	
	static const uint8_t MAX_CHANNEL_COUNT = 8;
	
	//returns the value of channel ch
	int16_t get_channel(uint8_t ch) const;
	
	//returns the value of all channels up to min(size, get_channel_count())
	void get_channels(int16_t* dst, uint8_t size) const;
	
private:
};

extern RC_Inputs rc_inputs;

}