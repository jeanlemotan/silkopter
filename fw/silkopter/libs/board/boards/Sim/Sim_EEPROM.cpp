#include "Config.h"

#if CPU_TYPE == SIM

#include "debug/debug.h"
#include "board/boards/Sim/Sim_EEPROM.h"

namespace board
{

Sim_EEPROM::Sim_EEPROM(std::string const& filename, size_t size)
	: m_filename(filename)
	, m_size(size)
{
	m_buffer.resize(m_size);

	std::fstream file(filename, std::ios::in | std::ios::binary);
	file.read(reinterpret_cast<char*>(m_buffer.data()), m_size);
	file.close();
}

Sim_EEPROM::~Sim_EEPROM()
{
}

void Sim_EEPROM::flush()
{
	std::fstream file(m_filename, std::ios::out | std::ios::binary | std::ios::trunc);
	file.write(reinterpret_cast<char*>(m_buffer.data()), m_size);
	file.close();
}

bool Sim_EEPROM::is_ready() const
{
	return true;
}

EEPROM::size_type Sim_EEPROM::get_capacity() const
{
	return m_size;
}

void Sim_EEPROM::read(uint8_t* dst, size_type size, offset_type offset) const
{
	ASSERT(offset + size < m_size);
	memcpy(dst, m_buffer.data() + offset, size);
}

void Sim_EEPROM::write(uint8_t const* src, size_type size, offset_type offset)
{
	ASSERT(offset + size < m_size);
	memcpy(m_buffer.data() + offset, src, size);
	flush();
}

}

#endif

