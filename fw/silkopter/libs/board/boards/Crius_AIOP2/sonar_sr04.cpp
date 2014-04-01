#include "Config.h"

#if BOARD_TYPE == CRIUS_AIOP2 && SONAR_TYPE == SONAR_SR04
#include <avr/io.h>
#include <avr/interrupt.h>
#include "debug/debug.h"
#include "board/board.h"
#include <qmath.h>

//////////////////////////////////////////////////////////////////////////

static board::Sonar* s_instance = nullptr;

ISR(PCINT0_vect)
{
	ASSERT(s_instance);
	
	//TRACE();
	if (s_instance->s_state == 0)
	{
		if (PINB & 0B00010000)
		{
			(chrono::time_us&)s_instance->s_start_time = clock::now_us(); // We got 1 on Echo pin, remeber current counter value
		}
		else
		{
			(chrono::micros&)s_instance->s_echo_delay = clock::now_us() - s_start_time; // We got 0 on Echo pin, calculate impulse length in counter ticks
			s_instance->s_state = 1; // Set "Measurement finished" flag
		}
	}
}

namespace board
{

static const chrono::micros k_period(150000);
static const chrono::micros k_echo_max_delay(20000); //20ms max
static const float k_max_distance = 1.8f;


void _sonar_trigger(void* ptr)
{
	auto* sonar = reinterpret_cast<board::Sonar*>(ptr);
	ASSERT(sonar);
	
	auto now = board::clock::now_us();
	if (now - sonar->s_last_trigger >= k_period)
	{
		sonar->s_last_trigger = now;
		sonar->s_state = 0;
		sonar->s_echo_delay.count = 0;
		
		PORTH |= 0B01000000; // set Sonar TX pin to 1 and after ~12us set it to 0 (below) to start new measurement
		clock::delay(chrono::micros(12));
		PORTH &= 0B10111111; // set TX pin to 0, and wait for 1 on Echo pin (below)
	}
	else if (sonar->s_state == 1)
	{
		sonar->s_state = 0;
		chrono::micros delay(sonar->s_echo_delay.count >> 1); //divide by two to account for the round trip
		sonar->s_distance = math::min(delay, k_echo_max_delay).count * 0.000343f;
	}
}

//////////////////////////////////////////////////////////////////////////

Sonar::Sonar()
	: s_is_initialized(false)
	, s_state(0)
	, s_distance(0)
{
	ASSERT(!s_instance);
	s_instance = this;

	// Sonar INIT
	//=======================
	PORTH &= 0B10111111; // H6 -d9  - sonar TX
	DDRH |= 0B01000000;

	PORTB &= 0B11101111; // B4 -d10 - sonar Echo
	DDRB &= 0B11101111;
	
	//INT0 init
	PCMSK0 |= (1 << PCINT4); // Enable interrup o sonar ECHO pin: B4 - D10
	PCICR |= (1 << PCIE0); // PCINT0 Interrupt enable for PORTB
	
	scheduler::register_callback(_sonar_trigger, this);
}

void Sonar::get_data(Data& data) const
{
	ASSERT(s_is_initialized);
	data.is_valid = s_distance < 3.5f;
	data.altitude = data.is_valid ? s_distance : 0.f;
}


}

#endif
