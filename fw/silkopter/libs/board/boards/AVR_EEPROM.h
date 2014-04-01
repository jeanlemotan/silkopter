#pragma once

#include "board/EEPROM.h"

namespace board
{

class AVR_EEPROM : public EEPROM
{
public:
	size_type get_size() const;
    bool is_ready() const;

	////////////// READING /////////////////////////////

    int8_t 	read_int8(offset_type& offset) const = 0;
    uint8_t read_uint8(offset_type& offset) const = 0;
    int16_t read_int16(offset_type& offset) const = 0;
    uint16_t read_uint16(offset_type& offset) const = 0;
    int32_t read_int32(offset_type& offset) const = 0;
    uint32_t read_uint32(offset_type& offset) const = 0;
    float 	read_float(offset_type& offset) const = 0;
    void 	read(uint8_t* dst, size_type size, offset_type& offset) const = 0;

    ////////////// WRITING /////////////////////////////

    void 	write(int8_t x, offset_type& offset) = 0;
    void 	write(uint8_t x, offset_type& offset) = 0;
    void 	write(int16_t x, offset_type& offset) = 0;
    void 	write(uint16_t x, offset_type& offset) = 0;
    void 	write(int32_t x, offset_type& offset) = 0;
    void 	write(uint32_t x, offset_type& offset) = 0;
    void 	write(float x, offset_type& offset) = 0;
    void 	write(uint8_t const* src, size_type size, offset_type& offset) = 0;
};

}

