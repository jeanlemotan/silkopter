#pragma once

namespace q
{
namespace util
{
    extern const uint8_t s_char_64_rev[256];
    extern const uint8_t s_char_64[64];

    inline size_t compute_base64_encoded_size(size_t n)
    {
        return ((4 * n / 3) + 3) & (~3);
    }
    inline size_t compute_base64_max_decoded_size(size_t n)
    {
        if (n & 3)
        {
            QASSERT(0);
            return 0;
        }
        return 3 * n / 4;
    }

    void encode_base64(const char* src, size_t size, char* dst);

    template <typename _II, typename _OI>
    void encode_base64(_II first, _II last, _OI result)
    {
        size_t i = 0;
        size_t src_size = std::distance(first, last);
        while (src_size >= 3)
        {
            uint32_t bit_pattern = static_cast<uint32_t>(*first++) << 16;
            bit_pattern += static_cast<uint32_t>(*first++) << 8;
            bit_pattern += static_cast<uint32_t>(*first++);

            src_size -= 3;

            // create 4 encoded chars taking 6 bits each time
            *result++ = s_char_64[(bit_pattern  >> 18) & 0x3F];
            *result++ = s_char_64[(bit_pattern  >> 12) & 0x3F];
            *result++ = s_char_64[(bit_pattern  >> 6 ) & 0x3F];
            *result++ = s_char_64[(bit_pattern       ) & 0x3F];
        }

        if (src_size == 2)
        {
            uint32_t bit_pattern = static_cast<uint32_t>(*first++) << 16;
            bit_pattern += static_cast<uint32_t>(*first++) << 8;

            // create 4 encoded chars taking 6 bits each time
            *result++ = s_char_64[(bit_pattern  >> 18) & 0x3F];
            *result++ = s_char_64[(bit_pattern  >> 12) & 0x3F];
            *result++ = s_char_64[(bit_pattern  >> 6 ) & 0x3F];
            *result++ = '=';
        }
        else if (src_size == 1)
        {
            uint32_t bit_pattern = static_cast<uint32_t>(*first++) << 16;

            // create 4 encoded chars taking 6 bits each time
            *result++ = s_char_64[(bit_pattern  >> 18) & 0x3F];
            *result++ = s_char_64[(bit_pattern  >> 12) & 0x3F];
            *result++ = '=';
            *result++ = '=';
        }
    }

    template <typename _II, typename _OI>
    _OI decode_base64(_II first, _II last, _OI result)
    {
        auto src_size = std::distance(first, last);
        QASSERT(src_size % 4 == 0);

        // for each 4 encoded chars, create 3 bytes
        while (src_size >= 6) //4 bytes processes, and maybe 2 bytes worth of '=' that we don't want to process
        {
            uint8_t idx0 = s_char_64_rev[static_cast<uint8_t>(*first++)];
            uint8_t idx1 = s_char_64_rev[static_cast<uint8_t>(*first++)];
            uint8_t idx2 = s_char_64_rev[static_cast<uint8_t>(*first++)];
            uint8_t idx3 = s_char_64_rev[static_cast<uint8_t>(*first++)];
            src_size -= 4;

            // create bit_pattern from encoded chars
            uint32_t bit_pattern  = (idx0 << 18);
            bit_pattern          += (idx1 << 12);
            bit_pattern          += (idx2 <<  6);
            bit_pattern          += (idx3      );
            // split 3 bytes
            *result++ = (bit_pattern >> 16) & 0xFF;
            *result++ = (bit_pattern >>  8) & 0xFF;
            *result++ = (bit_pattern      ) & 0xFF;
        }
        if (src_size >= 4)
        {
            uint8_t f0 = static_cast<uint8_t>(*first++);
            uint8_t f1 = static_cast<uint8_t>(*first++);
            uint8_t f2 = static_cast<uint8_t>(*first++);
            uint8_t f3 = static_cast<uint8_t>(*first++);
            uint8_t idx0 = s_char_64_rev[f0];
            uint8_t idx1 = s_char_64_rev[f1];
            uint8_t idx2 = s_char_64_rev[f2];
            uint8_t idx3 = s_char_64_rev[f3];
            // create bit_pattern from encoded chars
            uint32_t bit_pattern  = (idx0 << 18);
            bit_pattern          += (idx1 << 12);
            bit_pattern          += (idx2 <<  6);
            bit_pattern          += (idx3      );
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
        return result;
    }

	template<class String_T>
    auto encode_base64(String_T const& src) -> String_T
	{
		std::vector<uint8_t> dst;
        dst.reserve(src.size());
        encode_base64(src.begin(), src.end(), std::back_inserter(dst));
		return dst.empty() ? String_T() : String_T((char const*)&dst[0], (char const*)&dst[0] + dst.size());
	}
	template<class String_T>
    auto decode_base64(String_T const& src) -> String_T
	{
        std::vector<uint8_t> dst;
        dst.reserve(src.size());
        decode_base64(src.begin(), src.end(), std::back_inserter(dst));
		return dst.empty() ? String_T() : String_T((char const*)&dst[0], (char const*)&dst[0] + dst.size());
	}

}
}
