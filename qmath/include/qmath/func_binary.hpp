namespace math
{


//////////////////////////////////////////////////////////////////////////

template<typename T> inline bool is_pot(T x)
{
    return (!is_zero(x)) && ((x & (x - 1)) == 0);
}

template<typename T> inline T get_min_pot(T min)
{
    MATH_ASSERT(min < ~0u);
	T result = 1;
	while (result < min) 
	{	
		result <<= 1;
	}
	return result;
}
template<typename T> inline T get_max_pot(T max)
{
    MATH_ASSERT(max > 0);
	T result = 1;
	T crt = result;
	while (crt <= max) 
	{	
		result = crt;
		crt <<= 1;
	}
	return result;
}

template<typename T> inline int8_t get_pot(T val)
{
    MATH_ASSERT(val > 0);
	int8_t shift = -1;
	while (val)
	{
		shift++;
		val >>= 1;
	}
	return shift;
}

template<> inline int8_t get_on_bits_count(uint32_t x)
{
	// From Hacker's Delight (http://books.google.com/books?id=iBNKMspIlqEC)
	x = x - ((x >> 1) & 0x55555555);
	x = (x & 0x33333333) + ((x >> 2) & 0x33333333);
	x = (x + (x >> 4)) & 0x0F0F0F0F;
	x = x + (x >> 8);
	x = x + (x >> 16);
	return int8_t(x & 0x0000003F);
}

template<typename T> inline int8_t get_on_bits_count(T x)
{
	uint8_t count = 0;
	uint8_t size = sizeof(T);
	for (uint8_t i = 0; i < size && x; i++)
	{
		count += (x & 0x1);
		x >>= 1;
	}
	return count;
}

}
