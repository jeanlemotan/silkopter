#pragma once

namespace hal
{
	
class Clock
{
public:
	Clock();
	
	uint32_t millis();
	uint32_t micros();
	
	void delay_micros(uint16_t us);
	void delay_millis(uint16_t ms);
	
private:
};

extern Clock clock;

}