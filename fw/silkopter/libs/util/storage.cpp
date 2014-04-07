#include "util/storage.h"
#include "board/board.h"

#include <util/crc.h>

namespace util
{
namespace storage
{
	static board::EEPROM::offset_type s_start_offset(0);
	
	struct Main_Header
	{
		Main_Header() : crc(0), count(0), end_offset(sizeof(Main_Header)) {}
		crc_type crc;
		size_t count;
		size_t end_offset;
	};

	struct Header
	{
		Id id;
		size_type size;
		crc_type crc;
	};	
	
	static const size_type MAX_DATA_SIZE = 255;
	
	static Main_Header s_main_header;

	//static const size_t ITEM_SIZE = sizeof(Header);
	
	//////////////////////////////////////////////////////////////////////////
	
	
	static void _write_main_header()
	{
		auto& eeprom = board::get_eeprom();
		eeprom.write(s_main_header, 0);
	}
	
	static crc_type _compute_main_crc()
	{
		Header header;
		uint8_t buffer[MAX_DATA_SIZE];
		crc_type crc = 0;
		auto& eeprom = board::get_eeprom();
		auto offset = s_start_offset;
		for (size_t i = 0; i < s_main_header.count; i++)
		{
			eeprom.read(header, offset);
			auto* __restrict bytes = reinterpret_cast<uint8_t const*>(&header.crc);
			crc = _crc16_update(crc, bytes[0]);
			crc = _crc16_update(crc, bytes[1]);
// 			eeprom.read(buffer, header.size, offset + sizeof(Header));
// 			for (size_type j = 0; j < header.size; j++)
// 			{
// 				crc = _crc16_update(crc, buffer[i]);
// 			}
			offset += sizeof(Header);
			offset += header.size;
			if (offset >= eeprom.get_capacity())
			{
				return 0;
			}
		}
		return crc;
	}

	static crc_type _compute_crc(uint8_t const* buffer, size_type size)
	{
		crc_type crc = 0;
		for (size_type i = 0; i < size; i++)
		{
			crc = _crc16_update(crc, buffer[i]);
		}
		return crc;
	}
	static offset_type _find_offset_by_id(Id id)
	{
		Header header;
		auto& eeprom = board::get_eeprom();
		auto offset = s_start_offset;
		for (size_t i = 0; i < s_main_header.count; i++)
		{
			eeprom.read(header, offset);
			if (header.id.value == id.value)
			{
				return offset;
			}
			offset += sizeof(Header);
			offset += header.size;
		}
		//not found
		return 0;
	}

	//////////////////////////////////////////////////////////////////////////
	// PUBLIC API
	
	void init()
	{
		auto& eeprom = board::get_eeprom();
		
		eeprom.read_advance(s_main_header, s_start_offset);
		PRINT("\nStorage: crc {0}, count {1}", s_main_header.crc, s_main_header.count);

		auto crc = _compute_main_crc();
		if (crc != s_main_header.crc)
		{
			PRINT("\nStorage corrupted. Formatting...");
			remove_all();
		}
 		else
 		{
 			PRINT("\nCRC is ok");
 		}
	}

	crc_type get_crc()
	{
		return s_main_header.crc;
	}
	
	size_t get_record_count()
	{
		return s_main_header.count;
	}
	
	void set_record(Id id, uint8_t const* data, size_type size)
	{
		auto offset = find_record_offset_by_id(id);
		if (offset != 0)
		{
			if (get_record_size(offset) != size)
			{
				//incompatible size - remove and add again
				remove_record(offset);
				set_record(id, data, size);
				return;
			}
			auto crc = _compute_crc(data, size);
			board::get_eeprom().write(crc, offset + sizeof(Id) + sizeof(size_type));
			board::get_eeprom().write(data, size, offset + sizeof(Header));
			return;
		}
		
		offset = s_main_header.end_offset;
		
		Header header;
		header.id = id;
		header.size = size;
		header.crc = _compute_crc(data, size);
		board::get_eeprom().write_advance(header, offset);
		board::get_eeprom().write(data, size, offset);
		s_main_header.count++;
		s_main_header.end_offset = offset + size;
		s_main_header.crc = _compute_main_crc();
		_write_main_header();
	}
	
	void remove_record(Id id)
	{
		auto offset = _find_offset_by_id(id);
		if (offset != 0)
		{
			remove_record(offset);
		}
	}
	void remove_record(offset_type offset)
	{
		if (offset < s_start_offset || offset >= s_main_header.end_offset)
		{
			ASSERT(0);
			return;
		}

		auto& eeprom = board::get_eeprom();
		
		static const size_t BUFFER_SIZE = 256;
		uint8_t buffer[BUFFER_SIZE];
		auto dst_offset = offset;
		auto src_offset = offset + sizeof(Header) + get_record_size(offset);
		auto size = std::min(BUFFER_SIZE, s_main_header.end_offset - src_offset);
		while (size > 0)
		{
			eeprom.read(buffer, size, src_offset);
			eeprom.write(buffer, size, dst_offset);
			src_offset += size;
			dst_offset += size;
			size = std::min(BUFFER_SIZE, s_main_header.end_offset - src_offset);
		}
		s_main_header.count--;
		s_main_header.end_offset = src_offset;
		s_main_header.crc = _compute_main_crc();
		_write_main_header();
	}
		
	size_type get_record(offset_type offset, uint8_t* dst, size_type max_size)
	{
		ASSERT(dst);
		if (offset < s_start_offset || offset >= s_main_header.end_offset)
		{
			ASSERT(0);
			return 0;
		}

		auto& eeprom = board::get_eeprom();
		auto size = std::min(get_record_size(offset), max_size);
		if (size)
		{
			eeprom.read(dst, size, offset + sizeof(Header));
		}
		return size;
	}
	size_type get_record(Id id, uint8_t* dst, size_type max_size)
	{
		ASSERT(dst);
		auto offset = _find_offset_by_id(id);
		if (offset != 0)
		{
			return get_record(offset, dst, max_size);
		}
		return 0;
	}

	crc_type get_record_crc(offset_type offset)
	{
		if (offset < s_start_offset || offset >= s_main_header.end_offset)
		{
			ASSERT(0);
			return 0;
		}

		auto& eeprom = board::get_eeprom();
		crc_type crc;
		eeprom.read(crc, offset + sizeof(Id) + sizeof(size_type));
		return crc;
	}
	size_type get_record_size(offset_type offset)
	{
		if (offset < s_start_offset || offset >= s_main_header.end_offset)
		{
			ASSERT(0);
			return 0;
		}

		auto& eeprom = board::get_eeprom();
		size_type size;
		eeprom.read(size, offset + sizeof(Id));
		return size;
	}
		
	offset_type find_record_offset_by_id(Id id)
	{
		return _find_offset_by_id(id);
	}


	void remove_all()
	{
		s_main_header.count = 0;
		s_main_header.crc = 0;
		s_main_header.end_offset = sizeof(Main_Header);
		_write_main_header();
	}
	
}
}
