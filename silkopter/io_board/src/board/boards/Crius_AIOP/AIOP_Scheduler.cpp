#include "Config.h"

#if BOARD_TYPE == BOARD_CRIUS_AIOP_2

#include <avr/io.h>
#include <avr/wdt.h>
#include <avr/interrupt.h>
#include "board/board.h"
#include <_qmath.h>
#include "utils/Scope_Sync.h"

namespace board
{
namespace scheduler
{


static uint16_t s_frequency = 2000;
static bool s_is_initialized = false;
struct CB_Data
{
	Callback callback = nullptr;
	void* user_data = nullptr;
	int8_t period = 1;
	int8_t counter = 1;
	bool is_executing = false;
};
static CB_Data s_callbacks[MAX_CALLBACK_COUNT]; 
static uint8_t s_callback_count = 0;

// AVRScheduler timer interrupt period is controlled by TCNT2.
// 256-124 gives a 500Hz period
// 256-62 gives a 1kHz period.
// 256-31 gives a 2kHz period.
static volatile uint8_t s_timer_reset_value = (256 - 31);

static void _run_timer_procs();

ISR(TIMER2_OVF_vect) 
{
    TCNT2 = s_timer_reset_value;
    sei();
    _run_timer_procs();
}

void _run_timer_procs()
{
	// now call the timer based drivers
	for (int i = 0; i < s_callback_count; i++)
	{
		auto& data = s_callbacks[i];
		--data.counter;
		if (data.counter <= 0)
		{
			data.counter = data.period;
			if (!data.is_executing)
			{
				data.is_executing = true;
				data.callback(data.user_data);
				data.is_executing = false;
			}
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

	s_timer_reset_value = 256 - (31 * (2000 / s_frequency));
	
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

void register_callback(chrono::micros _period, Callback cb, void* user_data)
{
	if (!cb)
	{
		return;
	}
	QASSERT(_period < chrono::millis(10000));
	
    int32_t period = _period.count;
	
	for (int i = 0; i < s_callback_count; i++) 
	{
		if (s_callbacks[i].callback == cb && 
			s_callbacks[i].user_data == user_data && 
			s_callbacks[i].period == period)
		{
			return;
		}
	}

    if (s_callback_count < MAX_CALLBACK_COUNT) 
	{
        uint32_t us_period = 1000000 / s_frequency;

        uint32_t x = period / us_period;

        /* this write to _timer_proc can be outside the critical section
         * because that memory won't be used until _num_timer_procs is
         * incremented. */
		CB_Data data;
		data.callback = cb;
        data.period = std::max<uint32_t>(x, 1);
        data.counter = x;
		data.user_data = user_data;
        s_callbacks[s_callback_count] = data;
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

uint16_t get_frequency()
{
	return s_frequency;
}

}
}

#endif
