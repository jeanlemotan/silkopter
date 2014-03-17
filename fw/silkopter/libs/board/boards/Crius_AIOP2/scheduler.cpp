#include "Config.h"

#if BOARD_TYPE == CRIUS_AIOP2

#include <avr/io.h>
#include <avr/wdt.h>
#include <avr/interrupt.h>
#include "board/scheduler.h"
#include "board/clock.h"
#include "debug/debug.h"
#include <qmath.h>

namespace board
{
namespace scheduler
{
    

static volatile bool s_is_in_callback = false;
static bool s_is_initialized = false;
static volatile bool s_is_suspended = false;
static volatile bool s_event_missed = false;
static Callback s_callbacks[MAX_CALLBACK_COUNT] = { nullptr }; 
static uint8_t s_callback_count = 0;

/* AVRScheduler timer interrupt period is controlled by TCNT2.
 * 256-124 gives a 500Hz period
 * 256-62 gives a 1kHz period. */
static volatile uint8_t s_timer2_reset_value = (256 - 62);

static void _run_timer_procs(bool called_from_isr);

ISR(TIMER2_OVF_vect) 
{
    // we enable the interrupt again immediately and also enable
    // interrupts. This allows other time critical interrupts to
    // run (such as the serial receive interrupt). We catch the
    // timer calls taking too long using _in_timer_call.
    // This approach also gives us a nice uniform spacing between
    // timer calls

    TCNT2 = s_timer2_reset_value;
    sei();
    _run_timer_procs(true);
}

void _run_timer_procs(bool called_from_isr)
{
	//	hal.gpio->write(45,1);
	if (s_is_in_callback)
	{
		ASSERT("timer functions took too long");
		return;
	}
	s_is_in_callback = true;

	if (!s_is_suspended)
	{
		// now call the timer based drivers
		auto micros = clock::micros();
		for (int i = 0; i < s_callback_count; i++)
		{
			if (s_callbacks[i] != NULL)
			{
				s_callbacks[i](micros);
			}
		}
	}
	else if (called_from_isr)
	{
		s_event_missed = true;
	}

	//	hal.gpio->write(45,0);
	s_is_in_callback = false;
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
	
    /* TIMER2: Setup the overflow interrupt to occur at 1khz. */
    TIMSK2 = 0;                     /* Disable timer interrupt */
    TCCR2A = 0;                     /* Normal counting mode */
    TCCR2B = _BV(CS21) | _BV(CS22); /* Prescaler to clk/256 */
    TCNT2 = 0;                      /* Set count to 0 */
    TIFR2 = _BV(TOV2);              /* Clear pending interrupts */
    TIMSK2 = _BV(TOIE2);            /* Enable overflow interrupt*/
    
    sei();

	clock::init();
}

void register_callback(Callback cb) 
{
    for (int i = 0; i < s_callback_count; i++) 
	{
        if (s_callbacks[i] == cb) 
		{
            return;
        }
    }

    if (s_callback_count < MAX_CALLBACK_COUNT) 
	{
        /* this write to _timer_proc can be outside the critical section
         * because that memory won't be used until _num_timer_procs is
         * incremented. */
        s_callbacks[s_callback_count] = cb;
        /* _num_timer_procs is used from interrupt, and multiple bytes long. */
        uint8_t sreg = SREG;
        cli();
        s_callback_count++;
        SREG = sreg;        
    }

}

void suspend() 
{
    s_is_suspended = true;
}

void resume() 
{
    s_is_suspended = false;
    if (s_event_missed == true) 
	{
        _run_timer_procs(false);
        s_event_missed = false;
    }
}

bool is_in_callback() 
{
    return s_is_in_callback;
}

void set_callback_frequency(uint16_t timer_hz)
{
	timer_hz = math::clamp(timer_hz, uint16_t(250), uint16_t(1000));
    s_timer2_reset_value = 256 - (62 * (1000 / timer_hz));
}

}
}

#endif
