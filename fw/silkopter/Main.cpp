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
#include <board/inertial.h>
#include <util/format.h>
#include <debug/debug.h>
#include <board/sonar.h>
#include <board/baro.h>

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
 	board::inertial::init(board::inertial::Sample_Rate::RATE_500_HZ);
 	board::sonar::init();
	board::baro::init();

	board::pwm_out::set_frequencies(50);
	board::pwm_out::set_all_enabled(true);

	auto last = board::clock::micros();
	
	util::FString<128> str;
	
    while(1)
    {
        //TODO:: Please write your application code 
		auto now = board::clock::micros();

		int16_t channels[8];
		last = board::clock::micros();
		board::rc_in::get_channels(channels, 8);
		now = board::clock::micros();
		auto d1 = now - last;


		last = board::clock::micros();
		auto value = channels[2];
		board::pwm_out::set_all_channels(value);

// 		format(str, "#{0}s: {1}\t{2}\t{3}\t{4}\t{5}\t{6}\t{7}\t{8}\n", board::clock.millis() >> 10,
// 		channels[0], channels[1], channels[2], channels[3],
// 		channels[4], channels[5], channels[6], channels[7]);
// 		now = board::clock::micros();
// 		auto d2 = now - last;

		static math::vec3f attitude;

		static math::vec3f gyro_offset;
		static int32_t calibration_step = 100;
		
		board::inertial::Data i_inertial;
 		board::inertial::get_data(i_inertial);
		
		if (calibration_step > 0)
		{
			calibration_step--;
			gyro_offset = math::max(gyro_offset, math::abs(i_inertial.gyroscope.value));
		}
		else
		{
			//auto val = math::sgn(i_inertial.gyroscope.value) * 
			attitude += (i_inertial.gyroscope.value - gyro_offset);
		}

		PRINT("\n{0} :: {1}", attitude, gyro_offset);
		
		board::sonar::Data i_sonar; 
 		board::sonar::get_data(i_sonar);

		board::baro::Data i_baro;
		board::baro::get_data(i_baro);

//		oard::uart0.write(str.c_str());
 		//util::format(str, "timing: {0}us / {1}us\n", d1, d2);
			 
// 		int32_t iterations = 0;
// 		now = board::clock::micros();
// 		while (board::clock::micros() - now < 100000)
// 		{
// 			volatile int32_t x = 732715;
// 			for (volatile uint32_t i = 0; i < 1000; i++)
// 			{
// 				x += 7;
// 			}
// 			iterations++;
// 		}
// 		{
// 			auto cpu = (22 - iterations) * 100 / 22;
// 			PRINT("CPU: {0}\n", cpu); //22
// 		}

//  		PRINT("sonar {0}\tbaro {1}\ttemp {2}\tgyro {3}\taccel {4}\n", 
// 			 i_sonar.altitude, 
// 			 i_baro.pressure.value,
// 			 i_baro.temperature.value,
// 			 i_inertial.gyroscope.value, 
// 			 i_inertial.accelerometer.value);

//		board::clock::delay_millis(30);

		last = now;
   }
}