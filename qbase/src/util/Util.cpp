#include "QBaseStdAfx.h"
#include "QBase.h"

namespace q
{
namespace util
{

int stricmp(char const* s1, char const* s2)
{
	for (;;) 
	{
		int cs1 = *s1++;
		int cs2 = *s2++;
		if (cs1 != cs2) 
		{
			int d = toupper_ascii(cs1) - toupper_ascii(cs2);
			if (d) 
			{
				return d;
			}
		} 
		else if (cs1 == '\0') 
		{
			return 0;
		}
	}
}
int strnicmp(char const* s1, char const* s2, size_t len)
{
	for (; len > 0; len--) 
	{
		int cs1 = *s1;
		int cs2 = *s2;
		if (cs1 != cs2) 
		{
			int d = toupper_ascii(cs1) - toupper_ascii(cs2);
			if (d) 
			{
				return d;
			}
		} 
		else if (cs1 == '\0') 
		{
			return 0;
		}

		++s1;
		++s2;
	}
	return 0;
}

int memicmp(char const* s1, char const* s2, size_t n)
{
	if (n != 0) 
	{
		do
		{
			int usl1 = *s1;
			int usl2 = *s2;
			if (usl1 != usl2)
			{
				int d = toupper_ascii(usl1) - toupper_ascii(usl2);
				if (d)
				{
					return d;
				}
			}
			s1++;
			s2++;
		} while (--n != 0);
	}
	return (0);
}

char const* memichr(char const* s, char ch, size_t n)
{
	if (n != 0) 
	{
		int chu = toupper_ascii(ch);
		do
		{
			int usl = *s;
			if (usl != chu)
			{
				usl = toupper_ascii(usl);
			}
			if (usl == chu)
			{
				return s;
			}
			s++;
		} while (--n != 0);
	}
	return (0);
}

char const* stristr(char const* in, char const* str)
{
	if (!in || !str)
	{
		return 0;
	}

	size_t len;
	int c = toupper_ascii(*str++);
	if (!c)
	{
		return in;
	}

	len = strlen(str);
	do {
		int sc;
		do {
			sc = toupper_ascii(*in++);
			if (!sc)
			{
				return 0;
			}
		} while (sc != c);
	} while (strnicmp(in, str, len) != 0);
	return in - 1;
}

//-----------------------------------------------------------------------------
// MurmurHash2, by Austin Appleby

// Note - This code makes a few assumptions about how your machine behaves -

// 1. We can read a 4-byte value from any address without crashing
// 2. sizeof(int) == 4

// And it has a few limitations -

// 1. It will not work incrementally.
// 2. It will not produce the same results on little-endian and big-endian
//    machines.

uint32_t murmur_hash(const void* key, int length, uint32_t seed)
{
	// 'm' and 'r' are mixing constants generated offline.
	// They're not really 'magic', they just happen to work well.

	const uint32_t m = 0x5bd1e995;
	const int r = 24;

	// Initialize the hash to a 'random' value

	uint32_t h = seed ^ length;

	// Mix 4 bytes at a time into the hash

	const uint8_t* data = (const uint8_t*)key;

	while (length >= 4)
	{
		uint32_t k = *(uint32_t*)data;

		k *= m; 
		k ^= k >> r; 
		k *= m; 

		h *= m; 
		h ^= k;

		data += 4;
		length -= 4;
	}

	// Handle the last few bytes of the input array

	switch(length)
	{
	case 3: h ^= uint32_t(data[2]) << 16;
	case 2: h ^= uint32_t(data[1]) << 8;
	case 1: h ^= (data[0]);
		h *= m;
	};

	// Do a few final mixes of the hash to ensure the last few
	// bytes are well-incorporated.

	h ^= h >> 13;
	h *= m;
	h ^= h >> 15;

	return h;
} 

//NOTE!!! CASE INSENSITIVE
uint32_t murmur_hash_cs(const void* key, int length, uint32_t seed)
{
	// 'm' and 'r' are mixing constants generated offline.
	// They're not really 'magic', they just happen to work well.

	const uint32_t m = 0x5bd1e995;
	const int r = 24;

	// Initialize the hash to a 'random' value

	uint32_t h = seed ^ length;

	// Mix 4 bytes at a time into the hash

	const uint8_t* data = (const uint8_t*)key;

	while (length >= 4)
	{
		uint32_t k = *(uint32_t*)data | 0x20202020;

		k *= m; 
		k ^= k >> r; 
		k *= m; 

		h *= m; 
		h ^= k;

		data += 4;
		length -= 4;
	}

	// Handle the last few bytes of the input array

	switch(length)
	{
	case 3: h ^= uint32_t(data[2] | 0x20) << 16;
	case 2: h ^= uint32_t(data[1] | 0x20) << 8;
	case 1: h ^= (data[0] | 0x20);
		h *= m;
	};

	// Do a few final mixes of the hash to ensure the last few
	// bytes are well-incorporated.

	h ^= h >> 13;
	h *= m;
	h ^= h >> 15;

	return h;
} 



void memcpy(uint8_t* dst, size_t dst_stride_bytes, uint8_t const* src, size_t src_stride_bytes, size_t element_size_bytes, size_t count)
{
    QASSERT(src && dst && count && element_size_bytes);
    src_stride_bytes = src_stride_bytes ? src_stride_bytes : element_size_bytes;
    dst_stride_bytes = dst_stride_bytes ? dst_stride_bytes : element_size_bytes;

	if ((((size_t)dst | (size_t)src) & 0x3) != 0 ||
        ((dst_stride_bytes | src_stride_bytes) & 0x3) != 0)
	{
		//slow, unaligned version
		for (size_t i = 0; i < count; i++)
		{
            ::memcpy(dst, src, element_size_bytes);
            dst += dst_stride_bytes;
            src += src_stride_bytes;
		}
		return;
	}

    switch (element_size_bytes)
	{
	case 1:
		for (size_t i = 0; i < count; i++)
		{
			(*dst) = (*src);
            dst += dst_stride_bytes;
            src += src_stride_bytes;
		}
		break;
	case 2:
		for (size_t i = 0; i < count; i++)
		{
			(*(uint16_t*)dst) = (*(uint16_t*)src);
            dst += dst_stride_bytes;
            src += src_stride_bytes;
		}
		break;
	case 4:
		for (size_t i = 0; i < count; i++)
		{
			(*(uint32_t*)dst) = (*(uint32_t*)src);
            dst += dst_stride_bytes;
            src += src_stride_bytes;
		}
		break;
	case 8:
		for (size_t i = 0; i < count; i++)
		{
			((uint32_t*)dst)[0] = ((uint32_t*)src)[0];
			((uint32_t*)dst)[1] = ((uint32_t*)src)[1];
            dst += dst_stride_bytes;
            src += src_stride_bytes;
		}
		break;
	case 12:
		for (size_t i = 0; i < count; i++)
		{
			((uint32_t*)dst)[0] = ((uint32_t*)src)[0];
			((uint32_t*)dst)[1] = ((uint32_t*)src)[1];
			((uint32_t*)dst)[2] = ((uint32_t*)src)[2];
            dst += dst_stride_bytes;
            src += src_stride_bytes;
		}
		break;
	case 16:
		for (size_t i = 0; i < count; i++)
		{
			((uint32_t*)dst)[0] = ((uint32_t*)src)[0];
			((uint32_t*)dst)[1] = ((uint32_t*)src)[1];
			((uint32_t*)dst)[2] = ((uint32_t*)src)[2];
			((uint32_t*)dst)[3] = ((uint32_t*)src)[3];
            dst += dst_stride_bytes;
            src += src_stride_bytes;
		}
		break;
	case 20:
		for (size_t i = 0; i < count; i++)
		{
			((uint32_t*)dst)[0] = ((uint32_t*)src)[0];
			((uint32_t*)dst)[1] = ((uint32_t*)src)[1];
			((uint32_t*)dst)[2] = ((uint32_t*)src)[2];
			((uint32_t*)dst)[3] = ((uint32_t*)src)[3];
			((uint32_t*)dst)[4] = ((uint32_t*)src)[4];
            dst += dst_stride_bytes;
            src += src_stride_bytes;
		}
		break;
	case 24:
		for (size_t i = 0; i < count; i++)
		{
			((uint32_t*)dst)[0] = ((uint32_t*)src)[0];
			((uint32_t*)dst)[1] = ((uint32_t*)src)[1];
			((uint32_t*)dst)[2] = ((uint32_t*)src)[2];
			((uint32_t*)dst)[3] = ((uint32_t*)src)[3];
			((uint32_t*)dst)[4] = ((uint32_t*)src)[4];
			((uint32_t*)dst)[5] = ((uint32_t*)src)[5];
            dst += dst_stride_bytes;
            src += src_stride_bytes;
		}
		break;
	case 28:
		for (size_t i = 0; i < count; i++)
		{
			((uint32_t*)dst)[0] = ((uint32_t*)src)[0];
			((uint32_t*)dst)[1] = ((uint32_t*)src)[1];
			((uint32_t*)dst)[2] = ((uint32_t*)src)[2];
			((uint32_t*)dst)[3] = ((uint32_t*)src)[3];
			((uint32_t*)dst)[4] = ((uint32_t*)src)[4];
			((uint32_t*)dst)[5] = ((uint32_t*)src)[5];
			((uint32_t*)dst)[6] = ((uint32_t*)src)[6];
            dst += dst_stride_bytes;
            src += src_stride_bytes;
		}
		break;
	case 32:
		for (size_t i = 0; i < count; i++)
		{
			((uint32_t*)dst)[0] = ((uint32_t*)src)[0];
			((uint32_t*)dst)[1] = ((uint32_t*)src)[1];
			((uint32_t*)dst)[2] = ((uint32_t*)src)[2];
			((uint32_t*)dst)[3] = ((uint32_t*)src)[3];
			((uint32_t*)dst)[4] = ((uint32_t*)src)[4];
			((uint32_t*)dst)[5] = ((uint32_t*)src)[5];
			((uint32_t*)dst)[6] = ((uint32_t*)src)[6];
			((uint32_t*)dst)[7] = ((uint32_t*)src)[7];
            dst += dst_stride_bytes;
            src += src_stride_bytes;
		}
		break;
	default:
		//slow version
		for (size_t i = 0; i < count; i++)
		{
            ::memcpy(dst, src, element_size_bytes);
            dst += dst_stride_bytes;
            src += src_stride_bytes;
		}
		break;
	}
}


//////////////////////////////////////////////////////////////////////////
}
}
