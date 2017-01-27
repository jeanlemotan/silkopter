#pragma once

inline uint16_t q_crc16_update(uint16_t crc, uint8_t data)
{
    crc ^= data;
    for (int i = 7; i >= 0; --i)
    {
        if (crc & 1)
        {
            crc = (crc >> 1) ^ 0xA001;
        }
        else
        {
            crc >>= 1;
        }
    }
    return crc;
}
inline uint8_t q_crc_ibutton_update(uint8_t crc, uint8_t data)
{
    crc ^= data;
    for (int i = 7; i >= 0; --i)
    {
        if (crc & 0x01)
        {
            crc = (crc >> 1) ^ 0x8C;
        }
        else
        {
            crc >>= 1;
        }
    }
    return crc;
}

namespace q
{
namespace util
{
    typedef uint8_t crc8_t;
	typedef uint16_t crc16_t;

    template<typename T> inline void update_crc(T& crc, uint8_t v); //NOT IMPLEMENTED on purpose
	
    template<> inline void update_crc(crc8_t& crc, uint8_t v)
	{
        crc = q_crc_ibutton_update(crc, v);
	}
    template<> inline void update_crc(crc16_t& crc, uint8_t v)
    {
        crc = q_crc16_update(crc, v);
    }

    template<typename T> inline void update_crc(T& crc, int8_t v)
	{
		auto const* __restrict _v = reinterpret_cast<uint8_t const*>(&v);
        update_crc(crc, *_v);
	}
    template<typename T> inline void update_crc(T& crc, uint16_t v)
	{
		auto const* __restrict _v = reinterpret_cast<uint8_t const*>(&v);
        update_crc(crc, _v[0]);
        update_crc(crc, _v[1]);
	}
    template<typename T> inline void update_crc(T& crc, int16_t v)
	{
		auto const* __restrict _v = reinterpret_cast<uint8_t const*>(&v);
        update_crc(crc, _v[0]);
        update_crc(crc, _v[1]);
    }
    template<typename T> inline void update_crc(T& crc, uint32_t v)
	{
		auto const* __restrict _v = reinterpret_cast<uint8_t const*>(&v);
        update_crc(crc, _v[0]);
        update_crc(crc, _v[1]);
        update_crc(crc, _v[2]);
        update_crc(crc, _v[3]);
	}
    template<typename T> inline void update_crc(T& crc, int32_t v)
	{
		auto const* __restrict _v = reinterpret_cast<uint8_t const*>(&v);
        update_crc(crc, _v[0]);
        update_crc(crc, _v[1]);
        update_crc(crc, _v[2]);
        update_crc(crc, _v[3]);
    }
    template<typename T> inline void update_crc(T& crc, float v)
	{
		auto const* __restrict _v = reinterpret_cast<uint8_t const*>(&v);
        update_crc(crc, _v[0]);
        update_crc(crc, _v[1]);
        update_crc(crc, _v[2]);
        update_crc(crc, _v[3]);
    }
	
    template<typename CRC_T, class T>
    CRC_T compute_crc(T const* data, size_t size)
	{
        CRC_T crc = 0;
		T const* ptr = data;
		T const* end = ptr + size;
		for (; ptr < end; ++ptr)
		{
			update_crc(crc, *ptr);
		}
		return crc;
	}
    template<typename CRC_T, class Container>
    CRC_T compute_crc(Container const& container, size_t size)
	{
        CRC_T crc = 0;
		for (size_t i = 0; i < size; i++)
		{
			update_crc(crc, container[i]);
		}
		return crc;
	}
	

}
}
