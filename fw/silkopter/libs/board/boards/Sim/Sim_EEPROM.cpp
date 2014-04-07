#include "Config.h"

#if CPU_TYPE == SIM

#include "debug/debug.h"
#include "board/boards/Sim/Sim_EEPROM.h"

namespace board
{

Sim_EEPROM::Sim_EEPROM(std::string const& filename)
	: m_filename(filename)
{
	m_file.open(filename, std::ios::in | std::ios::out | std::ios::binary);
}

bool Sim_EEPROM::is_ready() const
{
	return m_file.is_open();
}

EEPROM::size_type Sim_EEPROM::get_capacity() const
{
	return 4096;
}

void Sim_EEPROM::read(uint8_t* dst, size_type size, offset_type offset) const
{
	ASSERT(0);
}

void Sim_EEPROM::write(uint8_t const* src, size_type size, offset_type offset)
{
	ASSERT(0);
}

}

#endif

