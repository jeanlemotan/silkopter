#include "Config.h"

#if BOARD_TYPE == CRIUS_AIOP2

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/eeprom.h>
#include "debug/debug.h"
#include "board/eeprom.h"

namespace board
{
namespace eeprom
{


static bool s_is_initialized = false;
void init()
{
	if (s_is_initialized)
	{
		return;
	}
	s_is_initialized = true;

}

bool is_ready()
{
	ASSERT(s_is_initialized);
	return eeprom_is_ready();
}

size_type get_size()
{
	ASSERT(s_is_initialized);
	return 4096;
}

int8_t 	read_int8(offset_type& offset)
{
	ASSERT(s_is_initialized);
	auto x = eeprom_read_byte((uint8_t*)offset);
	offset += 1;
	return *reinterpret_cast<int8_t*>(&x);
}
uint8_t read_uint8(offset_type& offset)
{
	ASSERT(s_is_initialized);
	auto x = eeprom_read_byte((uint8_t*)offset);
	offset += 1;
	return x;
}

int16_t read_int16(offset_type& offset)
{
	ASSERT(s_is_initialized);
	auto x = eeprom_read_word((uint16_t*)offset);
	offset += 2;
	return *reinterpret_cast<int16_t*>(&x);
}
uint16_t read_uint16(offset_type& offset)
{
	ASSERT(s_is_initialized);
	auto x = eeprom_read_word((uint16_t*)offset);
	offset += 2;
	return x;
}

int32_t read_int32(offset_type& offset)
{
	ASSERT(s_is_initialized);
	auto x = eeprom_read_dword((uint32_t*)offset);
	offset += 4;
	return *reinterpret_cast<int32_t*>(&x);
}
uint32_t read_uint32(offset_type& offset)
{
	ASSERT(s_is_initialized);
	auto x = eeprom_read_dword((uint32_t*)offset);
	offset += 4;
	return x;
}

float read_float(offset_type& offset)
{
	ASSERT(s_is_initialized);
	auto x = eeprom_read_dword((uint32_t*)offset);
	offset += 4;
	return *reinterpret_cast<float*>(&x);
}

void read(uint8_t* dst, size_type size, offset_type& offset)
{
	ASSERT(s_is_initialized);
	eeprom_read_block(dst, (uint8_t*)offset, size);
	offset += size;
}

void write(int8_t x, offset_type& offset)
{
	ASSERT(s_is_initialized);
	auto y = *reinterpret_cast<uint8_t*>(&x);
	eeprom_update_byte((uint8_t*)offset, y);
	offset += 1;
}
void write(uint8_t x, offset_type& offset)
{
	ASSERT(s_is_initialized);
	auto y = x;
	eeprom_update_byte((uint8_t*)offset, y);
	offset += 1;
}
void write(int16_t x, offset_type& offset)
{
	ASSERT(s_is_initialized);
	auto y = *reinterpret_cast<uint16_t*>(&x);
	eeprom_update_word((uint16_t*)offset, y);
	offset += 2;
}
void write(uint16_t x, offset_type& offset)
{
	ASSERT(s_is_initialized);
	auto y = x;
	eeprom_update_word((uint16_t*)offset, y);
	offset += 2;
}
void write(int32_t x, offset_type& offset)
{
	ASSERT(s_is_initialized);
	auto y = *reinterpret_cast<uint32_t*>(&x);
	eeprom_update_dword((uint32_t*)offset, y);
	offset += 4;
}
void write(uint32_t x, offset_type& offset)
{
	ASSERT(s_is_initialized);
	auto y = x;
	eeprom_update_dword((uint32_t*)offset, y);
	offset += 4;
}
void write(float x, offset_type& offset)
{
	ASSERT(s_is_initialized);
	auto y = *reinterpret_cast<uint32_t*>(&x);
	eeprom_update_dword((uint32_t*)offset, y);
	offset += 4;
}
void write(const uint8_t* src, size_type size, offset_type& offset)
{
	ASSERT(s_is_initialized);
	eeprom_update_block(src, (uint8_t*)offset, size);
	offset += size;
}

}
}

#endif

