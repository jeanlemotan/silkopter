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

	id_t read_id(size_t item_idx);
	data_t read_data(size_t item_idx);
	
	//this will recompute the crc - so it's very slow
	void write_data(size_t item_idx, data_t data);
	
	//this will not change the crc
	//You're supposed to call refresh_crc/set_crc after calling this
	void write_data_batch(size_t item_idx, data_t data);

	//recalculates the crc and returns true if it's different than the old one
	bool refresh_crc();
	bool set_crc(crc_t crc);

	//searches an item by id and returns the idx or npos on failure
	size_t find_idx_by_id(id_t id);

	//adds a new item to the storage. This might triger a rehash
	//returns the index of the item or npos on failure (no space left)
	size_t add(id_t id, data_t data);

	//this will not change the crc
	//You're supposed to call refresh_crc/set_crc after calling this
	size_t add_batch(id_t id, data_t data);

	//removes an item from the system. This will trigger a rehash
	void remove(size_t item_idx);

	//this will not change the crc
	//You're supposed to call refresh_crc/set_crc after calling this
	void remove_batch(size_t item_idx);

	//clears all the items
	void remove_all();

}
}
