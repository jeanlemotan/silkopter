#pragma once

namespace board
{
namespace eeprom
{
	extern void init();

	typedef uint16_t size_type;
	typedef uint16_t offset_type;

	extern size_type get_size();
    extern bool is_ready();

	////////////// READING /////////////////////////////

    extern int8_t 	read_int8(offset_type& offset);
    extern uint8_t 	read_uint8(offset_type& offset);
    extern int16_t 	read_int16(offset_type& offset);
    extern uint16_t read_uint16(offset_type& offset);
    extern int32_t 	read_int32(offset_type& offset);
    extern uint32_t read_uint32(offset_type& offset);
    extern float 	read_float(offset_type& offset);
    extern void 	read(uint8_t* dst, size_type size, offset_type& offset);

    ////////////// WRITING /////////////////////////////

    extern void 	write(int8_t x, offset_type& offset);
    extern void 	write(uint8_t x, offset_type& offset);
    extern void 	write(int16_t x, offset_type& offset);
    extern void 	write(uint16_t x, offset_type& offset);
    extern void 	write(int32_t x, offset_type& offset);
    extern void 	write(uint32_t x, offset_type& offset);
    extern void 	write(float x, offset_type& offset);
    extern void 	write(const uint8_t* src, size_type size, offset_type& offset);
}
}

