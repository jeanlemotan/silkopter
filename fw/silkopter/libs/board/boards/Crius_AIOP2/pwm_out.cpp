#include "Config.h"

#if BOARD_TYPE == CRIUS_AIOP2

#include <avr/io.h>
#include <avr/interrupt.h>
#include "qmath.h"
#include "debug/assert.h"
#include "board/boards/Crius_AIOP2/pwm_out.h"
#include "board/boards/Crius_AIOP2/gpio.h"

namespace board
{
namespace pwm_out
{

static const uint16_t MIN_PULSE_WIDTH = 1800; // 900
static const uint16_t MAX_PULSE_WIDTH = 4200; // 2100
static const uint16_t PULSE_RANGE = (MAX_PULSE_WIDTH - MIN_PULSE_WIDTH);
static bool s_is_initialized = false;

void init()
{
	if (s_is_initialized)
	{
		return;
	}
	s_is_initialized = true;

	gpio::init();
	
	// --------------------- TIMER3: CH_0, CH_1, and CH_2 ---------------------
	gpio::set_pin_mode(2, gpio::Mode::OUTPUT); // CH_0 (PE4/OC3B)
	gpio::set_pin_mode(3, gpio::Mode::OUTPUT); // CH_1 (PE5/OC3C)
	gpio::set_pin_mode(5, gpio::Mode::OUTPUT); // CH_2 (PE3/OC3A)

	// WGM: 1 1 1 0. Clear timer on Compare, TOP is ICR3
	// CS31: prescale by 8 => 0.5us tick
	TCCR3A = (1<<WGM31);
	TCCR3B = (1<<WGM33) | (1<<WGM32) | (1<<CS31);
	OCR3A = 0xFFFF; // Init OCR registers to nil output signal
	OCR3B = 0xFFFF;
	OCR3C = 0xFFFF;
	ICR3 = 40000; // 0.5us tick => 50hz freq

	// --------------------- TIMER4: CH_3, CH_4, and CH_5 ---------------------
	gpio::set_pin_mode(6, gpio::Mode::OUTPUT); // CH_3 (PE3/OC4A)
	gpio::set_pin_mode(7, gpio::Mode::OUTPUT); // CH_4 (PE4/OC4B)
	gpio::set_pin_mode(8, gpio::Mode::OUTPUT); // CH_5 (PE5/OC4C)

	// WGM: 1 1 1 0. Clear timer on Compare, TOP is ICR3
	// CS31: prescale by 8 => 0.5us tick
	TCCR4A = (1<<WGM41);
	TCCR4B = (1<<WGM43) | (1<<WGM42) | (1<<CS41);
	OCR4A = 0xFFFF; // Init OCR registers to nil output signal
	OCR4B = 0xFFFF;
	OCR4C = 0xFFFF;
	ICR4 = 40000; // 0.5us tick => 50hz freq
	// --------------------- TIMER1: CH_6, CH_7 ---------------
	gpio::set_pin_mode(11, gpio::Mode::OUTPUT); // CH_6 (PB5/OC1A)
	gpio::set_pin_mode(12, gpio::Mode::OUTPUT); // CH_7 (PB6/OC1B)
	// WGM: 1 1 1 0. Clear Timer on Compare, TOP is ICR1.
	// CS11: prescale by 8 => 0.5us tick
	TCCR1A = (1<<WGM11);
	TCCR1B = (1<<WGM13)|(1<<WGM12)|(1<<CS11);
	OCR1A = 0xFFFF;
	OCR1B = 0xFFFF;
	ICR1 = 40000; //50hz freq...Datasheet says  (system_freq/prescaler)/target frequency. So (16000000hz/8)/50hz=40000,
}

#define timer_period(hz) (2000000UL / (hz))

void set_frequency(uint8_t ch, uint16_t hz)
{
	ASSERT(s_is_initialized);
	
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
void set_frequencies(const uint16_t* hz, uint8_t size)
{
	ASSERT(s_is_initialized);

	for (uint8_t i = 0; i < size; i++)
	{
		set_frequency(i, hz[i]);
	}
}
void set_frequencies(uint16_t hz)
{
	ASSERT(s_is_initialized);

	for (uint8_t i = 0; i < MAX_CHANNEL_COUNT; i++)
	{
		set_frequency(i, hz);
	}
}
void set_channel(uint8_t ch, int16_t val) 
{
	ASSERT(s_is_initialized);

	val = math::clamp(val, int16_t(0), int16_t(1024));
	val = (uint32_t(val) * uint32_t(PULSE_RANGE) >> 10);
	uint16_t pwm = val + MIN_PULSE_WIDTH;
 
	switch(ch)
	{
		case 0: OCR3B = pwm; break; //2
		case 1: OCR3C = pwm; break; //3
		case 2: OCR3A = pwm; break; //5
		case 3: OCR4A = pwm; break; //6
		case 4: OCR4B = pwm; break; //7
		case 5: OCR4C = pwm; break; //8
		case 6: OCR1A = pwm; break;// d11
		case 7: OCR1B = pwm; break;// d12
	}
} 

void set_all_channels(int16_t val)
{
	ASSERT(s_is_initialized);

	val = math::clamp(val, int16_t(0), int16_t(1024));
	val = (uint32_t(val) * uint32_t(PULSE_RANGE) >> 10);
	uint16_t pwm = val + MIN_PULSE_WIDTH;

	OCR3B = pwm;
	OCR3C = pwm;
	OCR3A = pwm;
	OCR4A = pwm;
	OCR4B = pwm;
	OCR4C = pwm;
	OCR1A = pwm;
	OCR1B = pwm;
}

void set_all_enabled(bool enabled)
{
	ASSERT(s_is_initialized);

	if (enabled)
	{
		TCCR3A |= (1<<COM3A1); 
		TCCR4A |= (1<<COM4A1); 		TCCR3A |= (1<<COM3B1); 		TCCR3A |= (1<<COM3C1); 		TCCR4A |= (1<<COM4B1); 		TCCR4A |= (1<<COM4C1); 		TCCR1A |= (1<<COM1A1); 		TCCR1A |= (1<<COM1B1); 	}
	else
	{
		TCCR3A &= ~(1<<COM3A1);
		TCCR4A &= ~(1<<COM4A1);		TCCR3A &= ~(1<<COM3B1);		TCCR3A &= ~(1<<COM3C1);		TCCR4A &= ~(1<<COM4B1);		TCCR4A &= ~(1<<COM4C1);		TCCR1A &= ~(1<<COM1A1);		TCCR1A &= ~(1<<COM1B1);	}
}
void set_enabled(uint8_t ch, bool enabled)
{
	ASSERT(s_is_initialized);

	if (enabled)
	{
		switch (ch)
		{
			case 0: TCCR3A |= (1<<COM3B1); break;
			case 1: TCCR3A |= (1<<COM3C1); break;
			case 2: TCCR3A |= (1<<COM3A1); break;
			case 3: TCCR4A |= (1<<COM4A1); break;
			case 4: TCCR4A |= (1<<COM4B1); break;
			case 5: TCCR4A |= (1<<COM4C1); break;
			case 6: TCCR1A |= (1<<COM1A1); break;
			case 7: TCCR1A |= (1<<COM1B1); break;		}
	}
	else
	{
		switch (ch)
		{
			case 0: TCCR3A &= ~(1<<COM3B1); break;
			case 1: TCCR3A &= ~(1<<COM3C1); break;
			case 2: TCCR3A &= ~(1<<COM3A1); break;
			case 3: TCCR4A &= ~(1<<COM4A1); break;
			case 4: TCCR4A &= ~(1<<COM4B1); break;
			case 5: TCCR4A &= ~(1<<COM4C1); break;
			case 6: TCCR1A &= ~(1<<COM1A1); break;
			case 7: TCCR1A &= ~(1<<COM1B1); break;		}
	}
}

}
}

#endif
