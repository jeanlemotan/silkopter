namespace math
{


//////////////////////////////////////////////////////////////////////////

template<typename T> MATH_FORCE_INLINE bool is_pot(T a)
{
	return !(a & (a - 1)) && a;
}

template<typename T> MATH_FORCE_INLINE T get_min_pot(T min)
{
	assert(min < ~0u);
	T result = 1;
	while (result < min) 
	{	
		result <<= 1;
	}
	return result;
}
template<typename T> MATH_FORCE_INLINE T get_max_pot(T max)
{
	assert(max > 0);
	T result = 1;
	T crt = result;
	while (crt <= max) 
	{	
		result = crt;
		crt <<= 1;
	}
	return result;
}

template<typename T> MATH_FORCE_INLINE int8_t get_pot(T val)
{
	assert(val > 0);
	int8_t shift = -1;
	while (val)
	{
		shift++;
		val >>= 1;
	}
	return shift;
}

MATH_FORCE_INLINE uint32_t get_on_bits_count(uint32_t x)
{
	// From Hacker's Delight (http://books.google.com/books?id=iBNKMspIlqEC)
	x = x - ((x >> 1) & 0x55555555);
	x = (x & 0x33333333) + ((x >> 2) & 0x33333333);
	x = (x + (x >> 4)) & 0x0F0F0F0F;
	x = x + (x >> 8);
	x = x + (x >> 16);
	return x & 0x0000003F;
}


}