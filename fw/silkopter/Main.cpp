#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdio.h>
#include <vector>
#include <algorithm>
//#include <chrono>
#include <qmath.h>
#include <board/uart.h>
#include <board/clock.h>
#include <board/rc_in.h>
#include <board/pwm_out.h>
#include <board/scheduler.h>
#include <board/imu.h>
#include <util/format.h>
#include <debug/debug.h>
#include <board/sonar.h>
#include <board/baro.h>								   
#include <board/boards/avr_i2c.h>
#include "GS/GS.h"

__extension__ typedef int __guard __attribute__((mode (__DI__))); 

extern "C" int __cxa_guard_acquire(__guard *g) {return !*(char *)(g);};
extern "C" void __cxa_guard_release (__guard *g) {*(char *)g = 1;};
extern "C" void __cxa_guard_abort (__guard *) {};
extern "C" void __cxa_pure_virtual() { while (1); }
	
int main(void)
{
	sei();
	
	board::uart0.begin(115200);
	debug::init(&board::uart0);
	
	board::scheduler::init();
	board::scheduler::set_callback_frequency(500);
	board::clock::init();
	board::rc_in::init();
 	board::pwm_out::init();
 	board::imu::init(board::imu::Sample_Rate::RATE_500_HZ);
 	board::sonar::init();
	board::baro::init();

	board::pwm_out::set_frequencies(50);
	board::pwm_out::set_all_enabled(true);
	
	board::imu::calibrate(chrono::millis(1000));

	util::FString<128> str;

	silk::UAV uav(silk::Motor_Mixer::Type::X, 4, 0.3f);
	silk::GS gs(uav, board::uart0);

// 	auto last_fps = board::clock::now_ms();
// 	uint32_t fps = 0;
// 	uint32_t crt_fps = 0;
	
    while(1)
    {
        //TODO:: Please write your application code 
		auto start = board::clock::now_us();
		//auto start = now;
		
		uav.process();

		//the extra time - run the ground station connection
		//for up to 3ms
		{
			auto duration = board::clock::now_us() - start;
			if (duration < chrono::micros(5000))
			{
				gs.process(chrono::micros(5000) - duration);
			}
		}
   }
}