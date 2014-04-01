#pragma once

#include "util/Noncopyable.h"

namespace board
{

class EEPROM : util::Noncopyable
{
public:
	typedef uint16_t size_type;
	typedef uint16_t offset_type;

	virtual size_type get_size() const = 0;
    virtual bool is_ready() const = 0;

	////////////// READING /////////////////////////////

    virtual int8_t 	read_int8(offset_type& offset) const = 0;
    virtual uint8_t read_uint8(offset_type& offset) const = 0;
    virtual int16_t read_int16(offset_type& offset) const = 0;
    virtual uint16_t read_uint16(offset_type& offset) const = 0;
    virtual int32_t read_int32(offset_type& offset) const = 0;
    virtual uint32_t read_uint32(offset_type& offset) const = 0;
    virtual float 	read_float(offset_type& offset) const = 0;
    virtual void 	read(uint8_t* dst, size_type size, offset_type& offset) const = 0;

    ////////////// WRITING /////////////////////////////

    virtual void 	write(int8_t x, offset_type& offset) = 0;
    virtual void 	write(uint8_t x, offset_type& offset) = 0;
    virtual void 	write(int16_t x, offset_type& offset) = 0;
    virtual void 	write(uint16_t x, offset_type& offset) = 0;
    virtual void 	write(int32_t x, offset_type& offset) = 0;
    virtual void 	write(uint32_t x, offset_type& offset) = 0;
    virtual void 	write(float x, offset_type& offset) = 0;
    virtual void 	write(uint8_t const* src, size_type size, offset_type& offset) = 0;
};

}

