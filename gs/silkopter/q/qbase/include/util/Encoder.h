#pragma once

namespace q
{
namespace util
{
	void encode_base_64(std::vector<uint8_t>& dst, uint8_t const* src, size_t src_size);
	void decode_base_64(std::vector<uint8_t>& dst, uint8_t const* src, size_t src_size);

	template<class String_T>
	auto encode_base_64(String_T const& src) -> String_T
	{
		std::vector<uint8_t> dst;
		encode_base_64(dst, (uint8_t const*)src.c_str(), src.size());
		return dst.empty() ? String_T() : String_T((char const*)&dst[0], (char const*)&dst[0] + dst.size());
	}
	template<class String_T>
	auto decode_base_64(String_T const& src) -> String_T
	{
		std::vector<uint8_t> dst;
		decode_base_64(dst, (uint8_t const*)src.c_str(), src.size());
		return dst.empty() ? String_T() : String_T((char const*)&dst[0], (char const*)&dst[0] + dst.size());
	}

}
}