namespace math
{

template<typename T> mat2<T> const mat2<T>::zero(0);																		   \
template<typename T> mat2<T> const mat2<T>::one(1);																		   \
template<typename T> mat2<T> const mat2<T>::identity;																		   \

///////////////////////////////////////////////////////////////////////////////
//constructors
///////////////////////////////////////////////////////////////////////////////

template <typename T>
inline mat2<T>::mat2()
: column0(math::uninitialized)
, column1(math::uninitialized)
{
	set_identity();
}

template <typename T>
inline mat2<T>::mat2(math::ZUninitialized)
: column0(math::uninitialized)
, column1(math::uninitialized)
{
}

template <typename T>
inline mat2<T>::mat2(angle<T> const& rotation)
: column0(math::uninitialized)
, column1(math::uninitialized)
{
	T s, c;
    sin_cos(rotation, s, c);

	m[0] = c;
	m[1] = s;
	m[2] = -s;
	m[3] = c;
}

template <typename T>
inline mat2<T>::mat2(T value)
: column0(math::uninitialized)
, column1(math::uninitialized)
{
	m[0] = m[1] = value;
	m[2] = m[3] = value;
}

template <typename T>
inline mat2<T>::mat2(mat2<T> const& other)
: column0(math::uninitialized)
, column1(math::uninitialized)
{
	*this = other;
}

template<typename T>
template<typename U>
inline mat2<T>::mat2(mat2<U> const& other)
: column0(math::uninitialized)
, column1(math::uninitialized)
{
	m[0] = (T)other.m[0];
	m[1] = (T)other.m[1];
	m[2] = (T)other.m[2];
	m[3] = (T)other.m[3];
}

template<typename T>
inline mat2<T>::mat2(T const v0, T const v1, T const v2, T const v3)
: column0(math::uninitialized)
, column1(math::uninitialized)
{
	m[0] = v0;
	m[1] = v1;
	m[2] = v2;
	m[3] = v3;
}

template<typename T>
inline mat2<T>::mat2(vec2<T> const& column0, vec2<T> const& column1)
: column0(column0)
, column1(column1)
{
}

///////////////////////////////////////////////////////////////////////////////
//methods
///////////////////////////////////////////////////////////////////////////////

template<typename T>
inline void mat2<T>::set(T const values[4])
{
    QASSERT(values);
	memcpy(m, values, sizeof(T)*4);
}

template <typename T>
inline void mat2<T>::set_identity()
{
	m[0] = m[3] = (T)1;
	m[1] = m[2] = (T)0;
}

template <typename T>
template <class Policy>
inline bool mat2<T>::invert()
{
	T sav = m[0];
	T div = (m[0]*m[3] - m[1]*m[2]);
	if (is_zero(div))
	{
		return false;
	}

	div = inverse<T, Policy>(div);
	m[0] = div*m[3];
	m[1] = -div*m[1];
	m[2] = -div*m[2];
	m[3] = div*sav;

	return true;
}

template <typename T>
inline void mat2<T>::transpose()
{
	std::swap(m[1], m[2]);
}

template <typename T>
inline vec2<T> mat2<T>::get_row(uint8_t row) const
{
    QASSERT(row < 2);
	return vec2<T>(m[row], m[row + 2]);
}
template <typename T>
inline void mat2<T>::set_row(uint8_t row, vec2<T> const& v)
{
    QASSERT(row < 2);
	m[row    ] = v.x;
	m[row + 2] = v.y;
}

template <typename T>
inline vec2<T> const& mat2<T>::get_column(uint8_t column) const
{
    QASSERT(column < 2);
	uint8_t idx = column * 2;
	return *(vec2<T> const*)(&m[idx]);
}
template <typename T>
inline void mat2<T>::set_column(uint8_t column, vec2<T> const& v)
{
    QASSERT(column < 2);
	uint8_t idx = column * 2;
	m[idx    ] = v.x;
	m[idx + 1] = v.y;
}

template <typename T>
inline vec2<T> const& mat2<T>::get_axis_x() const
{
	return get_column(0);
}

template <typename T>
inline vec2<T> const& mat2<T>::get_axis_y() const
{
	return get_column(1);
}

template <typename T>
inline vec2<T> mat2<T>::get_scale() const
{
	return vec2<T>(length(get_axis_x()), length(get_axis_y()));
}

template <typename T>
inline void mat2<T>::set_axis_x(vec2<T> const& axis)
{
	m[0] = axis.x;
	m[1] = axis.y;
}

template <typename T>
inline void mat2<T>::set_axis_y(vec2<T> const& axis)
{
	m[2] = axis.x;
	m[3] = axis.y;
}

template <typename T>
inline void mat2<T>::set_scale(vec2<T> const& s)
{
	m[ 0] = s.x;
	m[ 3] = s.y;
}

//////////////////////////////////////////////////////////////////////////

template <typename T>
inline bool mat2<T>::operator==(mat2<T> const& m) const
{
	return memcmp(this->m, m.m, sizeof(T) * 4) == 0;
}
template <typename T>
inline bool mat2<T>::operator!=(mat2<T> const& m) const
{
	return !operator==(m);
}

///////////////////////////////////////////////////////////////////////////////
// indexing operators
///////////////////////////////////////////////////////////////////////////////

template <typename T>
inline T* mat2<T>::data()
{
	return m;
}
template <typename T>
inline T const* mat2<T>::data() const
{
	return m;
}

template <typename T>
inline T& mat2<T>::operator()(uint8_t column, uint8_t row)
{
    QASSERT(column < column_count && row < row_count);
	return m[column*row_count + row];
}

template <typename T>
inline T const& mat2<T>::operator()(uint8_t column, uint8_t row) const
{
    QASSERT(column < column_count && row < row_count);
	return m[column*row_count + row];
}

template <typename T>
inline mat2<T>& mat2<T>::operator=(mat2<T> const& m)
{
    memcpy(this->m, m.m, sizeof(mat2<T>));
}

template <typename T>
inline mat2<T> mat2<T>::operator*(mat2<T> const& other) const
{
	mat2<T> ret;
	return multiply(ret, *this, other);
}
template <typename T>
inline mat2<T> mat2<T>::operator+(mat2<T> const& other) const
{
	mat2<T> ret;
	return cwise::add(ret, *this, other);
}
template <typename T>
inline mat2<T> mat2<T>::operator-(mat2<T> const& other) const
{
	mat2<T> ret;
	return cwise::substract(ret, *this, other);
}
template <typename T>
inline mat2<T>& mat2<T>::operator*=(mat2<T> const& other)
{
	mat2<T> a(*this);
	return multiply(*this, a, other);
}
template <typename T>
inline mat2<T>& mat2<T>::operator+=(mat2<T> const& other)
{
	return cwise::add(*this, *this, other);
}
template <typename T>
inline mat2<T>& mat2<T>::operator-=(mat2<T> const& other)
{
	return cwise::substract(*this, *this, other);
}

template <typename T>
inline mat2<T> mat2<T>::operator*(T scalar) const
{
	mat2<T> ret;
	return cwise::multiply(ret, *this, scalar);
}
template <typename T>
inline mat2<T> mat2<T>::operator+(T scalar) const
{
	mat2<T> ret;
	return cwise::add(ret, *this, scalar);
}
template <typename T>
inline mat2<T> mat2<T>::operator-(T scalar) const
{
	mat2<T> ret;
	return cwise::substract(ret, *this, scalar);
}
template <typename T>
inline mat2<T>& mat2<T>::operator*=(T scalar)
{
	return cwise::multiply(*this, *this, scalar);
}
template <typename T>
inline mat2<T>& mat2<T>::operator+=(T scalar)
{
	return cwise::add(*this, *this, scalar);
}
template <typename T>
inline mat2<T>& mat2<T>::operator-=(T scalar)
{
	return cwise::substract(*this, *this, scalar);
}

}
