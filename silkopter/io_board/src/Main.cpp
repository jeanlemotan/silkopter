#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdio.h>
#include <vector>
#include <algorithm>
#include <_qmath.h>
#include <board/board.h>
#include <QBase.h>
#include "utils/storage.h"
#include "utils/Buffer.h"
#include "Comms.h"

__extension__ typedef int __guard __attribute__((mode (__DI__))); 

extern "C" int __cxa_guard_acquire(__guard *g) {return !*(char *)(g);};
extern "C" void __cxa_guard_release (__guard *g) {*(char *)g = 1;};
extern "C" void __cxa_guard_abort (__guard *) {};
extern "C" void __cxa_pure_virtual() { while (1); }

void* operator new(size_t size)
{
    return malloc(size);
}

void operator delete(void* obj)
{
    free(obj);
}

int main(void)
{
	sei();
	
    board::init();

    board::get_comm_uart().set_blocking(true);
    board::get_comm_uart().set_buffered(true);

    silk::Comms comms;

	//util::FString<128> str;
	
// 	auto last_fps = board::clock::now_ms();
// 	uint32_t fps = 0;
// 	uint32_t crt_fps = 0;

    //q::quick_logf(F_STR("Starting io board"));

    while(1)
    {
        comms.process();

		//auto start = now;

		
// 		{
// 			float max_error = 0.f;
// 			float avg_error = 0.f;
// 			size_t range = 100;
// 			const float range_inv = 1.f / range;
// 			for (volatile size_t i = 0; i < range; i++)
// 			{
// 				float angle = ((i * range_inv) * math::anglef::_2pi.radians);
// 				float res = math::sqrt(angle);
// 				float fres = __float32_flip_void* operator new(size_t objsize) {
// 				float error = math::abs(res - fres);
// 				max_error = math::max(max_error, error);
// 				avg_error += error;
// 			}
// 			avg_error /= float(range);
// 			q::quick_logf(F_STR("max_error = {}, avg_error = {}"), max_error, avg_error);
// 			float t = 0, t1 = 0;
// 			{
// 				TIMED_BLOCK();
// 				for (volatile size_t i = 0; i < range; i++)
// 				{
// 					float angle = i * math::anglef::_2pi.radians * range_inv;
// 					t += math::sqrt(angle);
// 					t += math::sqrt(t);
// 					t += math::sqrt(t);
// 					t += math::sqrt(t);
// 				}
// 			}
// 			{
// 				TIMED_BLOCK();
// 				for (volatile size_t i = 0; i < range; i++)
// 				{
// 					float angle = i * math::anglef::_2pi.radians * range_inv;
// 					t1 += __float32_flip_sqrt_float32(angle);
// 					t1 += __float32_flip_sqrt_float32(t);
// 					t1 += __float32_flip_sqrt_float32(t);
// 					t1 += __float32_flip_sqrt_float32(t);
// 				}
// 			}
// 			q::quick_logf(F_STR("{} == {}"), t, t1);
// 		}
// 
		
		{
// 			board::Barometer::Data data;
// 			board::get_barometer(0).get_data(data);
		}
   }
}
