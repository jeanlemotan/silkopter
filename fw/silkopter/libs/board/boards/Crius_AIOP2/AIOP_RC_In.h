#pragma once

#include "board/RC_In.h"

namespace board
{

class AIOP_RC_In : public RC_In
{
public:
	AIOP_RC_In();

	void init();
	
	uint8_t get_count() const;
	bool get_data(uint8_t channel, Data& data) const;
		
private:
	bool m_is_initialized;

	static void A8_ppm_isr();
	
	static const uint8_t MAX_CHANNEL_COUNT = 8;

	static volatile int16_t s_pulses[MAX_CHANNEL_COUNT];
	static volatile int16_t s_raw_data[MAX_CHANNEL_COUNT];
	static volatile uint16_t s_edge_time[MAX_CHANNEL_COUNT];
};
	
}

