namespace math
{

template<typename T> mat3<T> const mat3<T>::zero(0);																		   \
template<typename T> mat3<T> const mat3<T>::one(1);																		   \
template<typename T> mat3<T> const mat3<T>::identity;																		   \

///////////////////////////////////////////////////////////////////////////////
//constructors
///////////////////////////////////////////////////////////////////////////////

template <typename T>
inline mat3<T>::mat3()
: column0(math::uninitialized)
, column1(math::uninitialized)
, column2(math::uninitialized)
{
	set_identity();
}

template <typename T>
inline mat3<T>::mat3(ZUninitialized)
: column0(math::uninitialized)
, column1(math::uninitialized)
, column2(math::uninitialized)
{
}

template <typename T>
inline mat3<T>::mat3(T value)
: column0(math::uninitialized)
, column1(math::uninitialized)
, column2(math::uninitialized)
{
	m[0] = m[1] = m[2] = value;
	m[3] = m[4] = m[5] = value;
	m[6] = m[7] = m[8] = value;
}

template<typename T>
template<typename U>
inline mat3<T>::mat3(mat3<U> const& other)
: column0(math::uninitialized)
, column1(math::uninitialized)
, column2(math::uninitialized)
{
	m[0] = (T)other.m[0];
	m[1] = (T)other.m[1];
	m[2] = (T)other.m[2];
	m[3] = (T)other.m[3];
	m[4] = (T)other.m[4];
	m[5] = (T)other.m[5];
	m[6] = (T)other.m[6];
	m[7] = (T)other.m[7];
	m[8] = (T)other.m[8];
}

template<typename T>
inline mat3<T>::mat3(T const v0, T const v1, T const v2, T const v3, T const v4, T const v5, T const v6, T const v7, T const v8)
: column0(math::uninitialized)
, column1(math::uninitialized)
, column2(math::uninitialized)
{
	m[0] = v0;
	m[1] = v1;
	m[2] = v2;
	m[3] = v3;
	m[4] = v4;
	m[5] = v5;
	m[6] = v6;
	m[7] = v7;
	m[8] = v8;
}

template<typename T>
inline mat3<T>::mat3(vec3<T> const& column0, vec3<T> const& column1, vec3<T> const& column2)
: column0(column0)
, column1(column1)
, column2(column2)
{
	m[0] = column0.x;
	m[1] = column0.y;
	m[2] = column0.z;
	m[3] = column1.x;
	m[4] = column1.y;
	m[5] = column1.z;
	m[6] = column2.x;
	m[7] = column2.y;
	m[8] = column2.z;
}

///! Creates a world space lookat matrix (front axis is -yaxis, up axis is zaxis)
/// This lookAt method computes a look at matrix in jet coordinate system (3dmax biped).
/// This means that when you send a front of (0,1,0) and an up of (0,0,1) the resulting matrix is identity.
template<class T>
template<class Policy>
inline mat3<T> mat3<T>::look_at(vec3<T> const& front, vec3<T> const& up)
{
    vec3<T> axisY = normalized<T, Policy>(front);
    vec3<T> axisX = cross(axisY, normalized<T, Policy>(up));
    axisX.template normalize<Policy>(); //this normalize is mandatory because axisY and up may be unitary but they hardly are orthogonal
    vec3<T> axisZ = cross(axisX, axisY);

    mat3<T> mat;
    T* m = mat.m;
    m[0] = axisX.x;
    m[1] = axisX.y;
    m[2] = axisX.z;

    m[3] = axisY.x;
    m[4] = axisY.y;
    m[5] = axisY.z;

    m[6] = axisZ.x;
    m[7] = axisZ.y;
    m[8] = axisZ.z;

    return mat;
}

///////////////////////////////////////////////////////////////////////////////
//methods
///////////////////////////////////////////////////////////////////////////////

template<typename T>
inline void mat3<T>::set(T const values[9])
{
    QASSERT(values);
	memcpy(m, values, sizeof(T)*element_count);
}

template <typename T>
inline void mat3<T>::set_identity()
{
	m[0] = m[4] = (T)1;
	m[1] = m[2] = m[3] = m[5] = (T)0;
	m[8] = (T)1;
	m[6] = m[7] = (T)0;
}

template <typename T>
template <class Policy>
inline bool mat3<T>::invert()
{
	// http://www.geometrictools.com/LibFoundation/Mathematics/Wm4Matrix3.inl
	// Invert a 3x3 using cofactors.  This is faster than using a generic
	// Gaussian elimination because of the loop overhead of such a method.

	mat3<T> inv;
	inv.m[0] = m[4]*m[8] - m[5]*m[7];
	inv.m[1] = m[2]*m[7] - m[1]*m[8];
	inv.m[2] = m[1]*m[5] - m[2]*m[4];
	inv.m[3] = m[5]*m[6] - m[3]*m[8];
	inv.m[4] = m[0]*m[8] - m[2]*m[6];
	inv.m[5] = m[2]*m[3] - m[0]*m[5];
	inv.m[6] = m[3]*m[7] - m[4]*m[6];
	inv.m[7] = m[1]*m[6] - m[0]*m[7];
	inv.m[8] = m[0]*m[4] - m[1]*m[3];

	T fDet =
		m[0]*inv.m[0] +
		m[1]*inv.m[3] +
		m[2]*inv.m[6];

	if (is_zero(fDet))
	{
		return false;
	}

	T fInvDet = inverse<T, Policy>(fDet);
	inv.m[0] *= fInvDet;
	inv.m[1] *= fInvDet;
	inv.m[2] *= fInvDet;
	inv.m[3] *= fInvDet;
	inv.m[4] *= fInvDet;
	inv.m[5] *= fInvDet;
	inv.m[6] *= fInvDet;
	inv.m[7] *= fInvDet;
	inv.m[8] *= fInvDet;

	*this = inv;
	return true;
}

template <typename T>
inline void mat3<T>::transpose()
{
	std::swap(m[1], m[3]);
	std::swap(m[2], m[6]);
	std::swap(m[5], m[7]);
}

template <typename T>
inline vec3<T> mat3<T>::get_row(uint8_t row) const
{
    QASSERT(row < row_count);
	return vec3<T>(m[row + 0], m[row + 3], m[row + 6]);
}
template <typename T>
inline void mat3<T>::set_row(uint8_t row, vec3<T> const& v)
{
    QASSERT(row < row_count);
	m[row + 0] = v.x;
	m[row + 3] = v.y;
	m[row + 6] = v.z;
}

template <typename T>
inline vec3<T> const& mat3<T>::get_column(uint8_t column) const
{
    QASSERT(column < column_count);
	uint8_t idx = column * row_count;
	return reinterpret_cast<vec3<T> const&>(m[idx + 0]);
}
template <typename T>
inline void mat3<T>::set_column(uint8_t column, vec3<T> const& v)
{
    QASSERT(column < column_count);
	uint8_t idx = column * row_count;
	m[idx + 0] = v.x;
	m[idx + 1] = v.y;
	m[idx + 2] = v.z;
}

template <typename T>
inline vec3<T> const& mat3<T>::get_axis_x() const
{
	return get_column(0);
}

template <typename T>
inline vec3<T> const& mat3<T>::get_axis_y() const
{
	return get_column(1);
}

template <typename T>
inline vec3<T> const& mat3<T>::get_axis_z() const
{
	return get_column(2);
}

template <typename T>
inline vec3<T> mat3<T>::get_scale() const
{
	return vec3<T>(math::length(get_axis_x()), math::length(get_axis_y()), math::length(get_axis_z()));
}

template <typename T>
inline void mat3<T>::set_axis_x(vec3<T> const& axis)
{
	m[0] = axis.x;
	m[1] = axis.y;
	m[2] = axis.z;
}

template <typename T>
inline void mat3<T>::set_axis_y(vec3<T> const& axis)
{
	m[3] = axis.x;
	m[4] = axis.y;
	m[5] = axis.z;
}

template <typename T>
inline void mat3<T>::set_axis_z(vec3<T> const& axis)
{
	m[6] = axis.x;
	m[7] = axis.y;
	m[8] = axis.z;
}

template <typename T>
inline void mat3<T>::set_scale(vec3<T> const& s)
{
	m[0] = s.x;
	m[4] = s.y;
	m[8] = s.z;
}

template <typename T>
inline void mat3<T>::post_scale(vec3<T> const& s)
{
	m[0] *= s.x;
	m[4] *= s.y;
	m[8] *= s.z;
}

//////////////////////////////////////////////////////////////////////////

template <typename T>
inline bool mat3<T>::operator==(mat3<T> const& m) const
{
	return memcmp(this->m, m.m, sizeof(T) * element_count) == 0;
}
template <typename T>
inline bool mat3<T>::operator!=(mat3<T> const& m) const
{
	return !operator==(m);
}

///////////////////////////////////////////////////////////////////////////////
// indexing operators
///////////////////////////////////////////////////////////////////////////////

template <typename T>
inline T* mat3<T>::data()
{
	return m;
}
template <typename T>
inline T const* mat3<T>::data() const
{
	return m;
}

template <typename T>
inline T& mat3<T>::operator()(uint8_t column, uint8_t row)
{
    QASSERT(column < column_count && row < row_count);
	return m[column*row_count + row];
}

template <typename T>
inline T const& mat3<T>::operator()(uint8_t column, uint8_t row) const
{
    QASSERT(column < column_count && row < row_count);
	return m[column*row_count + row];
}

template <typename T>
inline mat3<T> mat3<T>::operator*(mat3<T> const& other) const
{
	mat3<T> ret(math::uninitialized);
	return multiply(ret, *this, other);
}
template <typename T>
inline mat3<T> mat3<T>::operator+(mat3<T> const& other) const
{
	mat3<T> ret(math::uninitialized);
	return cwise::add(ret, *this, other);
}
template <typename T>
inline mat3<T> mat3<T>::operator-(mat3<T> const& other) const
{
	mat3<T> ret(math::uninitialized);
	return cwise::substract(ret, *this, other);
}
template <typename T>
inline mat3<T>& mat3<T>::operator*=(mat3<T> const& other)
{
	mat3<T> a(*this);
	return multiply(*this, a, other);
}
template <typename T>
inline mat3<T>& mat3<T>::operator+=(mat3<T> const& other)
{
	return cwise::add(*this, *this, other);
}
template <typename T>
inline mat3<T>& mat3<T>::operator-=(mat3<T> const& other)
{
	return cwise::substract(*this, *this, other);
}

template <typename T>
inline mat3<T> mat3<T>::operator*(T scalar) const
{
	mat3<T> ret(math::uninitialized);
	return cwise::multiply(ret, *this, scalar);
}
template <typename T>
inline mat3<T> mat3<T>::operator+(T scalar) const
{
	mat3<T> ret(math::uninitialized);
	return cwise::add(ret, *this, scalar);
}
template <typename T>
inline mat3<T> mat3<T>::operator-(T scalar) const
{
	mat3<T> ret(math::uninitialized);
	return cwise::substract(ret, *this, scalar);
}
template <typename T>
inline mat3<T>& mat3<T>::operator*=(T scalar)
{
	return cwise::multiply(*this, *this, scalar);
}
template <typename T>
inline mat3<T>& mat3<T>::operator+=(T scalar)
{
	return cwise::add(*this, *this, scalar);
}
template <typename T>
inline mat3<T>& mat3<T>::operator-=(T scalar)
{
	return cwise::substract(*this, *this, scalar);
}


}
