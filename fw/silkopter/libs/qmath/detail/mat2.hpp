namespace math
{

template<typename T>
mat2<T> const mat2<T>::zero(0);

template<typename T>
mat2<T> const mat2<T>::one(1);

template<typename T>
mat2<T> const mat2<T>::identity;

///////////////////////////////////////////////////////////////////////////////
//constructors
///////////////////////////////////////////////////////////////////////////////

template <typename T>
MATH_FORCE_INLINE mat2<T>::mat2()
{
	set_identity();
}

template <typename T>
MATH_FORCE_INLINE mat2<T>::mat2(ZUninitialized)
{
}

template <typename T>
MATH_FORCE_INLINE mat2<T>::mat2(ZRotation, T angle)
{
	T s, c;
	sin_cos(angle, s, c);

	m[0] = c;
	m[1] = s;
	m[2] = -s;
	m[3] = c;
}

template <typename T>
MATH_FORCE_INLINE mat2<T>::mat2(T value)
{
	m[0] = m[1] = value;
	m[2] = m[3] = value;
}

template <typename T>
MATH_FORCE_INLINE mat2<T>::mat2(mat2<T> const& other)
{
	*this = other;
}

template<typename T>
template<typename U>
MATH_FORCE_INLINE mat2<T>::mat2(mat2<U> const& other)
{
	m[0] = (T)other.m[0];
	m[1] = (T)other.m[1];
	m[2] = (T)other.m[2];
	m[3] = (T)other.m[3];
}

template<typename T>
MATH_FORCE_INLINE mat2<T>::mat2(T const v0, T const v1, T const v2, T const v3)
{
	m[0] = v0;
	m[1] = v1;
	m[2] = v2;
	m[3] = v3;
}

template<typename T>
MATH_FORCE_INLINE mat2<T>::mat2(vec2<T> const& column0, vec2<T> const& column1)
{
	m[0] = column0.x;
	m[1] = column0.y;
	m[2] = column1.x;
	m[3] = column1.y;
}

///////////////////////////////////////////////////////////////////////////////
//methods
///////////////////////////////////////////////////////////////////////////////

template<typename T>
MATH_FORCE_INLINE mat2<T>& mat2<T>::set(T const values[4])
{
	assert(values);
	memcpy(m, values, sizeof(T)*4);
	return *this;
}

template <typename T>
MATH_FORCE_INLINE mat2<T>& mat2<T>::set_identity()
{
	m[0] = m[3] = (T)1;
	m[1] = m[2] = (T)0;
	return *this;
}

template <typename T>
MATH_FORCE_INLINE bool mat2<T>::invert()
{
	T sav = m[0];
	T div = (m[0]*m[3] - m[1]*m[2]);
	if (is_zero(div))
	{
		return false;
	}

	div = ((T)1) / div;
	m[0] = div*m[3];
	m[1] = -div*m[1];
	m[2] = -div*m[2];
	m[3] = div*sav;

	return true;
}

template <typename T>
MATH_FORCE_INLINE mat2<T>& mat2<T>::transpose()
{
	std::swap(m[1], m[2]);
	return *this;
}

template <typename T>
MATH_FORCE_INLINE vec2<T> mat2<T>::get_row(uint8_t row) const
{
	assert(row < 2);
	return vec2<T>(m[row], m[row + 2]);
}
template <typename T>
MATH_FORCE_INLINE mat2<T>& mat2<T>::set_row(uint8_t row, vec2<T> const& v)
{
	assert(row < 2);
	m[row    ] = v.x;
	m[row + 2] = v.y;
	return *this;
}

template <typename T>
MATH_FORCE_INLINE vec2<T> const& mat2<T>::get_column(uint8_t column) const
{
	assert(column < 2);
	uint8_t idx = column * 2;
	return *(vec2<T> const*)(&m[idx]);
}
template <typename T>
MATH_FORCE_INLINE mat2<T>& mat2<T>::set_column(uint8_t column, vec2<T> const& v)
{
	assert(column < 2);
	uint8_t idx = column * 2;
	m[idx    ] = v.x;
	m[idx + 1] = v.y;
	return *this;
}

template <typename T>
MATH_FORCE_INLINE vec2<T> const& mat2<T>::get_axis_x() const
{
	return get_column(0);
}

template <typename T>
MATH_FORCE_INLINE vec2<T> const& mat2<T>::get_axis_y() const
{
	return get_column(1);
}

template <typename T>
MATH_FORCE_INLINE vec2<T> mat2<T>::get_scale() const
{
	return vec2<T>(length(get_axis_x()), length(get_axis_y()));
}

template <typename T>
MATH_FORCE_INLINE mat2<T>& mat2<T>::set_axis_x(vec2<T> const& axis)
{
	m[0] = axis.x;
	m[1] = axis.y;
	return *this;
}

template <typename T>
MATH_FORCE_INLINE mat2<T>& mat2<T>::set_axis_y(vec2<T> const& axis)
{
	m[2] = axis.x;
	m[3] = axis.y;
	return *this;
}

template <typename T>
MATH_FORCE_INLINE mat2<T>& mat2<T>::set_scale(vec2<T> const& s)
{
	m[ 0] = s.x;
	m[ 3] = s.y;
	return *this;
}

//////////////////////////////////////////////////////////////////////////

template <typename T>
MATH_FORCE_INLINE bool mat2<T>::operator==(mat2<T> const& m) const
{
	return memcmp(this->m, m.m, sizeof(T) * 4) == 0;
}
template <typename T>
MATH_FORCE_INLINE bool mat2<T>::operator!=(mat2<T> const& m) const
{
	return !operator==(m);
}

///////////////////////////////////////////////////////////////////////////////
// indexing operators
///////////////////////////////////////////////////////////////////////////////

template <typename T>
MATH_FORCE_INLINE T* mat2<T>::data()
{
	return m;
}
template <typename T>
MATH_FORCE_INLINE T const* mat2<T>::data() const
{
	return m;
}

template <typename T>
MATH_FORCE_INLINE T& mat2<T>::operator()(uint8_t column, uint8_t row)
{
	assert(column < columnCount && row < rowCount);
	return m[column*rowCount + row];
}

template <typename T>
MATH_FORCE_INLINE T const& mat2<T>::operator()(uint8_t column, uint8_t row) const
{
	assert(column < columnCount && row < rowCount);
	return m[column*rowCount + row];
}

template <typename T>
MATH_FORCE_INLINE mat2<T> mat2<T>::operator*(mat2<T> const& other) const
{
	mat2<T> ret;
	return multiply(ret, *this, other);
}
template <typename T>
MATH_FORCE_INLINE mat2<T> mat2<T>::operator+(mat2<T> const& other) const
{
	mat2<T> ret;
	return cwise::add(ret, *this, other);
}
template <typename T>
MATH_FORCE_INLINE mat2<T> mat2<T>::operator-(mat2<T> const& other) const
{
	mat2<T> ret;
	return cwise::substract(ret, *this, other);
}
template <typename T>
MATH_FORCE_INLINE mat2<T>& mat2<T>::operator*=(mat2<T> const& other)
{
	mat2<T> a(*this);
	return multiply(*this, a, other);
}
template <typename T>
MATH_FORCE_INLINE mat2<T>& mat2<T>::operator+=(mat2<T> const& other)
{
	return cwise::add(*this, *this, other);
}
template <typename T>
MATH_FORCE_INLINE mat2<T>& mat2<T>::operator-=(mat2<T> const& other)
{
	return cwise::substract(*this, *this, other);
}

template <typename T>
MATH_FORCE_INLINE mat2<T> mat2<T>::operator*(T scalar) const
{
	mat2<T> ret;
	return cwise::multiply(ret, *this, scalar);
}
template <typename T>
MATH_FORCE_INLINE mat2<T> mat2<T>::operator+(T scalar) const
{
	mat2<T> ret;
	return cwise::add(ret, *this, scalar);
}
template <typename T>
MATH_FORCE_INLINE mat2<T> mat2<T>::operator-(T scalar) const
{
	mat2<T> ret;
	return cwise::substract(ret, *this, scalar);
}
template <typename T>
MATH_FORCE_INLINE mat2<T>& mat2<T>::operator*=(T scalar)
{
	return cwise::multiply(*this, *this, scalar);
}
template <typename T>
MATH_FORCE_INLINE mat2<T>& mat2<T>::operator+=(T scalar)
{
	return cwise::add(*this, *this, scalar);
}
template <typename T>
MATH_FORCE_INLINE mat2<T>& mat2<T>::operator-=(T scalar)
{
	return cwise::substract(*this, *this, scalar);
}

}