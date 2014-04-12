#pragma once

namespace q
{
namespace util
{

template<class T>
class Const_Ptr_Iterator
{
public:
	typedef std::random_access_iterator_tag iterator_category;
	typedef T value_type;
	typedef size_t size_type;
	typedef ptrdiff_t difference_type;
	typedef const T* pointer;
	typedef const T& reference;

	value_type const& operator*() const;
	Const_Ptr_Iterator(uint8_t const* data, size_type stride, size_type count);
	Const_Ptr_Iterator(uint8_t const* data, size_type count);
	Const_Ptr_Iterator(Const_Ptr_Iterator const& other);
	auto operator=(Const_Ptr_Iterator const& other) -> Const_Ptr_Iterator&;
	auto operator++() -> Const_Ptr_Iterator&;
	auto operator++(int) -> Const_Ptr_Iterator;
	auto operator+=(size_type i) -> Const_Ptr_Iterator&;
	auto operator+(size_type i) const -> Const_Ptr_Iterator;
	auto operator--() -> Const_Ptr_Iterator&;
	auto operator--(int) -> Const_Ptr_Iterator;
	auto operator-=(size_type i) -> Const_Ptr_Iterator&;
	auto operator-(size_type i) const -> Const_Ptr_Iterator;
	auto operator-(Const_Ptr_Iterator const& other) const -> difference_type;
	auto operator==(Const_Ptr_Iterator const& other) const -> bool;
	auto operator!=(Const_Ptr_Iterator const& other) const -> bool;
	auto operator<(Const_Ptr_Iterator const& other) const -> bool;
	auto is_valid() const -> bool;
	auto get() const -> value_type const*; //read-only!!!!!
private:
	uint8_t const* m_begin;
	uint8_t const* m_end;
	uint8_t const* m_ptr;
	size_type m_stride;
};

template<class T>
class Ptr_Iterator
{
public:
	typedef std::random_access_iterator_tag iterator_category;
	typedef T value_type;
	typedef size_t size_type;
	typedef ptrdiff_t difference_type;
	typedef T* pointer;
	typedef T& reference;

	value_type& operator*();
	Ptr_Iterator(uint8_t* data, size_type stride, size_type count);
	Ptr_Iterator(uint8_t* data, size_type count);
	Ptr_Iterator(Ptr_Iterator const& other);
	auto operator=(Ptr_Iterator const& other) -> Ptr_Iterator&;
	auto operator++() -> Ptr_Iterator&; //prefix
	auto operator++(int) -> Ptr_Iterator; //postfix
	auto operator+=(size_type i) -> Ptr_Iterator&;
	auto operator+(size_type i) const -> Ptr_Iterator;
	auto operator--() -> Ptr_Iterator&; //prefix
	auto operator--(int) -> Ptr_Iterator; //postfix
	auto operator-=(size_type i) -> Ptr_Iterator&;
	auto operator-(size_type i) const -> Ptr_Iterator;
	auto operator-(Ptr_Iterator const& other) const -> difference_type;
	auto operator==(Ptr_Iterator const& other) const -> bool;
	auto operator!=(Ptr_Iterator const& other) const -> bool;
	auto operator<(Ptr_Iterator const& other) const -> bool;
	auto is_valid() const -> bool;
	auto get() -> value_type*; //write-only!!!!!
private:
	uint8_t* m_begin;
	uint8_t const* m_end;
	uint8_t* m_ptr;
	size_type m_stride;
};

#include "Iterators.inl"

}
}
