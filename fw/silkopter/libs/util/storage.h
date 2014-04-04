#pragma once

#include <stdint.h>
#include <stddef.h>

namespace util
{
namespace storage
{
	typedef uint16_t crc_type;
	typedef uint8_t size_type;
	typedef size_t offset_type;
	
	struct Id
	{
		Id() = default;
		explicit Id(uint32_t value) : value(value) {}
		uint32_t value;	
	};

	void init();

	crc_type get_crc();

	size_t get_record_count();
	
	void set_record(Id id, uint8_t const* data, size_type size);

	template<class T> void set_record(Id id, T data)
	{
		typedef int _static_assert[(sizeof(T) < 256) ? 1 : -1];
		set_record(id, reinterpret_cast<uint8_t*>(&data), sizeof(T));
	}

	void remove_record(Id id);
	void remove_record(offset_type offset);
	
	size_type get_record(offset_type offset, uint8_t* dst, size_type max_size);
	size_type get_record(Id id, uint8_t* dst, size_type max_size);

	crc_type get_record_crc(offset_type offset);
	size_type get_record_size(offset_type offset);
	
	//searches an item by id and returns the idx or 0 on failure
	offset_type find_record_offset_by_id(Id id);

	template<class T> T get_record(offset_type offset, T default_)
	{
		typedef int _static_assert[(sizeof(T) < 256) ? 1 : -1];
		T t;
		auto sz = get_record(offset, reinterpret_cast<uint8_t*>(&t), sizeof(T));
		if (sz != sizeof(T))
		{
			return default_;
		}
		return t;
	}
	template<class T> T get_record(Id id, T default_)
	{
		typedef int _static_assert[(sizeof(T) < 256) ? 1 : -1];
		T t;
		auto sz = get_record(id, reinterpret_cast<uint8_t*>(&t), sizeof(T));
		if (sz != sizeof(T))
		{
			return default_;
		}
		return t;
	}
	
	//clears all the items
	void remove_all();
	
}
}
