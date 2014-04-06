#pragma once

#include "util/Noncopyable.h"

namespace util
{
	
	template <class T, size_t SIZE>
	class Circular_Buffer : util::Noncopyable
	{
	public:
		static const size_t MAX_SIZE = SIZE;
		
		Circular_Buffer()
			: m_start(0)
			, m_end(0)
		{
				
		}
		size_t size() const
		{
			return (m_start < m_end) ? (m_end - m_start) : ((SIZE - m_start) + m_end);
		}
		
		T const& operator[](size_t idx) const
		{
			ASSERT(idx < size());
			if (m_start < m_end)
			{
				return m_data[m_start + idx];
			}
			else
			{
				auto i = m_start + idx;
				if (i >= SIZE)
				{
					i -= SIZE;
				}
				return m_data[i];
			}
		}
		T& operator[](size_t idx)
		{
			ASSERT(idx < size());
			if (m_start < m_end)
			{
				return m_data[m_start + idx];
			}
			else
			{
				auto i = m_start + idx;
				if (i >= SIZE)
				{
					i -= SIZE;
				}
				return m_data[i];
			}
		}
		
		void push_back(T const& v)
		{
			m_data[m_end++] = v;
			if (m_end >= SIZE)
			{
				m_end = 0;
			}
		}
		T pop_front()
		{
			if (m_end == m_start)	
			{
				return T();
			}
			T ret = m_data[m_start++];
			if (m_start >= SIZE)
			{
				m_start = 0;
			}
			return ret;
		}
		void pop_front(size_t count)
		{
			if (m_end == m_start)
			{
				return;
			}
			m_start += std::min(count, size());
			while (m_start >= SIZE) m_start -= SIZE;
		}
		void clear()
		{
			m_end = m_start = 0;
		}
		
	private:
		size_t m_start;
		size_t m_end;
		T m_data[SIZE];
	};
	
	
}