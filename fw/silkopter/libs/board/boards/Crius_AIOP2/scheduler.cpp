#include "Config.h"

#if BOARD_TYPE == CRIUS_AIOP2

#include <avr/io.h>
#include <avr/wdt.h>
#include <avr/interrupt.h>
#include "board/scheduler.h"
#include "board/clock.h"
#include "debug/debug.h"
#include <qmath.h>
#include "util/Scope_Sync.h"

namespace board
{
namespace scheduler
{
    

static volatile bool s_is_in_callback = false;
static bool s_is_initialized = false;
static Callback s_callbacks[MAX_CALLBACK_COUNT] = { nullptr }; 
static uint8_t s_callback_count = 0;

// AVRScheduler timer interrupt period is controlled by TCNT2.
// 256-124 gives a 500Hz period
// 256-62 gives a 1kHz period.
// 256-31 gives a 2kHz period.
static volatile uint8_t s_timer2_reset_value = (256 - 62);
static uint16_t s_timer_frequency = 1000;

static void _run_timer_procs();

ISR(TIMER2_OVF_vect) 
{
    TCNT2 = s_timer2_reset_value;
    sei();
    _run_timer_procs();
}

void _run_timer_procs()
{
	if (s_is_in_callback)
	{
		PANIC_MSG("timers in progress");
		return;
	}
	s_is_in_callback = true;

	// now call the timer based drivers
	for (int i = 0; i < s_callback_count; i++)
	{
		s_callbacks[i]();
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
	if (!cb)
	{
		return;
	}
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
        {
            util::Scope_Sync ss();
            s_callback_count++;
        }
    }

}

void stop() 
{
	TIMSK2 = 0;
}

bool is_in_callback() 
{
    return s_is_in_callback;
}

void set_callback_frequency(uint16_t timer_hz)
{
	s_timer_frequency = math::clamp(timer_hz, uint16_t(250), uint16_t(2000));
    s_timer2_reset_value = 256 - (31 * (2000 / s_timer_frequency));
}
uint16_t get_callback_frequency()
{
	return s_timer_frequency;	
}

}
}

#endif
