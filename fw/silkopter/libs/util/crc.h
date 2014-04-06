#pragma once

#include <util/crc16.h>

namespace util
{
	typedef uint16_t crc_t;
	
	inline void update_crc(crc_t& crc, uint8_t v)
	{
		crc = _crc16_update(crc, v);
	}
	inline void update_crc(crc_t& crc, int8_t v)
	{
		auto const* __restrict _v = reinterpret_cast<uint8_t const*>(&v);
		crc = _crc16_update(crc, *_v);
	}
	inline void update_crc(crc_t& crc, uint16_t v)
	{
		auto const* __restrict _v = reinterpret_cast<uint8_t const*>(&v);
		crc = _crc16_update(crc, _v[0]);
		crc = _crc16_update(crc, _v[1]);
	}
	inline void update_crc(crc_t& crc, int16_t v)
	{
		auto const* __restrict _v = reinterpret_cast<uint8_t const*>(&v);
		crc = _crc16_update(crc, _v[0]);
		crc = _crc16_update(crc, _v[1]);
	}
	inline void update_crc(crc_t& crc, uint32_t v)
	{
		auto const* __restrict _v = reinterpret_cast<uint8_t const*>(&v);
		crc = _crc16_update(crc, _v[0]);
		crc = _crc16_update(crc, _v[1]);
		crc = _crc16_update(crc, _v[2]);
		crc = _crc16_update(crc, _v[3]);
	}
	inline void update_crc(crc_t& crc, int32_t v)
	{
		auto const* __restrict _v = reinterpret_cast<uint8_t const*>(&v);
		crc = _crc16_update(crc, _v[0]);
		crc = _crc16_update(crc, _v[1]);
		crc = _crc16_update(crc, _v[2]);
		crc = _crc16_update(crc, _v[3]);
	}
	inline void update_crc(crc_t& crc, float v)
	{
		auto const* __restrict _v = reinterpret_cast<uint8_t const*>(&v);
		crc = _crc16_update(crc, _v[0]);
		crc = _crc16_update(crc, _v[1]);
		crc = _crc16_update(crc, _v[2]);
		crc = _crc16_update(crc, _v[3]);
	}
	
	template<class T>
	crc_t compute_crc(T const* data, size_t size)
	{
		crc_t crc = 0;
		T const* ptr = data;
		T const* end = ptr + size;
		for (; ptr < end; ++ptr)
		{
			update_crc(crc, *ptr);
		}
		return crc;
	}
	template<class Container>
	crc_t compute_crc(Container const& container)
	{
		crc_t crc = 0;
		for (size_t i = 0; i < container.size(); i++)
		{
			update_crc(crc, container[i]);
		}
		return crc;
	}
	template<class Container>
	crc_t compute_crc(Container const& container, size_t size)
	{
		crc_t crc = 0;
		for (size_t i = 0; i < size; i++)
		{
			update_crc(crc, container[i]);
		}
		return crc;
	}
	
	
}
