#pragma once

#include "chrono.h"

namespace util
{
	class Timed_Scope
	{
	public:
		Timed_Scope(char const* file, int line);
		~Timed_Scope();
	private:
		char const* m_file;
		int m_line;
		chrono::time_us m_start;
	};
}


#define TIMED_BLOCK() debug::detail::Timed_Scope __ts__(__FILE__, __LINE__);