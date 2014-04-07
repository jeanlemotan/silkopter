#include "Config.h"

#if BOARD_TYPE == CRIUS_AIOP2

#include <avr/io.h>
#include <avr/wdt.h>
#include <avr/interrupt.h>
#include "board/board.h"
#include "debug/debug.h"
#include <_qmath.h>
#include "util/Scope_Sync.h"

namespace board
{
namespace scheduler
{
    

static volatile bool s_is_in_callback = false;
static bool s_is_initialized = false;
static std::pair<Callback, void*> s_callbacks[MAX_CALLBACK_COUNT]; 
static uint8_t s_callback_count = 0;

// AVRScheduler timer interrupt period is controlled by TCNT2.
// 256-124 gives a 500Hz period
// 256-62 gives a 1kHz period.
// 256-31 gives a 2kHz period.
static volatile uint8_t s_timer_reset_value = (256 - 62);
static Frequency s_frequency = Frequency::_1000_HZ;

static void _run_timer_procs();

ISR(TIMER2_OVF_vect) 
{
    TCNT2 = s_timer_reset_value;
    sei();
    _run_timer_procs();
}

void _run_timer_procs()
{
	if (s_is_in_callback)
	{
#ifdef NDEBUG
		//PANIC_MSG("timers in progress");
#else
		//PRINT("\ntimers in progress");
#endif
		return;
	}
	s_is_in_callback = true;

	// now call the timer based drivers
	for (int i = 0; i < s_callback_count; i++)
	{
		s_callbacks[i].first(s_callbacks[i].second);
	}

	//	hal.gpio->write(45,0);
	s_is_in_callback = false;
}

//////////////////////////////////////////////////////////////////////////
// PUBLIC API


void init(Frequency freq)
{
	if (s_is_initialized)
	{
		return;
	}
	s_is_initialized = true;

	s_frequency = freq;
	auto freq_hz = static_cast<uint16_t>(freq);
	s_timer_reset_value = 256 - (31 * (2000 / freq_hz));
	
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

void register_callback(Callback cb, void* user_data)
{
	if (!cb)
	{
		return;
	}
    for (int i = 0; i < s_callback_count; i++) 
	{
        if (s_callbacks[i].first == cb && s_callbacks[i].second == user_data) 
		{
            return;
        }
    }

    if (s_callback_count < MAX_CALLBACK_COUNT) 
	{
        /* this write to _timer_proc can be outside the critical section
         * because that memory won't be used until _num_timer_procs is
         * incremented. */
        s_callbacks[s_callback_count] = std::pair<Callback, void*>(cb, user_data);
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

Frequency get_frequency()
{
	return s_frequency;	
}
uint16_t get_frequency_hz()
{
	return static_cast<uint16_t>(s_frequency);
}

}
}

#endif
