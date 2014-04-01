#include "Config.h"

#if CPU_TYPE == ATMEGA_2560

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/eeprom.h>
#include "debug/debug.h"
#include "board/boards/AVR_EEPROM.h"

namespace board
{

bool AVR_EEPROM::is_ready() const
{
	return eeprom_is_ready();
}

EEPROM::size_type AVR_EEPROM::get_size() const
{
	return 4096;
}

int8_t AVR_EEPROM::read_int8(offset_type& offset) const
{
	auto x = eeprom_read_byte((uint8_t*)offset);
	offset += 1;
	return *reinterpret_cast<int8_t*>(&x);
}
uint8_t AVR_EEPROM::read_uint8(offset_type& offset) const
{
	auto x = eeprom_read_byte((uint8_t*)offset);
	offset += 1;
	return x;
}

int16_t AVR_EEPROM::read_int16(offset_type& offset) const
{
	auto x = eeprom_read_word((uint16_t*)offset);
	offset += 2;
	return *reinterpret_cast<int16_t*>(&x);
}
uint16_t AVR_EEPROM::read_uint16(offset_type& offset) const
{
	auto x = eeprom_read_word((uint16_t*)offset);
	offset += 2;
	return x;
}

int32_t AVR_EEPROM::read_int32(offset_type& offset) const
{
	auto x = eeprom_read_dword((uint32_t*)offset);
	offset += 4;
	return *reinterpret_cast<int32_t*>(&x);
}
uint32_t AVR_EEPROM::read_uint32(offset_type& offset) const
{
	auto x = eeprom_read_dword((uint32_t*)offset);
	offset += 4;
	return x;
}

float AVR_EEPROM::read_float(offset_type& offset) const
{
	auto x = eeprom_read_dword((uint32_t*)offset);
	offset += 4;
	return *reinterpret_cast<float*>(&x);
}

void AVR_EEPROM::read(uint8_t* dst, size_type size, offset_type& offset) const
{
	eeprom_read_block(dst, (uint8_t*)offset, size);
	offset += size;
}

void AVR_EEPROM::write(int8_t x, offset_type& offset)
{
	auto y = *reinterpret_cast<uint8_t*>(&x);
	eeprom_update_byte((uint8_t*)offset, y);
	offset += 1;
}
void AVR_EEPROM::write(uint8_t x, offset_type& offset)
{
	auto y = x;
	eeprom_update_byte((uint8_t*)offset, y);
	offset += 1;
}
void AVR_EEPROM::write(int16_t x, offset_type& offset)
{
	auto y = *reinterpret_cast<uint16_t*>(&x);
	eeprom_update_word((uint16_t*)offset, y);
	offset += 2;
}
void AVR_EEPROM::write(uint16_t x, offset_type& offset)
{
	auto y = x;
	eeprom_update_word((uint16_t*)offset, y);
	offset += 2;
}
void AVR_EEPROM::write(int32_t x, offset_type& offset)
{
	auto y = *reinterpret_cast<uint32_t*>(&x);
	eeprom_update_dword((uint32_t*)offset, y);
	offset += 4;
}
void AVR_EEPROM::write(uint32_t x, offset_type& offset)
{
	auto y = x;
	eeprom_update_dword((uint32_t*)offset, y);
	offset += 4;
}
void AVR_EEPROM::write(float x, offset_type& offset)
{
	auto* __restrict y = reinterpret_cast<uint32_t*>(&x);
	eeprom_update_dword((uint32_t*)offset, *y);
	offset += 4;
}
void AVR_EEPROM::write(uint8_t const* src, size_type size, offset_type& offset)
{
	eeprom_update_block(src, (uint8_t*)offset, size);
	offset += size;
}

}

#endif

