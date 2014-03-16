#include <avr/io.h>
#include <avr/interrupt.h>
#include "debug/assert.h"
#include "board/boards/Crius_AIOP2/clock.h"

#if BOARD_TYPE == CRIUS_AIOP2

namespace board
{
namespace clock
{

#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))

#define  AVR_TIMER_OVF_VECT       TIMER5_OVF_vect
#define  AVR_TIMER_TCNT           TCNT5
#define  AVR_TIMER_TIFR           TIFR5
#define  AVR_TIMER_TCCRA          TCCR5A
#define  AVR_TIMER_TCCRB          TCCR5B
#define  AVR_TIMER_OCRA           OCR5A
#define  AVR_TIMER_TIMSK          TIMSK5
#define  AVR_TIMER_TOIE           TOIE5
#define  AVR_TIMER_WGM0           WGM50
#define  AVR_TIMER_WGM1           WGM51
#define  AVR_TIMER_WGM2           WGM52
#define  AVR_TIMER_WGM3           WGM53
#define  AVR_TIMER_CS1            CS51

static volatile uint32_t s_micros_counter = 0;
static volatile uint32_t s_millis_counter = 0;
static bool s_is_initialized = false;

void init() 
{
	if (s_is_initialized)
	{
		return;
	}
	
	uint8_t old_sreg = SREG;
	cli();

	// Timer cleanup before configuring
	AVR_TIMER_TCNT = 0;
	AVR_TIMER_TIFR = 0;

	// Set timer 8x prescaler fast PWM mode toggle compare at OCRA
	AVR_TIMER_TCCRA = _BV( AVR_TIMER_WGM0 ) | _BV( AVR_TIMER_WGM1 );
	AVR_TIMER_TCCRB |= _BV( AVR_TIMER_WGM3 ) | _BV( AVR_TIMER_WGM2 ) | _BV( AVR_TIMER_CS1 );
	AVR_TIMER_OCRA  = 0xFFFF; // -1 to correct for wrap

	// Enable overflow interrupt
	AVR_TIMER_TIMSK |= _BV( AVR_TIMER_TOIE );

	// set a2d prescale factor to 128
	// 16 MHz / 128 = 125 KHz, inside the desired 50-200 KHz range.
	// XXX: this will not work properly for other clock speeds, and
	// this code should use F_CPU to determine the prescale factor.
	sbi(ADCSRA, ADPS2);
	sbi(ADCSRA, ADPS1);
	sbi(ADCSRA, ADPS0);

	// enable a2d conversions
	sbi(ADCSRA, ADEN);

	SREG = old_sreg;
	
	s_is_initialized = true;
}

SIGNAL(AVR_TIMER_OVF_VECT)
{
	// Hardcoded for AVR@16MHZ and 8x pre-scale 16-bit timer overflow at 40000
	s_micros_counter += 0xFFFF / 2; // 32768us each overflow
	s_millis_counter += 0xFFFF >> 11; // 32ms each overlflow
}

/* Delay for the given number of microseconds.  Assumes a 16 MHz clock. */
// void AVRTimer::delay_microseconds(uint16_t us)
// {
// 	// for the 16 MHz clock on most Arduino boards
// 	// for a one-microsecond delay, simply return.  the overhead
// 	// of the function call yields a delay of approximately 1 1/8 us.
// 	if (--us == 0)
// 	return;
// 
// 	// the following loop takes a quarter of a microsecond (4 cycles)
// 	// per iteration, so execute it four times for each microsecond of
// 	// delay requested.
// 	us <<= 2;
// 
// 	// account for the time taken in the preceeding commands.
// 	us -= 2;
// 
// 	// busy wait
// 	__asm__ __volatile__ (
// 	"1: sbiw %0,1" "\n\t" // 2 cycles
// 	"brne 1b" : "=w" (us) : "0" (us) // 2 cycles
// 	);
// }

uint32_t micros()
{
	ASSERT(s_is_initialized);

	uint8_t old_sreg = SREG;
	cli();

	// Hardcoded for AVR@16MHZ and 8x pre-scale 16-bit timer
	//uint32_t time_micros = timer_micros_counter + (AVR_TIMER_TCNT / 2);
	//uint32_t time_micros = timer_micros_counter + (AVR_TIMER_TCNT >> 1);

	uint32_t time_micros = s_micros_counter;
	uint16_t tcnt = AVR_TIMER_TCNT;

	// Check for  imminent timer overflow interrupt and pre-increment counter
	if (AVR_TIMER_TIFR & 1 && tcnt < 0xFFFF)
	{
		time_micros += 0xFFFF / 2;
	}
	SREG = old_sreg;

	return time_micros + (tcnt >> 1);
}

uint32_t millis() 
{
	ASSERT(s_is_initialized);

	uint8_t oldSREG = SREG;
	cli();
	// Hardcoded for AVR@16MHZ and 8x pre-scale 16-bit timer
	//uint32_t time_millis = timer_millis_counter + (AVR_TIMER_TCNT / 2000) ;
	//uint32_t time_millis =  timer_millis_counter + (AVR_TIMER_TCNT >> 11); // AVR_TIMER_CNT / 2048 is close enough (24us counter delay)

	uint32_t time_millis =  s_millis_counter;
	uint16_t tcnt = AVR_TIMER_TCNT;	  

	// Check for imminent timer overflow interrupt and pre-increment counter
	if (AVR_TIMER_TIFR & 1 && tcnt < 0xFFFF)
	{
		time_millis += 0xFFFF >> 11;
	}
	SREG = oldSREG;

	return time_millis + (tcnt >> 11);
}

void delay_micros(uint16_t us)
{
	ASSERT(s_is_initialized);

	// for the 16 MHz clock on most Arduino boards
	// for a one-microsecond delay, simply return.  the overhead
	// of the function call yields a delay of approximately 1 1/8 us.
	if (us <= 1)
	{
		return;
	}
	--us;
	
	// the following loop takes a quarter of a microsecond (4 cycles)
	// per iteration, so execute it four times for each microsecond of
	// delay requested.
	us <<= 2;

	// account for the time taken in the preceeding commands.
	us -= 3;

	// busy wait
	__asm__ __volatile__ (
	"1: sbiw %0,1" "\n\t" // 2 cycles
	"brne 1b" : "=w" (us) : "0" (us) // 2 cycles
	);
}

void delay_millis(uint16_t ms)
{
	ASSERT(s_is_initialized);

	uint32_t us = uint32_t(ms) * 1000;
	uint16_t times = us >> 14;
	uint16_t rest = ms & 16385; 
	
	for (uint16_t i = 0; i < times; i++)
	{
		delay_micros(16384);
	}
	if (rest)
	{
		delay_micros(rest);
	}
}


}
}
#endif
