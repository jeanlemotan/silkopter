#pragma once

//#include <cstddef>
//#include <cstdint>
#include <util/Noncopyable.h>

namespace q
{
namespace mem
{


template <std::size_t N>
class Arena : util::Noncopyable
{
	static std::size_t const alignment = 16;
	char m_buffer[N + alignment];
	char* m_start_ptr;
	char* m_ptr;

	std::size_t align_up(std::size_t n)
	{
		return n + (alignment-1) & ~(alignment-1);
	}

	std::uintptr_t ptr_align_up(std::uintptr_t n)
	{
		return n + (alignment-1) & ~(alignment-1);
	}

	bool is_ptr_in_buffer(char* p)
	{
		return m_start_ptr <= p && p <= m_start_ptr + N;
	}

public:
	Arena() : m_start_ptr((char*)ptr_align_up((std::uintptr_t)m_buffer)), m_ptr(m_start_ptr) {}
	~Arena() 
	{
		m_ptr = nullptr;
	}

public:
	char* allocate(std::size_t n)
	{
		QASSERT(is_ptr_in_buffer(m_ptr) && "mem::EmbeddedAllocator has outlived arena");
		n = align_up(n);
		if (m_start_ptr + N >= n + m_ptr)
		{
			char* r = m_ptr;
			m_ptr += n;
			return r;
		}
		return static_cast<char*>(::operator new(n));
	}
	void deallocate(char* p, std::size_t n)
	{
		QASSERT(is_ptr_in_buffer(m_ptr) && "mem::EmbeddedAllocator has outlived arena");
		if (is_ptr_in_buffer(p))
		{
			n = align_up(n);
			if (p + n == m_ptr)
			{
				m_ptr = p;
			}
		}
		else
		{
			::operator delete(p);
		}
	}

	static std::size_t get_size() 
	{
		return N;
	}
	std::size_t get_used() const 
	{
		return static_cast<std::size_t>(m_ptr - m_start_ptr);
	}
	void reset() 
	{
		m_ptr = m_start_ptr;
	}
};

template <class T, std::size_t N>
class Embedded_Allocator
{
	Arena<N>* m_arena;
public:
	typedef T value_type;
	typedef Arena<N> arena_type;
	typedef std::false_type propagate_on_container_move_assignment;
	typedef std::false_type propagate_on_container_swap;
	typedef std::false_type propagate_on_container_copy_assignment;

public:
	template <class _Up> struct rebind {typedef Embedded_Allocator<_Up, N> other;};

	Embedded_Allocator(Arena<N>& a) : m_arena(&a) {}
	template <class U> Embedded_Allocator(Embedded_Allocator<U, N>const & a) : m_arena(a.m_arena) {}

	Embedded_Allocator& operator=(Embedded_Allocator const & other)
	{ 
		m_arena = other.m_arena;
		return *this; 
	}
	
public:
	T* allocate(std::size_t n)
	{
		return reinterpret_cast<T*>(m_arena->allocate(n*sizeof(T)));
	}
	void deallocate(T* p, std::size_t n)
	{
		m_arena->deallocate(reinterpret_cast<char*>(p), n*sizeof(T));
	}
	void construct(T* ptr)
	{	
		// default construct object at ptr
		::new ((void* )ptr) T();
	}

	void construct(T* ptr, const T& val)
	{	
		// construct object at ptr with value val
		::new ((void* )ptr) T(val);
	}

	template<class UT>
	void destroy(UT* ptr)
	{	
		(void)ptr;
		// destroy object at ptr
		ptr->~UT();
	}

	template <class T1, std::size_t N1, class U, std::size_t M>
	friend bool operator==(Embedded_Allocator<T1, N1> const & x, Embedded_Allocator<U, M> const & y);

	template <class U, std::size_t M> friend class Embedded_Allocator;
};

template <class T, std::size_t N, class U, std::size_t M>
inline bool operator==(Embedded_Allocator<T, N> const & x, Embedded_Allocator<U, M> const & y)
{
	return N == M && x.m_arena == y.m_arena;
}

template <class T, std::size_t N, class U, std::size_t M>
inline bool operator!=(Embedded_Allocator<T, N> const & x, Embedded_Allocator<U, M> const & y)
{
	return !(x == y);
}


} //mem
} //q