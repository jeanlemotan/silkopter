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
#include <debug/assert.h>

int main(void)
{
	board::uart0.begin(115200);
	debug::init(&board::uart0);
	
	board::scheduler::init();
	board::scheduler::set_callback_frequency(100);
	board::clock::init();
	board::rc_in::init();
	board::pwm_out::init();
	board::inertial::init(board::inertial::Sample_Rate::RATE_200_HZ);

	board::pwm_out::set_frequencies(50);
	board::pwm_out::set_all_enabled(true);

	auto last = board::clock::micros();
	
	math::vec3f a(1, 2, 3);
	math::vec3f b;
	
	util::FString<64> str;
					
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
		
		auto g = board::inertial::get_gyroscope_data();
		auto a = board::inertial::get_accelerometer_data();

//		oard::uart0.write(str.c_str());
 		//util::format(str, "timing: {0}us / {1}us\n", d1, d2);

		util::format(str, "gyro: {0}, {1}, {2} ::: {3}, {4}, {5}\n", g.x, g.y, g.z, a.x, a.y, a.z);
		board::uart0.write(str.c_str());
		
		board::clock::delay_millis(1);

		last = now;
    }
}