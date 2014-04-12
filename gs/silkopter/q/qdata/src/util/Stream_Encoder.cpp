#include "QDataStdAfx.h"
#include "data/Sink.h"
#include "data/Source.h"
#include "util/Stream_Encoder.h"

namespace q
{
namespace util
{

// http://en.wikipedia.org/wiki/Base64

// Transform 2^6 values into 64 unique chars
static uint8_t s_char_64[64] =
{
'A',//0
'B',
'C',
'D',
'E',
'F',
'G',
'H',
'I',
'J',
'K',//10
'L',
'M',
'N',
'O',
'P',
'Q',
'R',
'S',
'T',
'U',//20
'V',
'W',
'X',
'Y',
'Z',
'a',//26
'b',
'c',
'd',
'e',
'f',
'g',
'h',
'i',
'j',
'k',//36
'l',
'm',
'n',
'o',
'p',
'q',
'r',
's',
't',
'u',//46
'v',
'w',
'x',
'y',
'z',
'0',//52
'1',
'2',
'3',
'4',
'5',
'6',
'7',
'8',
'9',
'+',//62
'/',
};

static const uint8_t s_char_64_rev[256] =
{
	0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
	0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
	62, 0,  0,  0,  63, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 0,  0,  0,  0, 0,  0,  0,
	0 , 1,  2,  3,  4,  5,  6,  7,  8,  9,  10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21,
	22, 23, 24, 25, 0,  0,  0,  0,  0,  0,  26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37,
	38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 0,  0,  0,  0,  0,  0,  0,  0,  
	0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
	0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
	0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
	0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
	0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
	0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0
};

void encode_base_64(data::Sink& sink, data::Source& source)
{
	size_t size = source.get_size() - source.get_offset();
	// for each 3 bytes, create 4 encoded chars
	size_t i = 0;
	if (size > 4)
	{
		for (i = 0; i < size - 3; i += 3)
		{
			uint32_t bit_pattern = 0;
			uint8_t d[3];
			source >> d;
			bit_pattern = d[0] << 16;
			bit_pattern += d[1] << 8;
			bit_pattern += d[2];
			// create 4 encoded chars taking 6 bits each time
			uint8_t out[4];
			for (uint32_t k = 0; k < 4; k++)
			{
				uint32_t index = bit_pattern  >> (24 - (k + 1) * 6);
				index = index & (0x3F);
				QASSERT(index < 64);
				out[k] = s_char_64[index];
			}
			sink << out;
		}
	}
	if (i < size)
	{
		uint32_t bit_pattern = 0;
		uint8_t i0, i1, i2;
		source >> i0;
		bit_pattern = i0 << 16;
		if (i + 1 < size) 
		{
			source >> i1;
			bit_pattern += i1 << 8;
		}
		if (i + 2 < size) 
		{
			source >> i2;
			bit_pattern += i2;
		}
		// create 4 encoded chars taking 6 bits each time
		uint8_t out[4];
		for (uint32_t k = 0; k < 4; k++)
		{
			uint32_t index = bit_pattern  >> (24 - (k + 1) * 6);
			index = index & (0x3F);
			QASSERT(index < 64);
			out[k] = s_char_64[index];
		}
		sink << out;
	}

	// process padding
	size_t m = size % 3;
	if (m == 1)
	{
		sink.seek_relative(-2);
		sink << '=';
		sink << '=';
	}
	else if (m == 2)
	{
		sink.seek_relative(-1);
		sink << '=';
	}
}

void decode_base_64(data::Sink& sink, data::Source& source)
{
	size_t size = source.get_size() - source.get_offset();
	QASSERT(size % 4 == 0);

	size_t i = 0;
	if (size > 4)
	{
		// for each 4 encoded chars, create 3 bytes
		for (i = 0; i < size - 4; i+=4)
		{
			uint8_t d[4];
			source >> d;
			uint32_t idx0 = s_char_64_rev[d[0]];
			uint32_t idx1 = s_char_64_rev[d[1]];
			uint32_t idx2 = s_char_64_rev[d[2]];
			uint32_t idx3 = s_char_64_rev[d[3]];
			// create bitPattern from encoded chars
			uint32_t bit_pattern = (idx0 << 18);
			bit_pattern += (idx1 << 12);
			bit_pattern += (idx2 <<  6);
			bit_pattern += (idx3      );
			// split 3 bytes
			uint8_t out[3];
			out[0] = (uint8_t)((bit_pattern >> 16) & 0xFF);
			out[1] = (uint8_t)((bit_pattern >>  8) & 0xFF);
			out[2] = (uint8_t)((bit_pattern      ) & 0xFF);
			sink << out;
		}
	}
	if (i + 4 <= size)
	{
		uint8_t i0, i1, i2, i3;
		source >> i0 >> i1 >> i2 >> i3;
		uint32_t idx0 = s_char_64_rev[i0];
		uint32_t idx1 = s_char_64_rev[i1];
		uint32_t idx2 = s_char_64_rev[i2];
		uint32_t idx3 = s_char_64_rev[i3];
		// create bitPattern from encoded chars
		uint32_t bit_pattern = (idx0 << 18);
		bit_pattern += (idx1 << 12);
		bit_pattern += (idx2 <<  6);
		bit_pattern += (idx3      );
		// split 3 bytes
		sink << (uint8_t)((bit_pattern >> 16) & 0xFF);
		if (i2 != '=')
		{
			sink << (uint8_t)((bit_pattern >>  8) & 0xFF);
		}
		if (i3 != '=')
		{
			sink << (uint8_t)((bit_pattern      ) & 0xFF);
		}
	}
}

}
}
