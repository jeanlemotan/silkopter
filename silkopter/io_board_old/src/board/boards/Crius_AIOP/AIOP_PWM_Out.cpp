#include "Config.h"

#if BOARD_TYPE == BOARD_CRIUS_AIOP_2

#include <avr/io.h>
#include <avr/interrupt.h>
#include "_qmath.h"
#include "board/board.h"
#include "board/boards/AVR_gpio.h"
#include "board/boards/Crius_AIOP/AIOP_PWM_Out.h"

namespace board
{

static const uint8_t MAX_CHANNEL_COUNT = 8;
static const int16_t MIN_PULSE_WIDTH = 1800; // 900
static const int16_t MAX_PULSE_WIDTH = 4200; // 2100
static const int16_t PULSE_RANGE = (MAX_PULSE_WIDTH - MIN_PULSE_WIDTH);
static const float PULSE_RANGE_F = float(PULSE_RANGE);
static const float PULSE_RANGE_F_INV = 1.f / PULSE_RANGE_F;

AIOP_PWM_Out::AIOP_PWM_Out()
	: m_is_initialized(false)
{
}

void AIOP_PWM_Out::init()
{
	if (m_is_initialized)
	{
		return;
	}
	m_is_initialized = true;

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

void AIOP_PWM_Out::set_frequency(uint8_t ch, uint16_t hz)
{
	QASSERT(m_is_initialized);

	uint16_t icr = timer_period(hz);
	switch(ch)
	{
		case 0:
		case 1:
		case 2:
			ICR3 = icr;
		break;

		case 6:
		case 7:
			ICR1 = icr;
		break;

		case 3:
		case 4:
		case 5:
			ICR4 = icr;
		break;
	}
}
void AIOP_PWM_Out::set_frequency_for_all(uint16_t hz)
{
	QASSERT(m_is_initialized);

	for (uint8_t i = 0; i < MAX_CHANNEL_COUNT; i++)
	{
		set_frequency(i, hz);
	}
}
void AIOP_PWM_Out::set_data(uint8_t ch, Data const& data) 
{
	QASSERT(m_is_initialized);

	int16_t pulse = static_cast<int16_t>(data.value * PULSE_RANGE_F);
	pulse = MIN_PULSE_WIDTH + math::clamp(pulse, int16_t(0), PULSE_RANGE);
	switch(ch)
	{
		case 0: OCR3B = pulse; break; //2
		case 1: OCR3C = pulse; break; //3
		case 2: OCR3A = pulse; break; //5
		case 3: OCR4A = pulse; break; //6
		case 4: OCR4B = pulse; break; //7
		case 5: OCR4C = pulse; break; //8
		case 6: OCR1A = pulse; break;// d11
		case 7: OCR1B = pulse; break;// d12
	}
} 
void AIOP_PWM_Out::set_data_for_all(Data const& data)
{
	QASSERT(m_is_initialized);

	int16_t pulse = static_cast<int16_t>(data.value * PULSE_RANGE_F);
	pulse = MIN_PULSE_WIDTH + math::clamp(pulse, int16_t(0), PULSE_RANGE);

	OCR3B = pulse; //2
	OCR3C = pulse; //3
	OCR3A = pulse; //5
	OCR4A = pulse; //6
	OCR4B = pulse; //7
	OCR4C = pulse; //8
	OCR1A = pulse;// d11
	OCR1B = pulse;// d12
}
void AIOP_PWM_Out::set_enabled_for_all(bool enabled)
{
	QASSERT(m_is_initialized);

	if (enabled)
	{
		TCCR3A |= (1<<COM3A1); 
		TCCR4A |= (1<<COM4A1); 
		TCCR3A |= (1<<COM3B1); 
		TCCR3A |= (1<<COM3C1); 
		TCCR4A |= (1<<COM4B1); 
		TCCR4A |= (1<<COM4C1); 
		TCCR1A |= (1<<COM1A1); 
		TCCR1A |= (1<<COM1B1); 
	}
	else
	{
		TCCR3A &= ~(1<<COM3A1);
		TCCR4A &= ~(1<<COM4A1);
		TCCR3A &= ~(1<<COM3B1);
		TCCR3A &= ~(1<<COM3C1);
		TCCR4A &= ~(1<<COM4B1);
		TCCR4A &= ~(1<<COM4C1);
		TCCR1A &= ~(1<<COM1A1);
		TCCR1A &= ~(1<<COM1B1);
	}
}
void AIOP_PWM_Out::set_enabled(uint8_t ch, bool enabled)
{
	QASSERT(m_is_initialized);

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
			case 7: TCCR1A |= (1<<COM1B1); break;
		}
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
			case 7: TCCR1A &= ~(1<<COM1B1); break;
		}
	}
}

uint8_t AIOP_PWM_Out::get_count() const
{
	QASSERT(m_is_initialized);

	return MAX_CHANNEL_COUNT;
}

}

#endif
