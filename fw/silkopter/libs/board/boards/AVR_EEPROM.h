#pragma once

#include "board/EEPROM.h"

namespace board
{

class AVR_EEPROM : public EEPROM
{
public:
	size_type get_capacity() const;
    bool is_ready() const;

	////////////// READING /////////////////////////////

    void 	read(uint8_t* dst, size_type size, offset_type offset) const;

    ////////////// WRITING /////////////////////////////

    void 	write(uint8_t const* src, size_type size, offset_type offset);
};

}

