#include "util/storage.h"
#include "board/board.h"

#include <util/crc16.h>

namespace util
{
namespace storage
{
	static crc_t s_crc;
	static size_t s_count;
	static size_t s_max_count;
	static board::EEPROM::offset_type s_start_item_offset(0);
	
	static const size_t ITEM_SIZE = sizeof(id_t) + sizeof(data_t);
	
	//////////////////////////////////////////////////////////////////////////
	
	
	static void _set_count(size_t count)
	{
		s_count = count;
		auto& eeprom = board::get_eeprom();
		board::EEPROM::offset_type offset = 0;
		offset += sizeof(crc_t);
		eeprom.write(count, offset);
	}
	
	static crc_t _compute_crc()
	{
		crc_t crc = 0;
		auto& eeprom = board::get_eeprom();
		auto offset = s_start_item_offset + sizeof(id_t);
		for (size_t i = 0; i < s_count; i++)
		{
			data_t data;
			eeprom.read(data, offset);
			offset += ITEM_SIZE;
			auto const* __restrict bytes = reinterpret_cast<uint8_t const*>(&data);
			crc = _crc16_update(crc, bytes[0]);
			crc = _crc16_update(crc, bytes[1]);
			crc = _crc16_update(crc, bytes[2]);
			crc = _crc16_update(crc, bytes[3]);
		}
		return crc;
	}
	
	static void _insert(size_t idx)
	{
		ASSERT(idx <= s_count);
		//fast case - insert at the end
		if (idx >= s_count)
		{
			_set_count(s_count + 1);
			return;
		}
		
		auto& eeprom = board::get_eeprom();
		
		board::EEPROM::offset_type start_offset = s_start_item_offset + idx * ITEM_SIZE;
		board::EEPROM::offset_type end_offset = s_start_item_offset + s_count * ITEM_SIZE;
		
		const size_t buffer_size = 128;
		uint8_t buffer[buffer_size];
		
		while (end_offset > start_offset)
		{
			size_t sz = math::min(buffer_size, end_offset - start_offset);
			end_offset -= sz;
			eeprom.read(buffer, sz, end_offset);
			eeprom.write(buffer, sz, end_offset + ITEM_SIZE);
		}
		_set_count(s_count + 1);
	}
	
	static void _remove(size_t idx)
	{
		ASSERT(idx <= s_count && s_count > 0);
		if (s_count == 0)
		{
			return;
		}
		//fast case - insert at the end
		if (idx >= s_count)
		{
			_set_count(s_count - 1);
			return;
		}
		
		auto& eeprom = board::get_eeprom();
		
		board::EEPROM::offset_type start_offset = s_start_item_offset + idx * ITEM_SIZE;
		board::EEPROM::offset_type end_offset = s_start_item_offset + s_count * ITEM_SIZE;
		
		static const size_t buffer_size = 128;
		uint8_t buffer[buffer_size];
		
		while (end_offset > start_offset)
		{
			size_t sz = math::min(buffer_size, end_offset - start_offset);
			eeprom.read(buffer, sz, start_offset);
			eeprom.write(buffer, sz, start_offset - ITEM_SIZE);
			start_offset += sz;
		}
		_set_count(s_count + 1);
	}
	
	static bool _find_idx_by_id(id_t id, size_t& last_idx, id_t& last_id)
	{
		//fast case - empty
		if (s_count == 0)
		{
			last_idx = npos;
			return false;
		}
		int16_t start = 0;
		int16_t end = s_count - 1;
		//fast cases - before start and at end
		last_idx = start;
		last_id = read_id(last_idx);
		if (id == last_id)
		{
			return true;
		}
		//only one element or before the first?
		if (start == end || id < last_id)
		{
			return false;
		}
			
		//at the end?
		last_idx = end;
		last_id = read_id(last_idx);
		if (id == last_id)
		{
			return true;
		}
		//after the end
		if (id > last_id)
		{
			return false;
		}
			
		//binary search the correct place
		while (end - start >= 2)
		{
			last_idx = (end + start) >> 1;
			last_id = read_id(last_idx);
			if (id < last_id)
			{
				end = last_idx;
			}
			else if (id > last_id)
			{
				start = last_idx;
			}
			else
			{
				return true;
			}
		}
			
		//not found
		return false;
	}

	static void _write_id(size_t item_idx, id_t id)
	{
		ASSERT(item_idx < s_max_count);
		board::get_eeprom().write(id, s_start_item_offset + item_idx * ITEM_SIZE);
	}
	
	static bool _check_integrity()
	{
		if (s_count == 0)
		{
			return true;
		}
		
		auto& eeprom = board::get_eeprom();
		auto offset = s_start_item_offset;
		id_t last_id = 0;
		eeprom.read(last_id, offset);
		offset += ITEM_SIZE;
		for (size_t i = 1; i < s_count; i++)
		{
			id_t id;
			eeprom.read(id, offset);
			offset += ITEM_SIZE;
			if (id < last_id)
			{
				return false;
			}
		}
		return true;
	}


	//////////////////////////////////////////////////////////////////////////
	// PUBLIC API
	
	void init()
	{
		auto& eeprom = board::get_eeprom();
		s_max_count = eeprom.get_capacity() / ITEM_SIZE;
		
		eeprom.read_advance(s_crc, s_start_item_offset);
		eeprom.read_advance(s_count, s_start_item_offset);
		PRINT("\nStorage: crc {0}, count {1} / {2}", s_crc, s_count, s_max_count);
// 		auto x = s_count;
// 		auto y = s_max_count;
		if (s_count > s_max_count)
		{
			PRINT("\nStorage corrupted. Formatting...");
			remove_all();
		}
		auto crc = _compute_crc();
		if (crc != s_crc)
		{
			PRINT("\nCrc doesn't match. Checking integrity...");
			if (!_check_integrity())
			{
				PRINT("\nStorage corrupted. Formatting...");
				remove_all();
			}
		}
	}

	crc_t get_crc()
	{
		return s_crc;
	}
	size_t get_count()
	{
		return s_count;
	}
	size_t get_max_count()
	{
		return s_max_count;
	}

	id_t read_id(size_t item_idx)
	{
		if (item_idx >= s_max_count)
		{
			return 0;
		}
		id_t id;
		board::get_eeprom().read(id, s_start_item_offset + item_idx * ITEM_SIZE);
		return id;
	}
	data_t read_data(size_t item_idx)
	{
		ASSERT(item_idx < s_max_count);
		data_t data;
		board::get_eeprom().read(data, s_start_item_offset + item_idx * ITEM_SIZE + sizeof(id_t));
		return data;
	}
	
	//this will recompute the crc - so it's very slow
	void write_data(size_t item_idx, data_t data)
	{
		ASSERT(item_idx < s_max_count);
		board::get_eeprom().write(data, s_start_item_offset + item_idx * ITEM_SIZE + sizeof(id_t));
	}

	//recalculates the crc and returns true if it's different than the old one
	bool refresh_crc()
	{
		auto crc = _compute_crc();
		return set_crc(crc);
	}
	bool set_crc(crc_t crc)
	{
		if (crc != s_crc)
		{
			s_crc = crc;
			board::get_eeprom().write(crc, 0);
			return true;
		}
		return false;
	}

	size_t add_data(id_t id, data_t data)
	{
		//fast case - empty
		if (s_count == 0)
		{
			_insert(0);	
			_write_id(0, id);
			write_data(0, data);
			return 0;
		}
		
		id_t last_id = 0;
		size_t last_idx = npos;
		bool found = _find_idx_by_id(id, last_idx, last_id);
		
		if (found)
		{
			write_data(last_idx, data);
			return last_idx;
		}
		
		if (id < last_id)
		{
			_insert(last_idx);
			_write_id(last_idx, id);
			write_data(last_idx, data);
			return last_idx;
		}
		if (id > last_id)
		{
			_insert(last_idx + 1);
			_write_id(last_idx + 1, id);
			write_data(last_idx + 1, data);
			return last_idx + 1;
		}
		ASSERT(0);
		return npos;
	}

	void remove(size_t item_idx)
	{
		_remove(item_idx);
	}
	void remove_by_id(id_t id)
	{
		auto idx = find_idx_by_id(id);
		if (idx != npos)
		{
			_remove(idx);
		}
	}

	//clears all the items
	void remove_all()
	{
		s_count = 0;
		s_crc = 0;
		board::EEPROM::offset_type offset(0);
		board::get_eeprom().write_advance(s_crc, offset);
		board::get_eeprom().write(s_count, offset);
	}
	
	size_t find_idx_by_id(id_t id)
	{
		id_t last_id = 0;
		size_t last_idx = npos;
		bool found = _find_idx_by_id(id, last_idx, last_id);
		return found ? last_idx : npos;
	}
}
}
