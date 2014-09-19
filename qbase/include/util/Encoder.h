#pragma once

namespace q
{
namespace util
{
    extern const uint8_t s_char_64_rev[256];
    extern const uint8_t s_char_64[64];

    template <typename _II, typename _OI>
    void encode_base_64(_II first, _II last, _OI result)
    {
        size_t i = 0;
        auto src_size = std::distance(last, first);
        if (src_size > 3)
        {
            // for each 3 bytes, create 4 encoded chars
            for (i = 0; i < src_size - 3; i += 3)
            {
                uint32_t bit_pattern = uint32_t(*first++) << 16;
                bit_pattern += uint32_t(*first++) << 8;
                bit_pattern += *first++;
                // create 4 encoded chars taking 6 bits each time
                for (uint8_t k = 0; k < 4; k++)
                {
                    uint32_t index = bit_pattern  >> (24 - (k + 1) * 6);
                    index = index & (0x3F);
                    QASSERT(index < 64);
                    *result++ = s_char_64[index];
                }
            }
        }
        if (i < src_size)
        {
            uint32_t bit_pattern = uint32_t(*first++) << 16;
            if (i + 1 < src_size)
            {
                bit_pattern += uint32_t(*first++) << 8;
            }
            if (i + 2 < src_size)
            {
                bit_pattern += *first++;
            }
            // create 4 encoded chars taking 6 bits each time
            for (uint8_t k = 0; k < 4; k++)
            {
                uint32_t index = bit_pattern  >> (24 - (k + 1) * 6);
                index = index & (0x3F);
                QASSERT(index < 64);
                *result++ = s_char_64[index];
            }
        }

        // process padding
        size_t m = src_size % 3;
        switch (m)
        {
        case 3: *result++ = '='; //fallthrough
        case 2: *result++ = '='; //fallthrough
        case 1: *result++ = '='; //fallthrough
        }
    }

    template <typename _II, typename _OI>
    void decode_base_64(_II first, _II last, _OI result)
    {
        auto src_size = std::distance(last, first);
        QASSERT(src_size % 4 == 0);

        // for each 4 encoded chars, create 3 bytes
        size_t i = 0;
        if (src_size > 4)
        {
            for (i = 0; i < src_size - 4; i += 4)
            {
                uint32_t idx0 = s_char_64_rev[*first++];
                uint32_t idx1 = s_char_64_rev[*first++];
                uint32_t idx2 = s_char_64_rev[*first++];
                uint32_t idx3 = s_char_64_rev[*first++];
                // create bitPattern from encoded chars
                uint32_t bit_pattern  = (idx0 << 18);
                bit_pattern += (idx1 << 12);
                bit_pattern += (idx2 <<  6);
                bit_pattern += (idx3      );
                // split 3 bytes
                *result++ = (bit_pattern >> 16) & 0xFF;
                *result++ = (bit_pattern >>  8) & 0xFF;
                *result++ = (bit_pattern      ) & 0xFF;
            }
        }
        if (i + 4 <= src_size)
        {
            auto f0 = *first++;
            auto f1 = *first++;
            auto f2 = *first++;
            auto f3 = *first++;
            uint32_t idx0 = s_char_64_rev[f0];
            uint32_t idx1 = s_char_64_rev[f1];
            uint32_t idx2 = s_char_64_rev[f2];
            uint32_t idx3 = s_char_64_rev[f3];
            // create bitPattern from encoded chars
            uint32_t bit_pattern  = (idx0 << 18);
            bit_pattern += (idx1 << 12);
            bit_pattern += (idx2 <<  6);
            bit_pattern += (idx3      );
            // split 3 bytes
            *result++ = (bit_pattern >> 16) & 0xFF;
            if (f2 != '=')
            {
                *result++ = (bit_pattern >>  8) & 0xFF;
            }
            if (f3 != '=')
            {
                *result++ = (bit_pattern) & 0xFF;
            }
        }
    }

	template<class String_T>
	auto encode_base_64(String_T const& src) -> String_T
	{
		std::vector<uint8_t> dst;
        dst.reserve(src.size());
        encode_base_64(src.begin(), src.end(), std::back_inserter(dst));
		return dst.empty() ? String_T() : String_T((char const*)&dst[0], (char const*)&dst[0] + dst.size());
	}
	template<class String_T>
	auto decode_base_64(String_T const& src) -> String_T
	{
        std::vector<uint8_t> dst;
        dst.reserve(src.size());
        decode_base_64(src.begin(), src.end(), std::back_inserter(dst));
		return dst.empty() ? String_T() : String_T((char const*)&dst[0], (char const*)&dst[0] + dst.size());
	}

}
}
