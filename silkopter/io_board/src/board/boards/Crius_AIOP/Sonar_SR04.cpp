#include "Config.h"

#if BOARD_TYPE == BOARD_CRIUS_AIOP_2 && SONAR_TYPE == SONAR_SR04

#include <avr/io.h>
#include <avr/interrupt.h>
#include <_qmath.h>
#include "board/board.h"
#include "board/boards/Crius_AIOP/Sonar_SR04.h"

//////////////////////////////////////////////////////////////////////////

static board::Sonar_SR04* s_instance = nullptr;

ISR(PCINT0_vect)
{
	board::Sonar_SR04::trigger();
}

namespace board
{

static const chrono::micros k_period(150000);
static const chrono::micros k_max_valid_delay(9300); //~3m


void Sonar_SR04::measure(void* ptr)
{
	auto* sonar = reinterpret_cast<board::Sonar_SR04*>(ptr);
	QASSERT(sonar);
	
	auto now = board::clock::now_us();
	if (now - sonar->m_last_trigger >= k_period)
	{
		sonar->m_last_trigger = now;
		sonar->m_echo_delay.count = 0;
		sonar->m_state = 0;
		
		PORTH |= 0B01000000; // set Sonar TX pin to 1 and after ~12us set it to 0 (below) to start new measurement
		clock::delay(chrono::micros(12));
		PORTH &= 0B10111111; // set TX pin to 0, and wait for 1 on Echo pin (below)
	}
	else if (sonar->m_state == 1)
	{
		sonar->m_state = 0;
		chrono::micros delay(sonar->m_echo_delay.count >> 1); //divide by two to account for the round trip
        if (delay < k_max_valid_delay)
        {
            auto& buffer = sonar->m_buffer.get();
            buffer.delay_us = delay.count;
            buffer.sample_count = 1;
        }
    }
}

void Sonar_SR04::trigger()
{
	QASSERT(s_instance);
	if (s_instance->m_state == 0)
	{
		if (PINB & 0B00010000)
		{
			s_instance->m_start_time.ticks = clock::now_us().ticks; // We got 1 on Echo pin, remeber current counter value
		}
		else
		{
			s_instance->m_echo_delay.count = (clock::now_us() - s_instance->m_start_time).count; // We got 0 on Echo pin, calculate impulse length in counter ticks
			s_instance->m_state = 1; // Set "Measurement finished" flag
		}
	}
}

//////////////////////////////////////////////////////////////////////////

Sonar_SR04::Sonar_SR04()
{
    m_data_config.scale = 0.000343f;
}

void Sonar_SR04::start()
{
    QASSERT(!s_instance);
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
	
    scheduler::register_callback(chrono::millis(10), measure, this);
    m_is_started = true;
}

bool Sonar_SR04::is_started() const
{
    return m_is_started;
}

bool Sonar_SR04::get_data(Data& data) const
{
    if (m_buffer.get().sample_count > 0)
    {
        auto& buffer = m_buffer.get_and_swap();

        uint16_t raw = buffer.delay_us;
        //buffer.delay = 0;

        QASSERT(buffer.sample_count > 0);
        //uint8_t sc = buffer.sample_count;
        buffer.sample_count = 0;

        //-------------------------------
        //done with the buffer

        m_out_data.distance = raw;
        m_out_data.id++;
    }

    if (m_out_data.id != data.id)
    {
        data = m_out_data;
        return true;
    }
    return false;
}


}

#endif
