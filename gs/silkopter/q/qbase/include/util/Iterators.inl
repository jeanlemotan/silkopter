//////////////////////////////////////////////////////////////////////////

template<class T>
Const_Ptr_Iterator<T>::Const_Ptr_Iterator(uint8_t const* data, size_type stride, size_type count)
	: m_begin(data)
	, m_end(data ? data + stride*count : nullptr)
	, m_ptr(data)
	, m_stride(stride)
{
	QASSERT(stride > 0);
}
template<class T>
Const_Ptr_Iterator<T>::Const_Ptr_Iterator(uint8_t const* data, size_type count)
	: m_begin(data)
	, m_end(data ? data + sizeof(T)*count : nullptr)
	, m_ptr(data)
	, m_stride(sizeof(T))
{
}
template<class T>
typename Const_Ptr_Iterator<T>::value_type const& Const_Ptr_Iterator<T>::operator*() const
{
	QASSERT(m_ptr <= m_end);
	return *reinterpret_cast<T const*>(m_ptr);
}
template<class T>
Const_Ptr_Iterator<T>::Const_Ptr_Iterator(Const_Ptr_Iterator<T> const& other)
	: m_begin(other.m_begin)
	, m_end(other.m_end)
	, m_ptr(other.m_ptr)
	, m_stride(other.m_stride)
{
}
template<class T>
Const_Ptr_Iterator<T>& Const_Ptr_Iterator<T>::operator=(Const_Ptr_Iterator<T> const& other)
{
	m_begin = other.m_begin;
	m_end = other.m_end;
	m_ptr = other.m_ptr;
	m_stride = other.m_stride;
	return *this;
}
template<class T>
Const_Ptr_Iterator<T>& Const_Ptr_Iterator<T>::operator++()
{
	QASSERT(m_ptr + m_stride <= m_end);
	m_ptr += m_stride;
	return *this;
}
template<class T>
Const_Ptr_Iterator<T> Const_Ptr_Iterator<T>::operator++(int)
{
	QASSERT(m_ptr + m_stride <= m_end);
	Const_Ptr_Iterator<T> p(*this);
	m_ptr += m_stride;
	return p;
}
template<class T>
Const_Ptr_Iterator<T>& Const_Ptr_Iterator<T>::operator+=(size_type i)
{
	QASSERT(m_ptr + m_stride*i <= m_end);
	m_ptr += m_stride*i;
	return *this;
}
template<class T>
Const_Ptr_Iterator<T> Const_Ptr_Iterator<T>::operator+(size_type i) const
{
	Const_Ptr_Iterator<T> x(*this);
	x += i;
	return x;
}
template<class T>
Const_Ptr_Iterator<T>& Const_Ptr_Iterator<T>::operator--()
{
	QASSERT(m_ptr - m_stride >= m_begin);
	m_ptr -= m_stride;
	return *this;
}
template<class T>
Const_Ptr_Iterator<T> Const_Ptr_Iterator<T>::operator--(int)
{
	QASSERT(m_ptr - m_stride >= m_begin);
	Const_Ptr_Iterator<T> p(*this);
	m_ptr -= m_stride;
	return p;
}
template<class T>
Const_Ptr_Iterator<T>& Const_Ptr_Iterator<T>::operator-=(size_type i)
{
	QASSERT(m_ptr - m_stride*i >= m_begin);
	m_ptr -= m_stride*i;
	return *this;
}
template<class T>
Const_Ptr_Iterator<T> Const_Ptr_Iterator<T>::operator-(size_type i) const
{
	Const_Ptr_Iterator<T> x(*this);
	x -= i;
	return x;
}
template<class T>
typename Const_Ptr_Iterator<T>::difference_type Const_Ptr_Iterator<T>::operator-(Const_Ptr_Iterator const& other) const
{
	return static_cast<difference_type>(m_ptr - other.m_ptr);
}
template<class T>
bool Const_Ptr_Iterator<T>::operator==(Const_Ptr_Iterator<T> const& other) const
{
	return m_ptr == other.m_ptr;
}
template<class T>
bool Const_Ptr_Iterator<T>::operator!=(Const_Ptr_Iterator<T> const& other) const
{
	return !operator==(other);
}
template<class T>
bool Const_Ptr_Iterator<T>::operator<(Const_Ptr_Iterator<T> const& other) const
{
	return m_ptr < other.m_ptr;
}
template<class T>
bool Const_Ptr_Iterator<T>::is_valid() const
{
	return m_ptr != nullptr;
}
template<class T>
T const* Const_Ptr_Iterator<T>::get() const
{
	QASSERT(m_ptr <= m_end);
	return reinterpret_cast<T const*>(m_ptr);
}

//////////////////////////////////////////////////////////////////////////

template<class T>
Ptr_Iterator<T>::Ptr_Iterator(uint8_t* data, size_type stride, size_type count)
	: m_begin(data)
	, m_end(data ? data + stride*count : nullptr)
	, m_ptr(data)
	, m_stride(stride)
{
	QASSERT(stride > 0);
}
template<class T>
Ptr_Iterator<T>::Ptr_Iterator(uint8_t* data, size_type count)
	: m_begin(data)
	, m_end(data ? data + sizeof(T)*count : nullptr)
	, m_ptr(data)
	, m_stride(sizeof(T))
{
}
template<class T>
typename Ptr_Iterator<T>::value_type& Ptr_Iterator<T>::operator*()
{
	QASSERT(m_ptr <= m_end);
	return *reinterpret_cast<T*>(m_ptr);
}
template<class T>
Ptr_Iterator<T>::Ptr_Iterator(Ptr_Iterator const& other)
	: m_begin(other.m_begin)
	, m_end(other.m_end)
	, m_ptr(other.m_ptr)
	, m_stride(other.m_stride)
{
}
template<class T>
Ptr_Iterator<T>& Ptr_Iterator<T>::operator=(Ptr_Iterator<T> const& other)
{
	m_begin = other.m_begin;
	m_end = other.m_end;
	m_ptr = other.m_ptr;
	m_stride = other.m_stride;
	return *this;
}
template<class T>
Ptr_Iterator<T>& Ptr_Iterator<T>::operator++()
{
	QASSERT(m_ptr + m_stride <= m_end);
	m_ptr += m_stride;
	return *this;
}
template<class T>
Ptr_Iterator<T> Ptr_Iterator<T>::operator++(int)
{
	QASSERT(m_ptr + m_stride <= m_end);
	Ptr_Iterator<T> p(*this);
	m_ptr += m_stride;
	return p;
}
template<class T>
Ptr_Iterator<T>& Ptr_Iterator<T>::operator+=(size_type i)
{
	QASSERT(m_ptr + m_stride*i <= m_end);
	m_ptr += m_stride*i;
	return *this;
}
template<class T>
Ptr_Iterator<T> Ptr_Iterator<T>::operator+(size_type i) const
{
	Ptr_Iterator<T> x(*this);
	x += i;
	return x;
}
template<class T>
Ptr_Iterator<T>& Ptr_Iterator<T>::operator--()
{
	QASSERT(m_ptr - m_stride >= m_begin);
	m_ptr -= m_stride;
	return *this;
}
template<class T>
Ptr_Iterator<T> Ptr_Iterator<T>::operator--(int)
{
	QASSERT(m_ptr - m_stride >= m_begin);
	Ptr_Iterator<T> p(*this);
	m_ptr -= m_stride;
	return p;
}
template<class T>
Ptr_Iterator<T>& Ptr_Iterator<T>::operator-=(size_type i)
{
	QASSERT(m_ptr - m_stride*i >= m_begin);
	m_ptr -= m_stride*i;
	return *this;
}
template<class T>
Ptr_Iterator<T> Ptr_Iterator<T>::operator-(size_type i) const
{
	Ptr_Iterator<T> x(*this);
	x -= i;
	return x;
}
template<class T>
typename Ptr_Iterator<T>::difference_type Ptr_Iterator<T>::operator-(Ptr_Iterator const& other) const
{
	return static_cast<difference_type>(m_ptr - other.m_ptr);
}
template<class T>
bool Ptr_Iterator<T>::operator==(Ptr_Iterator<T> const& other) const
{
	return m_ptr == other.m_ptr;
}
template<class T>
bool Ptr_Iterator<T>::operator!=(Ptr_Iterator<T> const& other) const
{
	return !operator==(other);
}
template<class T>
bool Ptr_Iterator<T>::operator<(Ptr_Iterator<T> const& other) const
{
	return m_ptr < other.m_ptr;
}
template<class T>
bool Ptr_Iterator<T>::is_valid() const
{
	return m_ptr != nullptr;
}
template<class T>
T* Ptr_Iterator<T>::get()
{
	QASSERT(m_ptr <= m_end);
	return reinterpret_cast<T*>(m_ptr);
}
