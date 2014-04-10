#pragma once

namespace util
{

template <size_t LEN>
inline uint32_t MURMURLEN(uint32_t hash, const char* str, size_t idx)
{
    return MURMURLEN<LEN-4>(
                MURMURLEN<4>(hash, str, idx),
                str,
                idx+4);
}

#define _m  0x5bd1e995
#define _r  24

inline uint32_t MURMURLEN4(uint32_t hash, char c1, char c2, char c3, char c4)
{
    #define _k  (uint32_t(c1) | (uint32_t(c2)<<8) | (uint32_t(c3) << 16) | (uint32_t(c4) << 24))

    uint32_t k;
    k = _k * _m;
    k ^= (k >> _r);
    k *= _m;
    return (hash * _m) ^ k;
}

template <>
inline uint32_t MURMURLEN<4>(uint32_t hash, const char* str, size_t idx)
{
    return MURMURLEN4(hash, str[idx+0],str[idx+1],str[idx+2],str[idx+3]);
}

template <>
inline uint32_t MURMURLEN<3>(uint32_t hash, const char* str, size_t idx)
{
    return (hash ^ (uint32_t(str[idx+2]) << 16) ^ (uint32_t(str[idx+1]) << 8) ^ (uint32_t(str[idx+0]))) * _m;
}

template <>
inline uint32_t MURMURLEN<2>(uint32_t hash, const char* str, size_t idx)
{
    return (hash ^ (str[idx+1] << 8) ^ (str[idx+0])) * _m;
}

template <>
inline uint32_t MURMURLEN<1>(uint32_t hash, const char* str, size_t idx)
{
    return (hash ^ (str[idx+0])) * _m;
}
template <size_t LEN>
inline uint32_t MURMUR(uint32_t seed, const char* str)
{
    uint32_t h = MURMURLEN<LEN>(seed ^ LEN, str, 0);
	h ^= h >> 13;
	h *= _m;
	h ^= h >> 15;
    return h;
}

inline uint32_t MurmurHash2 (const void * key, size_t len, uint32_t seed)
{
	// 'm' and 'r' are mixing constants generated offline.
	// They're not really 'magic', they just happen to work well.

	const uint32_t m = 0x5bd1e995;
	const size_t r = 24;

	// Initialize the hash to a 'random' value

	uint32_t h = seed ^ len;

	// Mix 4 bytes at a time into the hash

	const uint8_t* data = (const uint8_t*)key;

	while(len >= 4)
	{
		uint32_t k = *(uint32_t *)data;

		k *= m; 
		k ^= k >> r; 
		k *= m; 
		
		h *= m; 
		h ^= k;

		data += 4;
		len -= 4;
	}
	
	// Handle the last few bytes of the input array

	switch(len)
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

#undef _m
#undef _r

#define dynamic_murmurhash_seed(str, seed) util::MurmurHash2(str, strlen(str), seed)
#define static_murmurhash_seed(str, seed) util::MURMUR<sizeof(str)-1>(seed, str)

#define dynamic_murmurhash(str) util::MurmurHash2(str, strlen(str), 0)
#define static_murmurhash(str) util::MURMUR<sizeof(str)-1>(0, str)

}
