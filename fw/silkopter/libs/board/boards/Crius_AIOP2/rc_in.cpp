#include <avr/io.h>
#include <avr/interrupt.h>
#include "qmath.h"
#include "Debug/Assert.h"
#include "board/boards/Crius_AIOP2/rc_in.h"

#if BOARD_TYPE == CRIUS_AIOP2

namespace board
{
namespace rc_in
{

// PPM_SUM filtering
#define FILTER FILTER_DISABLED
/*
	FILTER_DISABLED
	FILTER_AVERAGE
	FILTER_JITTER
*/

#define JITTER_THRESHOLD 4
#define AVARAGE_FACTOR 1 		//changes the influance of the current meassured value to the final filter value -
								//bigger means less influance
								//min.value 1, resonable [1,2], uneven is faster [1]
#if (AVARAGE_FACTOR < 1)
#	error Wrong AVARAGE_FACTOR selected. Minimum value 1
#endif

static const uint8_t MIN_CHANNEL_COUNT = 5;     // for ppm sum we allow less than 8 channels to make up a valid packet

static const int16_t MIN_PULSE_WIDTH = 1800; // 900
static const int16_t MAX_PULSE_WIDTH = 4200; // 2100
static const int16_t MIN_PPM_SYNCH_WIDTH = 5000; //2500
static const int16_t PULSE_RANGE = (MAX_PULSE_WIDTH - MIN_PULSE_WIDTH);

/* private variables to communicate with input capture isr */
static volatile int16_t s_pulses[rc_in::MAX_CHANNEL_COUNT] = {0};  
static volatile int16_t s_raw_data[rc_in::MAX_CHANNEL_COUNT]; // Default RC values
static volatile uint16_t s_edge_time[8];
	
typedef void (*ISR_Func_Ptr)();
static volatile ISR_Func_Ptr s_isr_function = 0; 

static bool s_is_initialized = false;

ISR(PCINT2_vect) 
{
	if (s_isr_function)
	{
		s_isr_function();
	}
}

/* ISR for PPM SUM decoder on PL1 pin (only CRIUS v2 board) */
// static void _ppmsum_PL1_isr() 
// {
//     static uint16_t icr5_prev;
//     static uint8_t  channel_ctr;
// 
//     const uint16_t icr5_current = ICR5;
//     uint16_t pulse_width = icr5_current - icr5_prev;;
// 	
//     if (pulse_width > MIN_PPM_SYNCHWIDTH) 
// 	{
//         // sync pulse detected.  Pass through values if at least a minimum number of channels received
//         if( channel_ctr >= rc_in::MIN_CHANNEL_COUNT ) 
// 		{
//             _valid_channels = channel_ctr;
//         }
//         channel_ctr = 0;
//     } 
// 	else 
// 	{
//         if (channel_ctr < rc_in::MAX_CHANNEL_COUNT) 
// 		{
//             _pulse_capt[channel_ctr] = pulse_width;
//             channel_ctr++;
//             if (channel_ctr == rc_in::MAX_CHANNEL_COUNT) 
// 			{
//                 _valid_channels = rc_in::MAX_CHANNEL_COUNT;
//             }
//         }
//     }
//     icr5_prev = icr5_current;
// }

/* ISR for PPM SUM decoder on A8 pin */
static void A8_ppm_isr()
{ 
	static uint8_t s_last_pin = 0;
	static uint8_t s_crt_channel = 0;
	static bool s_got_first_synch = false;

	uint16_t curr_time = TCNT5;         // 0.5us resolution
	uint8_t pin = PINK;               // PINK indicates the state of each PIN for the arduino port dealing with [A8-A15] digital pins (8 bits variable)
	uint8_t mask = pin ^ s_last_pin;   // doing a ^ between the current interruption and the last one indicates wich pin changed
	s_last_pin = pin;          // we memorize the current state of all PINs [D0-D7]

	// Rising edge detection
	if (mask & 1) 
	{ // Ensure pin A8(PPM_SUM) changed
		if (pin & 1) 
		{ // Rising edge?
			// it should be guaranteed to wrap around - do not need to check. (unsigned values)
			static uint16_t s_last_time = 0;
			uint16_t width = curr_time - s_last_time;
			s_last_time = curr_time; // Save edge time
				
			// Process channel pulse
			// Good widths?
			if (width > MIN_PULSE_WIDTH && width < MAX_PULSE_WIDTH && s_got_first_synch) 
			{
				if (s_crt_channel < rc_in::MAX_CHANNEL_COUNT) 
				{
					#if FILTER == FILTER_DISABLED
						s_raw_data[s_crt_channel] = width;
					#elif FILTER == FILTER_AVERAGE 
						s_raw_data[s_crt_channel]=((AVARAGE_FACTOR*s_raw_data[s_crt_channel])+width)/(AVARAGE_FACTOR+1);
					#elif FILTER == FILTER_JITTER 
						if (abs(s_raw_data[s_crt_channel]-width) > JITTER_THRESHOLD)
							s_raw_data[s_crt_channel] = width;
					#endif
				}
				// Count always even if we will get more then NUM_CHANNELS >> fault detection.
				s_crt_channel++;
	
				if (s_crt_channel > rc_in::MAX_CHANNEL_COUNT) 
				{
					s_got_first_synch = false;						//reset decoder
				}
			}

			// Process First SYNCH
			// We skip first frame, because we can start from middle, so first frame can be invalid
			else if (width > MIN_PPM_SYNCH_WIDTH && !s_got_first_synch)
			{
				s_got_first_synch = true;
				s_crt_channel = 0;
			}

			// Process any other SYNCH
			// it's SYNCH
			else if (width > MIN_PPM_SYNCH_WIDTH)
			{
				// Check for minimal channels and copy data to further process
				if (s_crt_channel >= MIN_CHANNEL_COUNT)
				{
					//store channels
					std::copy(s_raw_data, s_raw_data + rc_in::MAX_CHANNEL_COUNT, s_pulses);
				}
				s_crt_channel = 0;								// always rest on synch
			}
	
			// Process FAILURE - start from beginning ....
			// that's bad - we do not want to be here at any time ....
			else 
			{
				s_crt_channel = 0;
				s_got_first_synch = false;						//reset decoder
			}
		}
	}
}

// static void _pwm_A8_A15_isr(void)
// { //this ISR is common to every receiver channel, it is call everytime a change state occurs on a digital pin [D2-D7]
// 	uint8_t mask;
// 	uint8_t pin;
// 	uint16_t cTime,dTime;
// 	static uint8_t PCintLast;
// 
// 	cTime = TCNT5;         // from sonar
// 	pin = PINK;             // PINK indicates the state of each PIN for the arduino port dealing with [A8-A15] digital pins (8 bits variable)
// 	mask = pin ^ PCintLast;   // doing a ^ between the current interruption and the last one indicates wich pin changed
// 	PCintLast = pin;          // we memorize the current state of all PINs [D0-D7]
// 
// 	// generic split PPM  
// 	// mask is pins [D0-D7] that have changed // the principle is the same on the MEGA for PORTK and [A8-A15] PINs
// 	// chan = pin sequence of the port. chan begins at D2 and ends at D7
// 	
// 	if (mask & 1<<0) {
// 		if (!(pin & 1<<0)) {
// 			dTime = (cTime-edgeTime[0]); if (MIN_PULSEWIDTH<dTime && dTime<MAX_PULSEWIDTH) _pulse_capt[0] = dTime;
// 		} else edgeTime[0] = cTime;
// 	}
// 	if (mask & 1<<1) {
// 		if (!(pin & 1<<1)) {
// 			dTime = (cTime-edgeTime[1]); if (MIN_PULSEWIDTH<dTime && dTime<MAX_PULSEWIDTH) _pulse_capt[1] = dTime;
// 		} else edgeTime[1] = cTime;
// 	}
// 	if (mask & 1<<2) {
// 		if (!(pin & 1<<2)) {
// 			dTime = (cTime-edgeTime[2]); if (MIN_PULSEWIDTH<dTime && dTime<MAX_PULSEWIDTH) _pulse_capt[2] = dTime;
// 		} else edgeTime[2] = cTime;
// 	}
// 	if (mask & 1<<3) {
// 		if (!(pin & 1<<3)) {
// 			dTime = (cTime-edgeTime[3]); if (MIN_PULSEWIDTH<dTime && dTime<MAX_PULSEWIDTH) _pulse_capt[3] = dTime;
// 		} else edgeTime[3] = cTime;
// 	}
// 	if (mask & 1<<4) {
// 		if (!(pin & 1<<4)) {
// 			dTime = (cTime-edgeTime[4]); if (MIN_PULSEWIDTH<dTime && dTime<MAX_PULSEWIDTH) _pulse_capt[4] = dTime;
// 		} else edgeTime[4] = cTime;
// 	}
// 	if (mask & 1<<5) {
// 		if (!(pin & 1<<5)) {
// 			dTime = (cTime-edgeTime[5]); if (MIN_PULSEWIDTH<dTime && dTime<MAX_PULSEWIDTH) _pulse_capt[5] = dTime;
// 		} else edgeTime[5] = cTime;
// 	}
// 	if (mask & 1<<6) {
// 		if (!(pin & 1<<6)) {
// 			dTime = (cTime-edgeTime[6]); if (MIN_PULSEWIDTH<dTime && dTime<MAX_PULSEWIDTH) _pulse_capt[6] = dTime;
// 		} else edgeTime[6] = cTime;
// 	}
// 	if (mask & 1<<7) {
// 		if (!(pin & 1<<7)) {
// 			dTime = (cTime-edgeTime[7]); if (MIN_PULSEWIDTH<dTime && dTime<MAX_PULSEWIDTH) _pulse_capt[7] = dTime;
// 		} else edgeTime[7] = cTime;
// 	}
// 	
// 	// If we got pulse on throttle pin, report success  
// 	if (mask & 1<<pin_rc_channel[2]) {
// 		_valid_channels = rc_in::MAX_CHANNEL_COUNT;
// 	}
// }

void init() 
{
	if (s_is_initialized)
	{
		return;
	}
	
	DDRK = 0;  // Set PORTK as a digital port ([A8-A15] are consired as digital PINs and not analogical)
//	hal.gpio->pinMode(46, GPIO_OUTPUT); // ICP5 pin (PL1) (PPM input) CRIUS v2
//	hal.gpio->write(46,0);

	//Timer5 already configured in Scheduler
	//TCCR5A = 0; //standard mode with overflow at A and OC B and C interrupts
	//TCCR5B = (1<<CS11); //Prescaler set to 8, resolution of 0.5us

// #if SERIAL_PPM == SERIAL_PPM_DISABLED
// 		FireISRRoutine = _pwm_A8_A15_isr;
// 		PORTK = (1<<0) | (1<<1) | (1<<2) | (1<<3) | (1<<4) | (1<<5) | (1<<6) | (1<<7); //enable internal pull ups on the PINs of PORTK
// 		PCMSK2 = (1<<0) | (1<<1) | (1<<2) | (1<<3) | (1<<4) | (1<<5) | (1<<6) | (1<<7); // enable interrupts on A8-A15 pins;
// 		PCICR |= (1 << PCIE2); // PCINT2 Interrupt enable
// #elif SERIAL_PPM == SERIAL_PPM_ENABLED
		s_isr_function = A8_ppm_isr;
		PORTK = (1<<PCINT16); //enable internal pull up on the SERIAL SUM pin A8
		PCMSK2 |= (1 << PCINT16); // Enable int for pin A8(PCINT16)
		PCICR |= (1 << PCIE2); // PCINT2 Interrupt enable
// #elif SERIAL_PPM == SERIAL_PPM_ENABLED_PL1
// 		FireISRRoutine = 0;
// 		hal.gpio->pinMode(48, GPIO_INPUT); // ICP5 pin (PL1) (PPM input) CRIUS v2
// 		ISRRegistry* isrregistry = (ISRRegistry*) _isrregistry;
// 		isrregistry->register_signal(ISR_REGISTRY_TIMER5_CAPT, _ppmsum_PL1_isr);
// 		TCCR5B |= (1<<ICES5); // Enable input capture on rising edge 
// 		TIMSK5 |= (1<<ICIE5); // Enable input capture interrupt. Timer interrupt mask  
// 		PCMSK2 = 0;	// Disable INT for pin A8-A15
// #else
// #	error You must check SERIAL_PPM mode, something wrong
// #endif

	s_is_initialized = true;
}

int16_t get_channel(uint8_t ch)
{
	ASSERT(s_is_initialized);

    /* constrain ch */
    if (ch >= MAX_CHANNEL_COUNT) 
	{
		return 0;
	}
    /* grab channel from isr's memory in critical section*/
    cli();
	//now pulses is between 0 and 1200
    int16_t pulse = s_pulses[ch] >> 1;
    sei();
	
    pulse = math::clamp(pulse, int16_t(0), int16_t(PULSE_RANGE >> 1));
	//dst = pulse * 1024 / 1200
	//dst = pulse * 27 / 32 -- like this I avoid the division and the intrmediaries fit in int16_t
	return pulse * 27 >> 5;
}

void get_channels(int16_t* dst, uint8_t size)
{
	ASSERT(s_is_initialized);

	size = math::min(size, MAX_CHANNEL_COUNT);

    cli();
	int16_t pulses[MAX_CHANNEL_COUNT];
    for (uint8_t i = 0; i < size; i++) 
	{
		//now pulses is between 0 and 1200
        pulses[i] = (s_pulses[i] - MIN_PULSE_WIDTH) >> 1; 
    }
    sei();

    for (uint8_t i = 0; i < size; i++) 
	{
        auto pulse = math::clamp(pulses[i], int16_t(0), int16_t(PULSE_RANGE >> 1));
		
		//dst = pulse * 1024 / 1200
		//dst = pulse * 27 / 32 -- like this I avoid the division and the intrmediaries fit in int16_t
		dst[i] = pulse * 27 >> 5;
    }
}

}
}

#endif
