#include <avr/io.h>
#include <avr/interrupt.h>
#include "qmath.h"
#include "HAL/boards/Crius_AIOP2/RC_Outputs.h"
#include "HAL/boards/Crius_AIOP2/GPIO.h"

#if BOARD_TYPE == CRIUS_AIOP2

using namespace hal;

namespace hal
{
	RC_Outputs rc_outputs;
}

static const uint16_t MIN_PULSE_WIDTH = 1800; // 900
static const uint16_t MAX_PULSE_WIDTH = 4200; // 2100
static const uint16_t PULSE_RANGE = (MAX_PULSE_WIDTH - MIN_PULSE_WIDTH);
static const uint16_t MAX_VALUE = 1024;
//static const uint16_t VALUE_MULTIPLIER = MAX_VALUE / PULSE_RANGE;

RC_Outputs::RC_Outputs()
{
	// --------------------- TIMER1: CH_10, CH_11 ---------------
	hal::gpio.set_pin_mode(11, GPIO::Mode::OUTPUT); // CH_10 (PB5/OC1A)
	hal::gpio.set_pin_mode(12, GPIO::Mode::OUTPUT); // CH_11 (PB6/OC1B)
	// WGM: 1 1 1 0. Clear Timer on Compare, TOP is ICR1.
	// CS11: prescale by 8 => 0.5us tick
	TCCR1A = (1<<WGM11);
	TCCR1B = (1<<WGM13)|(1<<WGM12)|(1<<CS11);
	OCR1A = 0xFFFF;
	OCR1B = 0xFFFF;
	ICR1 = 40000; //50hz freq...Datasheet says  (system_freq/prescaler)/target frequency. So (16000000hz/8)/50hz=40000,
  
	// --------------------- TIMER3: CH_1, CH_3, and CH_4 ---------------------
	hal::gpio.set_pin_mode(5, GPIO::Mode::OUTPUT); // CH_1 (PE3/OC3A)
	hal::gpio.set_pin_mode(2, GPIO::Mode::OUTPUT); // CH_3 (PE4/OC3B)
	hal::gpio.set_pin_mode(3, GPIO::Mode::OUTPUT); // CH_4 (PE5/OC3C)

	// WGM: 1 1 1 0. Clear timer on Compare, TOP is ICR3
	// CS31: prescale by 8 => 0.5us tick
	TCCR3A =((1<<WGM31));
	TCCR3B = (1<<WGM33)|(1<<WGM32)|(1<<CS31);
	OCR3A = 0xFFFF; // Init OCR registers to nil output signal
	OCR3B = 0xFFFF;
	OCR3C = 0xFFFF;
	ICR3 = 40000; // 0.5us tick => 50hz freq

	// --------------------- TIMER4: CH_7, CH_8, and CH_11 ---------------------
	hal::gpio.set_pin_mode(6, GPIO::Mode::OUTPUT); // CH_2 (PE3/OC4A)
	hal::gpio.set_pin_mode(7, GPIO::Mode::OUTPUT); // CH_7 (PE4/OC4B)
	hal::gpio.set_pin_mode(8, GPIO::Mode::OUTPUT); // CH_8 (PE5/OC4C)

	// WGM: 1 1 1 0. Clear timer on Compare, TOP is ICR3
	// CS31: prescale by 8 => 0.5us tick
	TCCR4A =((1<<WGM41));
	TCCR4B = (1<<WGM43)|(1<<WGM42)|(1<<CS41);
	OCR4A = 0xFFFF; // Init OCR registers to nil output signal
	OCR4B = 0xFFFF;
	OCR4C = 0xFFFF;
	ICR4 = 40000; // 0.5us tick => 50hz freq		TCCR3A |= (1<<COM3A1); // CH_1
	TCCR4A |= (1<<COM4A1); // CH_2
	TCCR3A |= (1<<COM3B1); // CH_3
	TCCR3A |= (1<<COM3C1); // CH_4
	TCCR4A |= (1<<COM4B1); // CH_7
	TCCR4A |= (1<<COM4C1); // CH_8
	TCCR1A |= (1<<COM1A1); // CH_10
	TCCR1A |= (1<<COM1B1); // CH_11  //  set_all_channels(0);}

#define timer_period(hz) (2000000UL / (hz))

void RC_Outputs::set_frequency(uint8_t ch, uint16_t hz)
{
	uint16_t icr = timer_period(hz);
	if (ch == 10 || ch == 11) 
	{
		ICR1 = icr;
	}
	if (ch == 1 || ch == 3 || ch == 4)
	{
		ICR3 = icr;
	}
	if (ch == 2 || ch == 7 || ch == 8)
	{
		ICR4 = icr;
	}
}
void RC_Outputs::set_frequencies(const uint16_t* hz, uint8_t size)
{
	for (uint8_t i = 0; i < size; i++)
	{
		set_frequency(i, hz[i]);
	}
}
void RC_Outputs::set_frequencies(uint16_t hz)
{
	for (uint8_t i = 0; i < MAX_CHANNEL_COUNT; i++)
	{
		set_frequency(i, hz);
	}
}
void RC_Outputs::set_channel(uint8_t ch, int16_t val) 
{
	val = math::clamp(val, int16_t(0), int16_t(MAX_VALUE));
	val = (val * PULSE_RANGE >> 10) + MIN_PULSE_WIDTH;
	
	uint16_t pwm = val << 1;
 
	switch(ch)
	{
		case 0:  OCR3A = pwm; break; //5
		case 1:  OCR4A = pwm; break; //6
		case 2:  OCR3B = pwm; break; //2
		case 3:  OCR3C = pwm; break; //3
		case 6:  OCR4B = pwm; break; //7
		case 7:  OCR4C = pwm; break; //8
		case 9:  OCR1A = pwm; break;// d11
		case 10: OCR1B = pwm; break;// d12
	}
} 

void RC_Outputs::set_all_channels(int16_t val)
{
	val = math::clamp(val, int16_t(0), int16_t(MAX_VALUE));
	val = (uint32_t(val) * uint32_t(PULSE_RANGE) >> 10) + MIN_PULSE_WIDTH;
	
	uint16_t pwm = val << 1;
	
	OCR3A = pwm;//5
	OCR4A = pwm;//6
	OCR3B = pwm;//2
	OCR3C = pwm;//3
	OCR4B = pwm;//7
	OCR4C = pwm;//8
	OCR1A = pwm;// d11
	OCR1B = pwm;// d12
}


#endif
