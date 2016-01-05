namespace math
{

template<typename T> mat4<T> const mat4<T>::zero(0);																		   \
template<typename T> mat4<T> const mat4<T>::one(1);																		   \
template<typename T> mat4<T> const mat4<T>::identity;																		   \


template<typename T>
inline mat4<T>::mat4()
{
	set_identity();
}

template <typename T>
inline mat4<T>::mat4(math::ZUninitialized)
{
}

template <typename T>
inline mat4<T>::mat4(T value)
{
    QASSERT(!is_nan(value));
    m[ 0] = m[ 1] = m[ 2] = m[ 3] = value;
	m[ 4] = m[ 5] = m[ 6] = m[ 7] = value;
	m[ 8] = m[ 9] = m[10] = m[11] = value;
	m[12] = m[13] = m[14] = m[15] = value;
}

template<typename T>
template<typename U>
inline mat4<T>::mat4(mat4<U> const& other)
{
	//column0
	m[ 0] = (T)other.m[ 0];
	m[ 1] = (T)other.m[ 1];
	m[ 2] = (T)other.m[ 2];
	m[ 3] = (T)other.m[ 3];
	//column1
	m[ 4] = (T)other.m[ 4];
	m[ 5] = (T)other.m[ 5];
	m[ 6] = (T)other.m[ 6];
	m[ 7] = (T)other.m[ 7];
	//column2
	m[ 8] = (T)other.m[ 8];
	m[ 9] = (T)other.m[ 9];
	m[10] = (T)other.m[10];
	m[11] = (T)other.m[11];
	//column3
	m[12] = (T)other.m[12];
	m[13] = (T)other.m[13];
	m[14] = (T)other.m[14];
	m[15] = (T)other.m[15];
}

template<typename T>
inline mat4<T>::mat4(T const v0, T const v1, T const v2, T const v3,
			  T const v4, T const v5, T const v6, T const v7,
			  T const v8, T const v9, T const v10, T const v11,
			  T const v12, T const v13, T const v14, T const v15)
{
	//column0
	m[ 0] = v0;
	m[ 1] = v1;
	m[ 2] = v2;
	m[ 3] = v3;
	//column1
	m[ 4] = v4;
	m[ 5] = v5;
	m[ 6] = v6;
	m[ 7] = v7;
	//column2
	m[ 8] = v8;
	m[ 9] = v9;
	m[10] = v10;
	m[11] = v11;
	//column3
	m[12] = v12;
	m[13] = v13;
	m[14] = v14;
	m[15] = v15;

    QASSERT(!is_nan(m[0]) && !is_nan(m[1]) && !is_nan(m[2]) && !is_nan(m[3]) &&
            !is_nan(m[4]) && !is_nan(m[5]) && !is_nan(m[6]) && !is_nan(m[7]) &&
            !is_nan(m[8]) && !is_nan(m[9]) && !is_nan(m[10]) && !is_nan(m[11]) &&
            !is_nan(m[12]) && !is_nan(m[13]) && !is_nan(m[14]) && !is_nan(m[15]));
}

template<typename T>
inline mat4<T>::mat4(vec4<T> const& column0, vec4<T> const& column1, vec4<T> const& column2, vec4<T> const& column3)
{
    columns[0] = column0;
    columns[1] = column1;
    columns[2] = column2;
    columns[3] = column3;
}

///////////////////////////////////////////////////////////////////////////////
//methods
///////////////////////////////////////////////////////////////////////////////

template<typename T>
inline void mat4<T>::set(T const values[16])
{
    QASSERT(values);
	memcpy(m, values, sizeof(T)*16);
    QASSERT(!is_nan(m[0]) && !is_nan(m[1]) && !is_nan(m[2]) && !is_nan(m[3]) &&
            !is_nan(m[4]) && !is_nan(m[5]) && !is_nan(m[6]) && !is_nan(m[7]) &&
            !is_nan(m[8]) && !is_nan(m[9]) && !is_nan(m[10]) && !is_nan(m[11]) &&
            !is_nan(m[12]) && !is_nan(m[13]) && !is_nan(m[14]) && !is_nan(m[15]));
}

template <typename T>
inline void mat4<T>::set_identity()
{
	m[ 0] = m[ 5] = m[10] = (T)1.0;
	m[ 1] = m[ 2] = m[ 3] = m[ 4] = m[ 6] = m[ 7] = m[ 8] = m[ 9] = m[11] = (T)0.0;
	m[15] = (T)1.0;
	m[12] = m[13] = m[14] = (T)0.0;
}

template <typename T>
template <class Policy>
inline bool mat4<T>::invert()
{
	T t0 = m[10] * m[15] - m[11] * m[14];
	T t1 = m[6] * m[15] - m[7] * m[14];
	T t2 = m[6] * m[11] - m[7] * m[10];
	T t3 = m[2] * m[15] - m[3] * m[14];
	T t4 = m[2] * m[11] - m[3] * m[10];
	T t5 = m[2] * m[7] - m[3] * m[6];

	T t6 = m[8] * m[13] - m[9] * m[12];
	T t7 = m[4] * m[13] - m[5] * m[12];
	T t8 = m[4] * m[9] - m[5] * m[8];
	T t9 = m[0] * m[13] - m[1] * m[12];
	T t10 = m[0] * m[9] - m[1] * m[8];
	T t11 = m[0] * m[5] - m[1] * m[4];

	T det = t0 * t11 - t1 * t10 + t2 * t9 + t3 * t8 - t4 * t7 + t5 * t6;
	if (is_zero(det))
	{
		return false;
	}

	det = math::inverse<T, Policy>(det);
	T ft0 = (T)(t0 * det);
	T ft1 = (T)(t1 * det);
	T ft2 = (T)(t2 * det);
	T ft3 = (T)(t3 * det);
	T ft4 = (T)(t4 * det);
	T ft5 = (T)(t5 * det);
	T ft6 = (T)(t6 * det);
	T ft7 = (T)(t7 * det);
	T ft8 = (T)(t8 * det);
	T ft9 = (T)(t9 * det);
	T ft10 = (T)(t10 * det);
	T ft11 = (T)(t11 * det);

	mat4<T> out;
	out.m[0] = m[5] * ft0 - m[9] * ft1 + m[13] * ft2;
	out.m[1] = m[9] * ft3 - m[1] * ft0 - m[13] * ft4;
	out.m[2] = m[1] * ft1 - m[5] * ft3 + m[13] * ft5;
	out.m[3] = m[5] * ft4 - m[1] * ft2 - m[9] * ft5;

	out.m[4] = m[8] * ft1 - m[4] * ft0 - m[12] * ft2;
	out.m[5] = m[0] * ft0 - m[8] * ft3 + m[12] * ft4;
	out.m[6] = m[4] * ft3 - m[0] * ft1 - m[12] * ft5;
	out.m[7] = m[0] * ft2 - m[4] * ft4 + m[8] * ft5;

	out.m[8] = m[7] * ft6 - m[11] * ft7 + m[15] * ft8;
	out.m[9] = m[11] * ft9 - m[3] * ft6 - m[15] * ft10;
	out.m[10] = m[3] * ft7 - m[7] * ft9 + m[15] * ft11;
	out.m[11] = m[7] * ft10 - m[3] * ft8 - m[11] * ft11;

	out.m[12] = m[10] * ft7 - m[6] * ft6 - m[14] * ft8;
	out.m[13] = m[2] * ft6 - m[10] * ft9 + m[14] * ft10;
	out.m[14] = m[6] * ft9 - m[2] * ft7 - m[14] * ft11;
	out.m[15] = m[2] * ft8 - m[6] * ft10 + m[10] * ft11;

	*this = out;

    QASSERT(!is_nan(m[0]) && !is_nan(m[1]) && !is_nan(m[2]) && !is_nan(m[3]) &&
            !is_nan(m[4]) && !is_nan(m[5]) && !is_nan(m[6]) && !is_nan(m[7]) &&
            !is_nan(m[8]) && !is_nan(m[9]) && !is_nan(m[10]) && !is_nan(m[11]) &&
            !is_nan(m[12]) && !is_nan(m[13]) && !is_nan(m[14]) && !is_nan(m[15]));

    return true;
}

template <typename T>
inline void mat4<T>::transpose()
{
	std::swap(m[ 1], m[ 4]);
	std::swap(m[ 2], m[ 8]);
	std::swap(m[ 3], m[12]);
	std::swap(m[ 6], m[ 9]);
	std::swap(m[ 7], m[13]);
	std::swap(m[11], m[14]);
}

template <typename T>
inline vec4<T> mat4<T>::get_row(uint8_t row) const
{
    QASSERT(row < 4);
	return vec4<T>(m[row + 0], m[row + 4], m[row + 8], m[row + 12]);
}

template <typename T>
inline void mat4<T>::set_row(uint8_t row, vec4<T> const& v)
{
    QASSERT(!cwise::any(cwise::is_nan(v)));
    QASSERT(row < 4);
	m[row + 0] = v.x;
	m[row + 4] = v.y;
	m[row + 8] = v.z;
	m[row + 12] = v.w;
}

template <typename T>
inline vec4<T> const& mat4<T>::get_column(uint8_t column) const
{
    QASSERT(column < 4);
    return columns[column];
}

template <typename T>
inline void mat4<T>::set_column(uint8_t column, vec4<T> const& v)
{
    QASSERT(!cwise::any(cwise::is_nan(v)));
    QASSERT(column < 4);
    columns[column] = v;
}

template <typename T>
inline vec4<T> const& mat4<T>::get_axis_x() const
{
	return get_column(0);
}

template <typename T>
inline vec4<T> const& mat4<T>::get_axis_y() const
{
	return get_column(1);
}

template <typename T>
inline vec4<T> const& mat4<T>::get_axis_z() const
{
	return get_column(2);
}

template <typename T>
inline vec4<T> const& mat4<T>::get_axis_w() const
{
	return get_column(3);
}

template <typename T>
inline void mat4<T>::set_axis_x(vec4<T> const& axis)
{
    QASSERT(!cwise::any(cwise::is_nan(axis)));
    set_column(0, axis);
}

template <typename T>
inline void mat4<T>::set_axis_y(vec4<T> const& axis)
{
    QASSERT(!cwise::any(cwise::is_nan(axis)));
    set_column(1, axis);
}

template <typename T>
inline void mat4<T>::set_axis_z(vec4<T> const& axis)
{
    QASSERT(!cwise::any(cwise::is_nan(axis)));
    set_column(2, axis);
}

template <typename T>
inline void mat4<T>::set_axis_w(vec4<T> const& axis)
{
    QASSERT(!cwise::any(cwise::is_nan(axis)));
    set_column(3, axis);
}

//////////////////////////////////////////////////////////////////////////

template <typename T>
inline bool mat4<T>::operator==(mat4<T> const& m) const
{
	return memcmp(this->m, m.m, sizeof(T) * element_count) == 0;
}
template <typename T>
inline bool mat4<T>::operator!=(mat4<T> const& m) const
{
	return !operator==(m);
}

///////////////////////////////////////////////////////////////////////////////
// indexing operators
///////////////////////////////////////////////////////////////////////////////

template <typename T>
inline T* mat4<T>::data()
{
	return m;
}
template <typename T>
inline T const* mat4<T>::data() const
{
	return m;
}

template <typename T>
inline T& mat4<T>::operator()(uint8_t column, uint8_t row)
{
    QASSERT(column < column_count && row < row_count);
	return m[column*row_count + row];
}

template <typename T>
inline T const& mat4<T>::operator()(uint8_t column, uint8_t row) const
{
    QASSERT(column < column_count && row < row_count);
	return m[column*row_count + row];
}

template <typename T>
inline mat4<T> mat4<T>::operator*(mat4<T> const& other) const
{
	mat4<T> ret;
	return multiply(ret, *this, other);
}
template <typename T>
inline mat4<T> mat4<T>::operator+(mat4<T> const& other) const
{
	mat4<T> ret;
	return cwise::add(ret, *this, other);
}
template <typename T>
inline mat4<T> mat4<T>::operator-(mat4<T> const& other) const
{
	mat4<T> ret;
	return cwise::substract(ret, *this, other);
}
template <typename T>
inline mat4<T>& mat4<T>::operator*=(mat4<T> const& other)
{
	mat4<T> a(*this);
	return multiply(*this, a, other);
}
template <typename T>
inline mat4<T>& mat4<T>::operator+=(mat4<T> const& other)
{
	return cwise::add(*this, *this, other);
}
template <typename T>
inline mat4<T>& mat4<T>::operator-=(mat4<T> const& other)
{
	return cwise::substract(*this, *this, other);
}

template <typename T>
inline mat4<T> mat4<T>::operator*(T scalar) const
{
    QASSERT(!is_nan(scalar));
    mat4<T> ret;
	return cwise::multiply(ret, *this, scalar);
}
template <typename T>
inline mat4<T> mat4<T>::operator+(T scalar) const
{
    QASSERT(!is_nan(scalar));
    mat4<T> ret;
	return cwise::add(ret, *this, scalar);
}
template <typename T>
inline mat4<T> mat4<T>::operator-(T scalar) const
{
    QASSERT(!is_nan(scalar));
    mat4<T> ret;
	return cwise::substract(ret, *this, scalar);
}
template <typename T>
inline mat4<T>& mat4<T>::operator*=(T scalar)
{
    QASSERT(!is_nan(scalar));
    return cwise::multiply(*this, *this, scalar);
}
template <typename T>
inline mat4<T>& mat4<T>::operator+=(T scalar)
{
    QASSERT(!is_nan(scalar));
    return cwise::add(*this, *this, scalar);
}
template <typename T>
inline mat4<T>& mat4<T>::operator-=(T scalar)
{
    QASSERT(!is_nan(scalar));
    return cwise::substract(*this, *this, scalar);
}



}
