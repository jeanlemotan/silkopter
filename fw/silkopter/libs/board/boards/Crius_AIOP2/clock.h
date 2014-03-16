#pragma once

namespace hal
{
namespace clock
{
	extern void init();
	
	extern uint32_t millis();
	extern uint32_t micros();
	
	extern void delay_micros(uint16_t us);
	extern void delay_millis(uint16_t ms);
}
}
