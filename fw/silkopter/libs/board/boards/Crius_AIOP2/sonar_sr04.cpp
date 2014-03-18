#include "Config.h"

#if BOARD_TYPE == CRIUS_AIOP2 && SONAR_TYPE == SONAR_SR04
#include <avr/io.h>
#include <avr/interrupt.h>
#include "debug/debug.h"
#include "board/sonar.h"
#include "board/clock.h"
#include "board/scheduler.h"
#include <qmath.h>

namespace board
{
namespace sonar
{

static bool s_is_initialized = false;
static volatile uint32_t s_last_trigger_us = 0;
static const uint32_t k_period_us = 150000;
static const uint32_t k_echo_max_delay_us = 20000; //20ms max
static const float k_max_distance = 1.8f; 

static volatile uint8_t s_state;
static volatile uint32_t s_start_us = 0;
static volatile uint32_t s_echo_delay_us = 0;
static volatile float s_altitude = 0.f;

//////////////////////////////////////////////////////////////////////////

static void _sonar_trigger(uint32_t micros)
{
	if (micros - s_last_trigger_us >= k_period_us)
	{
		s_last_trigger_us = micros;
		s_state = 0;
		s_echo_delay_us = 0;
		
		PORTH |= 0B01000000; // set Sonar TX pin to 1 and after ~12us set it to 0 (below) to start new measurement
		clock::delay_micros(12);
		PORTH &= 0B10111111; // set TX pin to 0, and wait for 1 on Echo pin (below)  
	}
	else if (s_state == 1)
	{
		s_state = 0;
		auto delay = s_echo_delay_us >> 1; //divide by two to account for the round trip
		s_altitude = math::min(delay, k_echo_max_delay_us) * 0.000343f;
	}
}

ISR(PCINT0_vect)
{
	//TRACE();
	if (s_state == 0) 
	{
		if (PINB & 0B00010000) 
		{
			s_start_us = clock::micros(); // We got 1 on Echo pin, remeber current counter value
		} 
		else 
		{
			s_echo_delay_us = clock::micros() - s_start_us; // We got 0 on Echo pin, calculate impulse length in counter ticks
			s_state = 1; // Set "Measurement finished" flag
		}
	}
}

//////////////////////////////////////////////////////////////////////////
// PUBLIC API

void init()
{
	if (s_is_initialized)
	{
		return;
	}
	s_is_initialized = true;

	scheduler::init();
	
	// Sonar INIT
	//=======================
	PORTH &= 0B10111111; // H6 -d9  - sonar TX
	DDRH |= 0B01000000;

	PORTB &= 0B11101111; // B4 -d10 - sonar Echo
	DDRB &= 0B11101111;
	
	//INT0 init
	PCMSK0 |= (1 << PCINT4); // Enable interrup o sonar ECHO pin: B4 - D10
	PCICR |= (1 << PCIE0); // PCINT0 Interrupt enable for PORTB
	
	scheduler::register_callback(_sonar_trigger);
}

bool is_enabled()
{
	ASSERT(s_is_initialized);
	return true;
}

void get_data(Data& data)
{
	ASSERT(s_is_initialized);
	data.is_valid = s_altitude < 3.5f;
	data.altitude = data.is_valid ? s_altitude : 0.f;
}


}
}

#endif
