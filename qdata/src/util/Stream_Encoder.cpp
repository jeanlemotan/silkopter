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
extern const uint8_t s_char_64[64];
extern const uint8_t s_char_64_rev[256];

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
