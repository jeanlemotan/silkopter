#pragma once

namespace board
{
namespace gpio
{
	extern void init();
	
	enum class Mode
	{
		INPUT,
		OUTPUT
	};
	enum class Interrupt_Mode
	{
		LOW,
		HIGH,
		FALLING,
		RISING
	};
	
    extern void set_pin_mode(uint8_t pin, Mode mode);
    extern bool	read(uint8_t pin);
    extern void write(uint8_t pin, bool value);
    extern void toggle(uint8_t pin);
    extern bool is_usb_connected();
}
}
