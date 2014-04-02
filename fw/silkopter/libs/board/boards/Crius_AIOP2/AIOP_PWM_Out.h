#pragma once

#include "board/PWM_Out.h"

namespace board
{

class AIOP_PWM_Out : public PWM_Out
{
public:
	AIOP_PWM_Out();
	
	uint8_t get_count() const;
	void set_data(uint8_t channel, Data const& data);
	void set_data_for_all(Data const& data);
	void set_enabled(uint8_t channel, bool enabled);
	void set_enabled_for_all(bool enabled);
		
	void set_frequency(uint8_t ch, uint16_t hz);
	void set_frequency_for_all(uint16_t hz);
	
private:
};
	
}

