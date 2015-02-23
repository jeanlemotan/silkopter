#pragma once

namespace q
{
namespace util
{

inline uint32_t compute_murmur_hash32(uint8_t const* data, size_t size, uint32_t seed = 0)
{
    QASSERT(data);
    if (!data)
    {
        return seed;
    }
    // 'm' and 'r' are mixing constants generated offline.
	// They're not really 'magic', they just happen to work well.

	const uint32_t m = 0x5bd1e995;
	const size_t r = 24;

	// Initialize the hash to a 'random' value

    uint32_t h = seed ^ size;

	// Mix 4 bytes at a time into the hash
    while(size >= 4)
	{
		uint32_t k = *(uint32_t *)data;

		k *= m; 
		k ^= k >> r; 
		k *= m; 
		
		h *= m; 
		h ^= k;

		data += 4;
        size -= 4;
	}
	
	// Handle the last few bytes of the input array

    switch (size)
	{
	case 3: h ^= uint32_t(data[2]) << 16;
	case 2: h ^= uint32_t(data[1]) << 8;
	case 1: h ^= uint32_t(data[0]);
	        h *= m;
	};

	// Do a few final mixes of the hash to ensure the last few
	// bytes are well-incorporated.

    h ^= h >> 13;
	h *= m;
	h ^= h >> 15;

	return h;
} 

inline uint32_t compute_murmur_hash32(char const* c_str, uint32_t seed = 0)
{
    QASSERT(c_str);
    if (!c_str)
    {
        return seed;
    }
    return compute_murmur_hash32(reinterpret_cast<uint8_t const*>(c_str), strlen(c_str), seed);
}

inline uint16_t compute_murmur_hash16(uint8_t const* data, size_t size, uint32_t seed = 0)
{
    auto h32 = compute_murmur_hash32(data, size, seed);
    uint16_t h = static_cast<uint16_t>(h32 & 0xFFFF) ^ static_cast<uint16_t>(h32 >> 16);
    return h;
}

inline uint32_t compute_murmur_hash16(char const* c_str, uint32_t seed = 0)
{
    QASSERT(c_str);
    if (!c_str)
    {
        return seed;
    }
    return compute_murmur_hash16(reinterpret_cast<uint8_t const*>(c_str), strlen(c_str), seed);
}

}
}
