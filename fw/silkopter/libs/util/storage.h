#pragma once

#include <stdint.h>
#include <stddef.h>

namespace util
{
namespace storage
{
	typedef uint16_t crc_t;
	typedef uint32_t data_t;
	typedef uint32_t id_t;

	//used to indicate item not found
	static const size_t npos = size_t(-1);

	//items is composed of an id and data
	
	void init();

	crc_t get_crc();
	size_t get_count();
	size_t get_max_count();

	//searches an item by id and returns the idx or npos on failure
	size_t find_idx_by_id(id_t id);

	id_t read_id(size_t idx);

	data_t read_data(size_t idx);

	template<class T> T read(size_t idx)
	{
		typedef int _static_assert[(sizeof(T) <= sizeof(data_t)) ? 1 : -1];
		data_t data = read_data(idx);
		auto* __restrict ptr = reinterpret_cast<T*>(&data);
		return *ptr;
	}
	template<class T> T read_by_id(id_t id, T default_)
	{
		typedef int _static_assert[(sizeof(T) <= sizeof(data_t)) ? 1 : -1];
		auto idx = find_idx_by_id(id);
		if (idx != npos)
		{
			data_t data = read_data(idx);
			auto* __restrict ptr = reinterpret_cast<T*>(&data);
			return *ptr;
		}
		return default_;
	}
	
	//You're supposed to call refresh_crc/set_crc after calling this
	void write_data(size_t idx, data_t data);

	template<class T> void write(size_t idx, T data)
	{
		typedef int _static_assert[(sizeof(T) <= sizeof(data_t)) ? 1 : -1];
		data_t _data = 0;
		auto* __restrict ptr = reinterpret_cast<T*>(&_data);
		*ptr = data;
		write_data(idx, _data);
	}
	
	//adds a new item to the storage. This might triger a rehash
	//returns the index of the item or npos on failure (no space left)
	//You're supposed to call refresh_crc/set_crc after calling this
	size_t add_data(id_t id, data_t data);

	template<class T> size_t add(id_t id, T data)
	{
		typedef int _static_assert[(sizeof(T) <= sizeof(data_t)) ? 1 : -1];
		data_t _data = 0;
		auto* __restrict ptr = reinterpret_cast<T*>(&_data);
		*ptr = data;
		return add_data(id, _data);
	}

	//removes an item from the system. This will trigger a rehash
	//You're supposed to call refresh_crc/set_crc after calling this
	void remove(size_t idx);
	void remove_by_id(id_t id);

	//recalculates the crc and returns true if it's different than the old one
	bool refresh_crc();
	bool set_crc(crc_t crc);

	//clears all the items
	void remove_all();
	
}
}
