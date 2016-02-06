#include "Config.h"

#if defined(CPU_TYPE) && CPU_TYPE == CPU_ATMEGA_2560

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/eeprom.h>
#include "board/boards/AVR_EEPROM.h"

namespace board
{

bool AVR_EEPROM::is_ready() const
{
	return eeprom_is_ready();
}

EEPROM::size_type AVR_EEPROM::get_capacity() const
{
	return 4096;
}

void AVR_EEPROM::read(uint8_t* dst, size_type size, offset_type offset) const
{
	eeprom_read_block(dst, (uint8_t*)offset, size);
}

void AVR_EEPROM::write(uint8_t const* src, size_type size, offset_type offset)
{
	eeprom_update_block(src, (uint8_t*)offset, size);
	
// 	uint8_t buf[128];
// 	eeprom_read_block(buf, (uint8_t*)offset, size);
// 	buf[0] = buf[0];
}

}

#endif

