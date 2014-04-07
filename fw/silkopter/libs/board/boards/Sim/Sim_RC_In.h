#pragma once

#include "board/RC_In.h"

namespace board
{

class Sim_RC_In : public RC_In
{
public:
	Sim_RC_In();

	void init();
	
	uint8_t get_count() const;
	bool get_data(uint8_t channel, Data& data) const;
		
private:
	bool m_is_initialized = false;

	static const uint8_t MAX_CHANNEL_COUNT = 8;
};
	
}

