#pragma once

namespace math
{
	//returns true if the argument is a power of 2.
	// isPow2(2) == true
	// isPow2(3) == false
	// isPow2(16) == true
	template<typename T> bool is_pot(T a);

	//returns the smallest power of 2 that is greater or equal than the argument
	// getSmallestPow2(127) == 128
	// getSmallestPow2(128) == 128
	// getSmallestPow2(129) == 256
	template<typename T> T get_min_pot(T min);

	//returns the biggest power of 2 that is smaller or equal than the argument
	// getSmallestPow2(127) == 64
	// getSmallestPow2(128) == 128
	// getSmallestPow2(129) == 128
	template<typename T> T get_max_pot(T max);

	//returns the log2 of the argument
	// getPow2(4) == 2
	// getPow2(5) == 2
	// getPow2(7) == 2
	// getPow2(8) == 3
	// getPow2(0) == undefined
	template<typename T> int8_t get_pot(T val);

	//returns the number of 1 bits in the argument
	// getOnBitsCount(4) == 1
	// getOnBitsCount(5) == 2
	// getOnBitsCount(7) == 3
	template<typename T> int8_t get_on_bits_count(T x);

}
