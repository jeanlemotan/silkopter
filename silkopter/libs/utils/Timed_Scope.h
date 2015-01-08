#pragma once

namespace util
{
	class Timed_Scope
	{
	public:
        Timed_Scope(char const* file, int line)
            : m_file(file)
            , m_line(line)
            , m_start(q::Clock::now())
        {
        }

        ~Timed_Scope()
        {
            QLOGI("Scope {}:{} took {}", m_file, m_line, q::Clock::now() - m_start);
        }

	private:
        char const* m_file = nullptr;
        int m_line = 0;
        q::Clock::time_point m_start;
	};
}


#define TIMED_SCOPE() util::Timed_Scope __ts__(__FILE__, __LINE__);
#define TIMED_FUNCTION() util::Timed_Scope __ts__(__FUNCTION__, 0);
