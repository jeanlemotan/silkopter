#pragma once

#include "board/EEPROM.h"
#include <fstream>

namespace board
{

class Sim_EEPROM : public EEPROM
{
public:

	Sim_EEPROM(std::string const& filename);

	size_type get_capacity() const;
    bool is_ready() const;

    void 	read(uint8_t* dst, size_type size, offset_type offset) const;
    void 	write(uint8_t const* src, size_type size, offset_type offset);

private:
	std::string m_filename;
	std::fstream m_file;
};

}

