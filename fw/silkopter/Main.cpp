#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdio.h>
#include <vector>
#include <algorithm>
//#include <chrono>
#include <qmath.h>
#include <HAL/UART.h>
#include <HAL/Clock.h>
#include <HAL/RC_Inputs.h>
#include <HAL/RC_Outputs.h>
#include <Format/Format.h>

int main(void)
{
	hal::uart0.begin(115200);
	hal::rc_outputs.set_frequencies(50);
	hal::rc_outputs.set_all_enabled(true);
	
	sei();

	uint32_t last = hal::clock.micros();
	
	math::vec3f a(1, 2, 3);
	math::vec3f b;
	
	FString<64> str;
					
    while(1)
    {
        //TODO:: Please write your application code 
		uint32_t now = hal::clock.micros();

		int16_t channels[8];
		last = hal::clock.micros();
		hal::rc_inputs.get_channels(channels, 8);
		now = hal::clock.micros();
		auto d1 = now - last;


		last = hal::clock.micros();
		uint16_t value = channels[2];
		hal::rc_outputs.set_all_channels(value);

// 		format(str, "#{0}s: {1}\t{2}\t{3}\t{4}\t{5}\t{6}\t{7}\t{8}\n", hal::clock.millis() >> 10,
// 		channels[0], channels[1], channels[2], channels[3],
// 		channels[4], channels[5], channels[6], channels[7]);
		now = hal::clock.micros();
		auto d2 = now - last;

		hal::uart0.write(str.c_str());
 		format(str, "timing: {0}us / {1}us\n", d1, d2);
		hal::uart0.write(str.c_str());
		
		hal::clock.delay_millis(1);

		last = now;
    }
}