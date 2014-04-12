#include "QBaseStdAfx.h"
#include "QBase.h"
#include "Interned_String_DB.h"

using namespace q;

// static int xxx = 0;
// static int yyy = 0;
// static int zzz = 0;

String_DB_Map::String_DB_Map()
	: m_cells(65536)
{
	m_cell_pool.push_back(new Cell_pool);
}

String_DB_Map::Cell& String_DB_Map::find_or_add(uint32_t hash, int length, const char* string)
{
	QASSERT(string);

	//every few allocs do a garbage collect
	if (m_garbage_collect_counter >= 1000)
	{
		m_garbage_collect_counter = 0;
		collect_garbage();
	}

	uint32_t hash0 = hash & 0xFFFF;
	uint32_t hash1 = hash >> 16;
	uint32_t idx = (hash0 ^ hash1) & 0xFFFF;

	Cell* cell = m_cells[idx];
	if (cell)
	{
		//now search for the family
		Cell* prev = nullptr;
		do 
		{
			if (cell->length == length)
			{
				bool csEqual = util::memcmp(cell->string, string, length) == 0; //first test case sensitive - it's faster
				if (csEqual || util::memicmp(cell->string, string, length) == 0) //family found?
				{
					Cell* head = cell;
					if (csEqual) //the head is a match - return it directly
					{
						return *head;
					}

					//search for a case sensitive match
					prev = head;
					cell = head->bext_sibling;
					if (cell)
					{
						do
						{
							if (util::memcmp(cell->string, string, length) == 0) //found sibling
							{
								return *cell;
							}
							prev = cell;
							cell = cell->bext_sibling;
						} while (cell);
					}
					//xxx++;
					//not found - add sibling
					cell = get_new_cell();
					copy_string(*cell, length, string);
					QASSERT(cell->counter == &cell->counter_data);
					cell->hash = head->hash;
					cell->key = head->key;
					cell->counter = &head->counter_data;
					cell->length = length;
					prev->bext_sibling = cell;
					return *cell;
				}
			}
			prev = cell;
			cell = cell->next;
		} while (cell);

		//yyy++;
		//not found - add. collision
		cell = get_new_cell();
		copy_string(*cell, length, string);
		QASSERT(cell->counter == &cell->counter_data);
		cell->hash = hash;
		cell->key = cell->string;
		cell->length = length;
		prev->next = cell;
		return *cell;
	}
	else
	{
		//zzz++;
		//not found - add
		cell = get_new_cell();
		copy_string(*cell, length, string);
		QASSERT(cell->counter == &cell->counter_data);
		cell->hash = hash;
		cell->key = cell->string;
		cell->length = length;
		m_cells[idx] = cell;
		return *cell;
	}
}

String_DB_Map::Cell* String_DB_Map::get_new_cell()
{
	m_garbage_collect_counter++;

	//first get from the free cell pool
	if (!m_free_cells.empty())
	{
		Cell* cell = m_free_cells.back();
		cell->reset();
		m_free_cells.pop_back();
		return cell;
	}

	//no more free cells? allocate from the pools
	Cell_pool* pool = m_cell_pool.back();
	if (pool->used_cell_count >= Cell_pool::pool_size)
	{
		m_cell_pool.push_back(new Cell_pool());
		pool = m_cell_pool.back();
	}

	return &pool->cells[pool->used_cell_count++];
}

void String_DB_Map::copy_string(Cell& cell, int length, char const* string)
{
	QASSERT(string);
	cell.length = length;
	char* dst = cell.string_storage;
	if (length + 1 >= Cell::string_storage_size)
	{
		dst = new char[length + 1];
	}
	memcpy(dst, string, length);
	dst[length] = 0;
	cell.string = dst;
}

void String_DB_Map::free_string_family(Cell& family)
{
	//the head of the family has the counter
	//we go to all the siblings and delete their string and set them to null
	QASSERT(family.counter == &family.counter_data);
	Cell* cell = &family;
	do
	{
		QASSERT(cell->counter == &family.counter_data);
		cell->counter = NULL;
		QASSERT((int)cell->counter_data == 0);
		if (cell->string != cell->string_storage)
		{
			delete[] cell->string;
		}
		cell->key = NULL;
		cell->string = NULL;
		cell = cell->bext_sibling;
	} while (cell);
}

size_t String_DB_Map::collect_garbage()
{
	size_t count = 0;

	//clean up the hash map to avoid further collisions
	for (size_t i = 0; i < m_cells.size(); i++)
	{
		Cell* head = m_cells[i];
		if (!head)
		{
			continue;
		}
		Cell* prev = NULL;

		//handle collisions. Each collision is a string family (equal case insensitive)
		Cell* cell = head;
		do
		{
			QASSERT(cell->counter == &cell->counter_data);
			Cell* next = cell->next;
			if ((int)*cell->counter == 0)
			{
				count++;
				free_string_family(*cell);
				if (cell == head)
				{
					m_cells[i] = next;
					head = next;
				}
				else
				{
					QASSERT(prev);
					prev->next = next;
				}
			}
			else
			{
				prev = cell;
			}
			cell = next;
		} while (cell);
	}

	//collect the empty cells
	m_free_cells.resize(0);
	for (size_t i = 0; i < m_cell_pool.size(); i++)
	{
		Cell_pool* pool = m_cell_pool[i];
		for (size_t j = 0; j < pool->used_cell_count; j++)
		{
			Cell& cell = pool->cells[j];
			//if a cell has a null counter, it was part of a family and it used to point to the counterData of the head
			//since it's null, the head was released previously
			QASSERT(!cell.counter || (size_t)cell.counter != 0);
			if (!cell.counter) //previously freed string family
			{
				QASSERT((size_t)cell.counter_data == 0);
				m_free_cells.push_back(&cell);
			}
		}
	}

	return count;
}


//////////////////////////////////////////////////////////////////////////

String_DB::String_DB()
	: m_keys_count(0)
	, m_instances_count(0)
{
	m_string_map.find_or_add(0, 0, "");
	m_keys_count++;
	m_instances_count++;
}

String_DB::~String_DB()
{
}


String_DB& String_DB::inst()
{
	static String_DB sInstance;
	return sInstance;
}

void String_DB::internalize_string(const char* strStart, const char* strEnd, String_data& retData)
{
	{
		size_t sz = sizeof(String); 
		QASSERT(sz <= 16);
        (void)sz;
	}

	size_t length = strEnd - strStart;
	uint32_t hash = util::murmur_hash_cs(strStart, length, 0xFF4f5);

	{
		std::lock_guard<std::mutex> sm(m_mutex);

		String_DB_Map::Cell& cell = m_string_map.find_or_add(hash, length, strStart);
		QASSERT(cell.counter);
		retData = String_data(&cell);
	}
}

