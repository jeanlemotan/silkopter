#include "QBaseStdAfx.h"
#include "QBase.h"

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


void encode_base_64(std::vector<uint8_t>& dst, uint8_t const* src, size_t srcSize)
{
	//dst.resize(0);
	dst.clear();
	dst.reserve(srcSize * 10 / 7);

	size_t i = 0;
	if (srcSize > 3)
	{
		// for each 3 bytes, create 4 encoded chars
		for (i = 0; i < srcSize - 3; i += 3)
		{
			uint32_t bitPattern = 0;
			bitPattern = src[i + 0] << 16;
			bitPattern += src[i + 1] << 8;
			bitPattern += src[i + 2];
			// create 4 encoded chars taking 6 bits each time
			for (uint32_t k = 0; k < 4; k++)
			{
				uint32_t index = bitPattern  >> (24 - (k + 1) * 6);
				index = index & (0x3F);
				QASSERT(index < 64);
				dst.push_back(s_char_64[index]);
			}
		}
	}
	if (i < srcSize)
	{
		uint32_t bitPattern = 0;
		bitPattern = src[i] << 16;
		if (i + 1 < srcSize) 
		{
			bitPattern += src[i + 1] << 8;
		}
		if (i + 2 < srcSize) 
		{
			bitPattern += src[i + 2];
		}
		// create 4 encoded chars taking 6 bits each time
		for (uint32_t k = 0; k < 4; k++)
		{
			uint32_t index = bitPattern  >> (24 - (k + 1) * 6);
			index = index & (0x3F);
			QASSERT(index < 64);
			dst.push_back(s_char_64[index]);
		}
	}

	// process padding
	size_t m = srcSize % 3;
	if (m == 1)
	{
		dst[dst.size() - 2] = '=';
		dst[dst.size() - 1] = '=';
	}
	else if (m == 2)
	{
		dst[dst.size() - 1] = '=';
	}
}
void decode_base_64(std::vector<uint8_t>& dst, uint8_t const* src, size_t srcSize)
{
	//dst.resize(0);
	dst.clear();
	dst.reserve(srcSize);

	QASSERT(srcSize % 4 == 0);

	// for each 4 encoded chars, create 3 bytes
	size_t i = 0;
	if (srcSize > 4)
	{
		for (i = 0; i < srcSize - 4; i += 4)
		{
			uint32_t idx0 = s_char_64_rev[src[i + 0]];
			uint32_t idx1 = s_char_64_rev[src[i + 1]];
			uint32_t idx2 = s_char_64_rev[src[i + 2]];
			uint32_t idx3 = s_char_64_rev[src[i + 3]];
			// create bitPattern from encoded chars
			uint32_t bitPattern;
			bitPattern  = (idx0 << 18);
			bitPattern += (idx1 << 12);
			bitPattern += (idx2 <<  6);
			bitPattern += (idx3      );
			// split 3 bytes
			dst.push_back((bitPattern >> 16) & 0xFF);
			dst.push_back((bitPattern >>  8) & 0xFF);
			dst.push_back((bitPattern      ) & 0xFF);
		}
	}
	if (i + 4 <= srcSize)
	{
		uint32_t idx0 = s_char_64_rev[src[i + 0]];
		uint32_t idx1 = s_char_64_rev[src[i + 1]];
		uint32_t idx2 = s_char_64_rev[src[i + 2]];
		uint32_t idx3 = s_char_64_rev[src[i + 3]];
		// create bitPattern from encoded chars
		uint32_t bitPattern;
		bitPattern  = (idx0 << 18);
		bitPattern += (idx1 << 12);
		bitPattern += (idx2 <<  6);
		bitPattern += (idx3      );
		// split 3 bytes
		dst.push_back((bitPattern >> 16) & 0xFF);
		if (src[i + 2] != '=')
		{
			dst.push_back((bitPattern >>  8) & 0xFF);
		}
		if (src[i + 3] != '=')
		{
			dst.push_back((bitPattern) & 0xFF);
		}	
	}
}


/*
void encodeBase64(data::OStream& dst, data::IStream& src)
{
	size_t size = src.getSize() - src.getPos();
	// for each 3 bytes, create 4 encoded chars
	size_t i = 0;
	if (size > 4)
	{
		for (i = 0; i < size - 3; i += 3)
		{
			uint32_t bitPattern = 0;
			uint8_t d[3];
			src >> d;
			bitPattern = d[0] << 16;
			bitPattern += d[1] << 8;
			bitPattern += d[2];
			// create 4 encoded chars taking 6 bits each time
			uint8_t out[4];
			for (uint32_t k = 0; k < 4; k++)
			{
				uint32_t index = bitPattern  >> (24 - (k + 1) * 6);
				index = index & (0x3F);
				QASSERT(index < 64);
				out[k] = sChar64[index];
			}
			dst << out;
		}
	}
	if (i < size)
	{
		uint32_t bitPattern = 0;
		uint8_t i0, i1, i2;
		src >> i0;
		bitPattern = i0 << 16;
		if (i + 1 < size) 
		{
			src >> i1;
			bitPattern += i1 << 8;
		}
		if (i + 2 < size) 
		{
			src >> i2;
			bitPattern += i2;
		}
		// create 4 encoded chars taking 6 bits each time
		uint8_t out[4];
		for (uint32_t k = 0; k < 4; k++)
		{
			uint32_t index = bitPattern  >> (24 - (k + 1) * 6);
			index = index & (0x3F);
			QASSERT(index < 64);
			out[k] = sChar64[index];
		}
		dst << out;
	}

	// process padding
	size_t m = size % 3;
	if (m == 1)
	{
		dst.seekRelative(-2);
		dst << '=';
		dst << '=';
	}
	else if (m == 2)
	{
		dst.seekRelative(-1);
		dst << '=';
	}
}

void decodeBase64(data::OStream& dst, data::IStream& src)
{
	size_t size = src.getSize() - src.getPos();
	QASSERT(size % 4 == 0);

	size_t i = 0;
	if (size > 4)
	{
		// for each 4 encoded chars, create 3 bytes
		for (i = 0; i < size - 4; i+=4)
		{
			uint8_t d[4];
			src >> d;
			uint32_t idx0 = sChar64Rev[d[0]];
			uint32_t idx1 = sChar64Rev[d[1]];
			uint32_t idx2 = sChar64Rev[d[2]];
			uint32_t idx3 = sChar64Rev[d[3]];
			// create bitPattern from encoded chars
			uint32_t bitPattern;
			bitPattern  = (idx0 << 18);
			bitPattern += (idx1 << 12);
			bitPattern += (idx2 <<  6);
			bitPattern += (idx3      );
			// split 3 bytes
			uint8_t out[3];
			out[0] = (uint8_t)((bitPattern >> 16) & 0xFF);
			out[1] = (uint8_t)((bitPattern >>  8) & 0xFF);
			out[2] = (uint8_t)((bitPattern      ) & 0xFF);
			dst << out;
		}
	}
	if (i + 4 <= size)
	{
		uint8_t i0, i1, i2, i3;
		src >> i0 >> i1 >> i2 >> i3;
		uint32_t idx0 = sChar64Rev[i0];
		uint32_t idx1 = sChar64Rev[i1];
		uint32_t idx2 = sChar64Rev[i2];
		uint32_t idx3 = sChar64Rev[i3];
		// create bitPattern from encoded chars
		uint32_t bitPattern;
		bitPattern  = (idx0 << 18);
		bitPattern += (idx1 << 12);
		bitPattern += (idx2 <<  6);
		bitPattern += (idx3      );
		// split 3 bytes
		dst << (uint8_t)((bitPattern >> 16) & 0xFF);
		if (i2 != '=')
		{
			dst << (uint8_t)((bitPattern >>  8) & 0xFF);
		}
		if (i3 != '=')
		{
			dst << (uint8_t)((bitPattern      ) & 0xFF);
		}
	}
}
*/

}
}
