#pragma once

namespace hal
{
	
class RC_Inputs
{
public:
	RC_Inputs();
	
	static const uint8_t MAX_CHANNEL_COUNT = 8;
	
	//channel values go from 0 to 1024!!!
	//It's stored as a signed integer to avoid wrap when doing math
	typedef int16_t Value;
	
	//returns the value of channel ch
	Value get_channel(uint8_t ch) const;
	
	//returns the value of all channels up to min(size, get_channel_count())
	void get_all_channels(Value* dst, uint8_t size) const;
	
private:
};

extern RC_Inputs rc_inputs;

}