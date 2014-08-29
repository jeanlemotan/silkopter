#include "util/storage.h"
#include "board/board.h"

#include "qbase.h"

namespace util
{
namespace storage
{
	static board::EEPROM::offset_type s_start_offset(0);
	
	struct Main_Header
	{
		crc_type crc = 0;
		size_t count = 0;
		size_t end_offset = sizeof(Main_Header);
	};

	struct Header
	{
		Id id = Id(0);
		size_type size = 0;
		crc_type crc = 0;
	};	
	
	static const size_type MAX_DATA_SIZE = 255;
	
	static Main_Header s_main_header;
	static bool s_is_initialized = false;

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
//		uint8_t buffer[MAX_DATA_SIZE];
		crc_type crc = 0;
		auto& eeprom = board::get_eeprom();
		auto offset = s_start_offset;
		for (size_t i = 0; i < s_main_header.count; i++)
		{
			eeprom.read(header, offset);
			q::util::update_crc(crc, header.crc);
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

	static Offset _find_offset_by_id(Id id)
	{
		Header header;
		auto& eeprom = board::get_eeprom();
		auto offset = s_start_offset;
		for (size_t i = 0; i < s_main_header.count; i++)
		{
			eeprom.read(header, offset);
			if (header.id.value == id.value)
			{
				return Offset(offset);
			}
			offset += sizeof(Header);
			offset += header.size;
		}
		//not found
		return Offset(0);
	}

	//////////////////////////////////////////////////////////////////////////
	// PUBLIC API
	
	void init()
	{
		if (s_is_initialized)
		{
			return;
		}
		s_is_initialized = true;
		
		auto& eeprom = board::get_eeprom();
		
		eeprom.read_advance(s_main_header, s_start_offset);
		q::logf(F_STR("\nStorage: crc {}, count {}"), s_main_header.crc, s_main_header.count);

		auto crc = _compute_main_crc();
		if (crc != s_main_header.crc)
		{
			q::logf(F_STR("\nStorage corrupted. Formatting..."));
			remove_all();
		}
 		else
 		{
 			q::logf(F_STR("\nCRC is ok"));
 		}

		//make sure we're never overriting the main header
		s_main_header.end_offset = std::max(s_main_header.end_offset, sizeof(Main_Header));
	}

	crc_type get_crc()
	{
		QASSERT(s_is_initialized);
		return s_main_header.crc;
	}
	
	size_t get_record_count()
	{
		QASSERT(s_is_initialized);
		return s_main_header.count;
	}
	
	void set_record(Id id, uint8_t const* data, size_type size)
	{
		QASSERT(s_is_initialized);
		auto& eeprom = board::get_eeprom();
		auto offset = find_record_offset_by_id(id).value;
		if (offset != 0)
		{
			if (get_record_size(Offset(offset)) != size)
			{
				//incompatible size - remove and add again
				remove_record(Offset(offset));
				set_record(id, data, size);
				return;
			}
			auto crc = q::util::compute_crc16(data, size);
			eeprom.write(crc, offset + sizeof(Id) + sizeof(size_type));
			eeprom.write(data, size, offset + sizeof(Header));
		}
		else
		{
			offset = s_main_header.end_offset;

			Header header;
			header.id = id;
			header.size = size;
			header.crc = q::util::compute_crc16(data, size);
			eeprom.write_advance(header, offset);
			eeprom.write(data, size, offset);
			s_main_header.count++;
			s_main_header.end_offset = offset + size;
		}

		s_main_header.crc = _compute_main_crc();
		_write_main_header();
	}
	
	void remove_record(Id id)
	{
		QASSERT(s_is_initialized);
		auto offset = _find_offset_by_id(id);
		if (offset.value != 0)
		{
			remove_record(offset);
		}
	}
	void remove_record(Offset offset)
	{
		QASSERT(s_is_initialized);
		if (offset.value < s_start_offset || offset.value >= s_main_header.end_offset)
		{
			QASSERT(0);
			return;
		}

		auto& eeprom = board::get_eeprom();
		
		static const size_t BUFFER_SIZE = 16;
		uint8_t buffer[BUFFER_SIZE];
		auto dst_offset = offset.value;
		auto src_offset = offset.value + sizeof(Header) + get_record_size(offset);
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
		
	size_type get_record(Offset offset, uint8_t* dst, size_type max_size)
	{
		QASSERT(s_is_initialized);
		QASSERT(dst);
		if (offset.value < s_start_offset || offset.value >= s_main_header.end_offset)
		{
			QASSERT(0);
			return 0;
		}

		auto& eeprom = board::get_eeprom();
		auto size = std::min(get_record_size(offset), max_size);
		if (size)
		{
			eeprom.read(dst, size, offset.value + sizeof(Header));
		}
		return size;
	}
	size_type get_record(Id id, uint8_t* dst, size_type max_size)
	{
		QASSERT(s_is_initialized);
		QASSERT(dst);
		auto offset = _find_offset_by_id(id);
		if (offset.value != 0)
		{
			return get_record(offset, dst, max_size);
		}
		return 0;
	}

	crc_type get_record_crc(Offset offset)
	{
		QASSERT(s_is_initialized);
		if (offset.value < s_start_offset || offset.value >= s_main_header.end_offset)
		{
			QASSERT(0);
			return 0;
		}

		auto& eeprom = board::get_eeprom();
		crc_type crc;
		eeprom.read(crc, offset.value + sizeof(Id) + sizeof(size_type));
		return crc;
	}
	size_type get_record_size(Offset offset)
	{
		QASSERT(s_is_initialized);
		if (offset.value < s_start_offset || offset.value >= s_main_header.end_offset)
		{
			QASSERT(0);
			return 0;
		}

		auto& eeprom = board::get_eeprom();
		size_type size;
		eeprom.read(size, offset.value + sizeof(Id));
		return size;
	}
		
	Offset find_record_offset_by_id(Id id)
	{
		QASSERT(s_is_initialized);
		return _find_offset_by_id(id);
	}


	void remove_all()
	{
		QASSERT(s_is_initialized);
		s_main_header.count = 0;
		s_main_header.crc = 0;
		s_main_header.end_offset = sizeof(Main_Header);
		_write_main_header();
	}
	
}
}
