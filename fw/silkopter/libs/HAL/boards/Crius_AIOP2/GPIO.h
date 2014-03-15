#pragma once

namespace hal
{
	
class GPIO
{
public:
	GPIO();
	
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
	
    void    set_pin_mode(uint8_t pin, Mode mode);
    //int8_t  analogPinToDigitalPin(uint8_t pin);

    bool	read(uint8_t pin);
    void    write(uint8_t pin, bool value);
    void    toggle(uint8_t pin);

    /* return true if USB cable is connected */
    bool    is_usb_connected() const;
private:
};

extern GPIO gpio;

}