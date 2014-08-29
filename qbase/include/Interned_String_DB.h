#pragma once

#include "Platform.h"
#include "Interned_String.h"

//disable the 'strdup' warning
#if defined _MSC_VER
#   pragma warning( disable : 4996 )
#endif

#ifndef _SCL_SECURE_NO_WARNINGS
#define _SCL_SECURE_NO_WARNINGS
#endif


namespace q
{

	class String_DB_Map
	{
	public:
		typedef String::Cell Cell;

		String_DB_Map();

		Cell& find_or_add(uint32_t hash, int length, char const* string);

	private:
		void copy_string(Cell& cell, int length, char const* string);
		Cell* get_new_cell();
		size_t collect_garbage();
		void free_string_family(Cell& family);

		std::vector<Cell*> m_cells;

		//the pool here is mainly to allocate cells in chunks therefore reducing the alloc count and increasing memory locality
		struct Cell_pool
		{
			Cell_pool() : used_cell_count(0) {}

			static const int pool_size = 64;
			Cell cells[pool_size];
			size_t used_cell_count;
		};
		std::vector<Cell_pool*> m_cell_pool;

		//free cells are gathered here
		std::vector<Cell*> m_free_cells;

		int m_garbage_collect_counter;
	};


class String_DB
{
protected:
	String_DB();
	~String_DB();

	typedef String::String_Data String_data;

public:
	static String_DB& inst();

	void internalize_string(char const* start, char const* end, String_data& ret_data);

private:
	String_DB_Map	m_string_map;
	size_t		m_keys_count;
	size_t		m_instances_count;
	std::mutex	m_mutex;
};

}

