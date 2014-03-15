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
	
private:
};

extern Clock clock;

}