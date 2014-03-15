#pragma once

namespace hal
{
	
class RC_Inputs
{
public:
	RC_Inputs();
	
	static const uint8_t RC_INPUTS_MAX_CHANNEL_COUNT = 8;
	static const uint8_t RC_INPUTS_MIN_CHANNEL_COUNT = 5;     // for ppm sum we allow less than 8 channels to make up a valid packet

	uint8_t get_channel_count() const;
	
	//channel values go from 0 to 65535
	
	//returns the value of channel ch
	uint16_t get_channel(uint8_t ch) const;
	
	//returns the value of all channels up to min(size, get_channel_count())
	void get_all_channels(uint16_t* dst, uint8_t size) const;
	
private:
};

extern RC_Inputs rc_inputs;

}