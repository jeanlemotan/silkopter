#pragma once

#include "QBase.h"

namespace q
{
namespace util
{
	
	template <class T, size_t SIZE>
	class Circular_Buffer : util::Noncopyable
	{
	public:
		static const size_t MAX_SIZE = SIZE;
        static const size_t MASK = SIZE - 1;

        Circular_Buffer()
            : m_start(0)
            , m_size(0)

        {
            QSTATIC_ASSERT(((SIZE - 1) & SIZE) == 0);
        }
		
		size_t size() const
		{
            return m_size;
		}
		bool empty() const
		{
            return m_size == 0;
		}

        size_t get_space_left() const
        {
            return SIZE - m_size;
        }
        size_t capacity() const
        {
            return SIZE;
        }

        void resize(size_t size)
        {
            QASSERT(size < capacity());
            m_size = size;
        }

		T const& operator[](size_t idx) const
		{
			QASSERT(idx < size());
            idx = (m_start + idx) & MASK;
            return m_data[idx];
		}
		T& operator[](size_t idx)
		{
            QASSERT(idx < size());
            idx = (m_start + idx) & MASK;
            return m_data[idx];
        }
		
		void push_back(T const& v)
		{
            QASSERT(size() < capacity());
            auto idx = (m_start + m_size) & MASK;
            m_data[idx] = v;
            m_size++;
		}
		T pop_front()
		{
            QASSERT(!empty());
            T ret = m_data[m_start];
            m_start = (m_start + 1) & MASK;
            m_size--;
			return ret;
		}
		void pop_front(size_t count)
		{
            QASSERT(count <= size());
            m_start = (m_start + count) & MASK;
            m_size -= count;
        }
		void clear()
		{
            m_start = 0;
            m_size = 0;
		}

	private:
        size_t m_start;
        size_t m_size;
		T m_data[SIZE];
	};
	
	
}
}
