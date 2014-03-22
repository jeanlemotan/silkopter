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

int __cxa_guard_acquire(__guard *g) {return !*(char *)(g);};
void __cxa_guard_release (__guard *g) {*(char *)g = 1;};
void __cxa_guard_abort (__guard *) {};
	
int main(void)
{
	sei();
	
	board::uart0.begin(115200);
	debug::init(&board::uart0);
	
	board::scheduler::init();
	board::scheduler::set_callback_frequency(1000);
	board::clock::init();
	board::rc_in::init();
 	board::pwm_out::init();
 	board::imu::init(board::imu::Sample_Rate::RATE_500_HZ);
 	board::sonar::init();
	board::baro::init();

	board::pwm_out::set_frequencies(50);
	board::pwm_out::set_all_enabled(true);

	auto last = board::clock::now_us();
	
	util::FString<128> str;

	silk::UAV uav(silk::Motor_Mixer::Type::X, 4, 0.3f);
	silk::GS gs(board::uart0);

// 	auto last_fps = board::clock::now_ms();
// 	uint32_t fps = 0;
// 	uint32_t crt_fps = 0;
	
    while(1)
    {
        //TODO:: Please write your application code 
		auto now = board::clock::now_us();
		//auto start = now;

// 		int16_t channels[8];
// 		last = board::clock::now_us();
// 		board::rc_in::get_channels(channels, 8);
// 		now = board::clock::now_us();
// 		auto d1 = now - last;


// 		last = board::clock::now_us();
// 		auto value = channels[2];
// 		board::pwm_out::set_all_channels(value);

// 		format(str, "#{0}s: {1}\t{2}\t{3}\t{4}\t{5}\t{6}\t{7}\t{8}\n", board::clock.millis() >> 10,
// 		channels[0], channels[1], channels[2], channels[3],
// 		channels[4], channels[5], channels[6], channels[7]);
// 		now = chrono::micros();
// 		auto d2 = now - last;

// 		static math::vec3f attitude;
// 
// 		static math::vec3f gyro_offset;
// 		static int32_t calibration_step = 100;
		
// 		board::imu::Data i_imu;
//  		board::imu::get_data(i_imu);
		
// 		if (calibration_step > 0)
// 		{
// 			calibration_step--;
// 			gyro_offset = math::max(gyro_offset, math::abs(i_imu.gyroscope.value));
// 		}
// 		else
// 		{
// 			//auto val = math::sgn(i_imu.gyroscope.value) * 
// 			attitude += (i_imu.gyroscope.value - gyro_offset);
// 		}

		//PRINT("\n{0} :: {1}", attitude, gyro_offset);
		
// 		board::sonar::Data i_sonar; 
//  		board::sonar::get_data(i_sonar);

// 		board::baro::Data i_baro;
// 		board::baro::get_data(i_baro);

//		oard::uart0.write(str.c_str());
 		//util::format(str, "timing: {0}us / {1}us\n", d1, d2);
			 
//  		int32_t iterations = 0;
//  		now = board::clock::now_us();
//  		while (board::clock::now_us() - now < 100000)
//  		{
//  			volatile int32_t x = 732715;
//  			for (volatile uint32_t i = 0; i < 1000; i++)
//  			{
//  				x += 7;
//  			}
//  			iterations++;
//  		}
//  		{
//  			auto cpu = (22 - iterations) * 100 / 22;
//  			PRINT("CPU: {0} locks {1}\n", cpu, board::i2c::get_lockup_count()); //22
//  		}

// 		auto fps_now = board::clock::now_ms();
// 		if (fps_now - last_fps >= chrono::millis(1000))
// 		{
// 			last_fps = fps_now;
// 			fps = crt_fps;
// 			crt_fps = 0;
// 		}
// 		crt_fps++;
// 
// 		static uint32_t counter = 0;
// 		if (counter++ > 30)
// 		{
// 			counter = 0;
// 			PRINT("{0}\n",
// 			fps);
// 		}
// 
		gs.process(chrono::micros(500));

// 		{
// 			auto duration = board::clock::now_us() - start;
// 			if (duration < chrono::micros(200000))
// 			{
// 				board::clock::delay(chrono::micros(200000) - duration);
// 			}
// 		}

		last = now;
   }
}