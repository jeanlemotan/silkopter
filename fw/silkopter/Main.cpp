#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdio.h>
#include <vector>
#include <algorithm>
//#include <chrono>
#include <_qmath.h>
#include <board/board.h>
#include <util/format.h>
#include "GS/GS.h"
#include "util/storage.h"
#include "util/murmurhash.h"

__extension__ typedef int __guard __attribute__((mode (__DI__))); 

extern "C" int __cxa_guard_acquire(__guard *g) {return !*(char *)(g);};
extern "C" void __cxa_guard_release (__guard *g) {*(char *)g = 1;};
extern "C" void __cxa_guard_abort (__guard *) {};
extern "C" void __cxa_pure_virtual() { while (1); }
	

int main(void)
{
	sei();
	
	board::Init_Params params;
	params.main_imu_sample_rate = board::IMU::Sample_Rate::_500_HZ;
	board::init(params);

	//util::FString<128> str;

	silk::UAV uav(silk::Motor_Mixer::Type::X, 4, 0.3f);
	silk::GS gs(uav, board::get_gs_full_uart());

// 	auto last_fps = board::clock::now_ms();
// 	uint32_t fps = 0;
// 	uint32_t crt_fps = 0;
	
    while(1)
    {
		auto start = board::clock::now_us();
		//auto start = now;

		uav.process();
		
		
// 		{
// 			float max_error = 0.f;
// 			float avg_error = 0.f;
// 			size_t range = 100;
// 			const float range_inv = 1.f / range;
// 			for (volatile size_t i = 0; i < range; i++)
// 			{
// 				float angle = ((i * range_inv) * math::anglef::_2pi.radians);
// 				float res = math::sqrt(angle);
// 				float fres = __float32_flip_sqrt_float32(angle);
// 				float error = math::abs(res - fres);
// 				max_error = math::max(max_error, error);
// 				avg_error += error;
// 			}
// 			avg_error /= float(range);
// 			debug::printf("\nmax_error = {}, avg_error = {}", max_error, avg_error);
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
// 			debug::printf("\n{} == {}", t, t1);
// 		}
// 
		
		//the extra time - run the ground station connection
		//for up to 3ms
		{
			//auto duration = board::clock::now_us() - start;
			//if (duration < chrono::micros(3000))
			{
				gs.process(chrono::micros(500));
			}
		}
   }
}