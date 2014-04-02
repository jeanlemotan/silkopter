#pragma once

#include "util/Noncopyable.h"

namespace board
{

class EEPROM : util::Noncopyable
{
public:
	typedef size_t size_type;
	typedef size_t offset_type;

	virtual size_type get_capacity() const = 0;
    virtual bool is_ready() const = 0;

	////////////// READING /////////////////////////////

    virtual void 	read(uint8_t* dst, size_type size, offset_type offset) const = 0;
	
	template<class T>
	void read(T& dst, offset_type offset) const
	{
		read(reinterpret_cast<uint8_t*>(&dst), sizeof(T), offset);
	}
	template<class T>
	void read_advance(T& dst, offset_type& offset) const
	{
		read(reinterpret_cast<uint8_t*>(&dst), sizeof(T), offset);
		offset += sizeof(T);
	}

    ////////////// WRITING /////////////////////////////

    virtual void 	write(uint8_t const* src, size_type size, offset_type offset) = 0;

	template<class T>
	void write(T const& src, offset_type offset)
	{
		write(reinterpret_cast<uint8_t const*>(&src), sizeof(T), offset);
	}
	template<class T>
	void write_advance(T const& src, offset_type& offset)
	{
		write(reinterpret_cast<uint8_t const*>(&src), sizeof(T), offset);
		offset += sizeof(T);
	}
};

}

