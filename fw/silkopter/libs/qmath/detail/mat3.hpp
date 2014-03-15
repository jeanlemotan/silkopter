namespace math
{


template<typename T>
mat3<T> const mat3<T>::zero(0);

template<typename T>
mat3<T> const mat3<T>::one(1);

template<typename T>
mat3<T> const mat3<T>::identity;

///////////////////////////////////////////////////////////////////////////////
//constructors
///////////////////////////////////////////////////////////////////////////////

template <typename T>
MATH_FORCE_INLINE mat3<T>::mat3()
{
	set_identity();
}

template <typename T>
MATH_FORCE_INLINE mat3<T>::mat3(ZUninitialized)
{
}

template <typename T>
MATH_FORCE_INLINE mat3<T>::mat3(T value)
{
	m[0] = m[1] = m[2] = value;
	m[3] = m[4] = m[5] = value;
	m[6] = m[7] = m[8] = value;
}

template <typename T>
MATH_FORCE_INLINE mat3<T>::mat3(mat3<T> const& other)
{
	*this = other;
}

template<typename T>
template<typename U>
MATH_FORCE_INLINE mat3<T>::mat3(mat3<U> const& other)
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
MATH_FORCE_INLINE mat3<T>::mat3(T const v0, T const v1, T const v2, T const v3, T const v4, T const v5, T const v6, T const v7, T const v8)
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
MATH_FORCE_INLINE mat3<T>::mat3(vec3<T> const& column0, vec3<T> const& column1, vec3<T> const& column2)
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

///////////////////////////////////////////////////////////////////////////////
//methods
///////////////////////////////////////////////////////////////////////////////

template<typename T>
MATH_FORCE_INLINE mat3<T>& mat3<T>::set(T const values[9])
{
	assert(values);
	memcpy(m, values, sizeof(T)*elementCount);
	return *this;
}

template <typename T>
MATH_FORCE_INLINE mat3<T>& mat3<T>::set_identity()
{
	m[0] = m[4] = (T)1;
	m[1] = m[2] = m[3] = m[5] = (T)0;
	m[8] = (T)1;
	m[6] = m[7] = (T)0;
	return *this;
}

template <typename T>
MATH_FORCE_INLINE bool mat3<T>::invert()
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

	T fInvDet = T(1)/fDet;
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
MATH_FORCE_INLINE mat3<T>& mat3<T>::transpose()
{
	std::swap(m[1], m[3]);
	std::swap(m[2], m[6]);
	std::swap(m[5], m[7]);
	return *this;
}

template <typename T>
MATH_FORCE_INLINE vec3<T> mat3<T>::get_row(uint8_t row) const
{
	assert(row < rowCount);
	return vec3<T>(m[row + 0], m[row + 3], m[row + 6]);
}
template <typename T>
MATH_FORCE_INLINE mat3<T>& mat3<T>::set_row(uint8_t row, vec3<T> const& v)
{
	assert(row < rowCount);
	m[row + 0] = v.x;
	m[row + 3] = v.y;
	m[row + 6] = v.z;
	return *this;
}

template <typename T>
MATH_FORCE_INLINE vec3<T> mat3<T>::get_column(uint8_t column) const
{
	assert(column < columnCount);
	uint8_t idx = column * rowCount;
	return vec3<T>(m[idx + 0], m[idx + 1], m[idx + 2]);
}
template <typename T>
MATH_FORCE_INLINE mat3<T>& mat3<T>::set_column(uint8_t column, vec3<T> const& v)
{
	assert(column < columnCount);
	uint8_t idx = column * rowCount;
	m[idx + 0] = v.x;
	m[idx + 1] = v.y;
	m[idx + 2] = v.z;
}

template <typename T>
MATH_FORCE_INLINE vec3<T> mat3<T>::get_axis_x() const
{
	return get_column(0);
}

template <typename T>
MATH_FORCE_INLINE vec3<T> mat3<T>::get_axis_y() const
{
	return get_column(1);
}

template <typename T>
MATH_FORCE_INLINE vec3<T> mat3<T>::get_axis_z() const
{
	return get_column(2);
}

template <typename T>
MATH_FORCE_INLINE vec3<T> mat3<T>::get_scale() const
{
	return vec3<T>(math::length(get_axis_x()), math::length(get_axis_y()), math::length(get_axis_z()));
}

template <typename T>
MATH_FORCE_INLINE mat3<T>& mat3<T>::set_axis_x(vec3<T> const& axis)
{
	m[0] = axis.x;
	m[1] = axis.y;
	m[2] = axis.z;
	return *this;
}

template <typename T>
MATH_FORCE_INLINE mat3<T>& mat3<T>::set_axis_y(vec3<T> const& axis)
{
	m[3] = axis.x;
	m[4] = axis.y;
	m[5] = axis.z;
	return *this;
}

template <typename T>
MATH_FORCE_INLINE mat3<T>& mat3<T>::set_axis_z(vec3<T> const& axis)
{
	m[6] = axis.x;
	m[7] = axis.y;
	m[8] = axis.z;
	return *this;
}

template <typename T>
MATH_FORCE_INLINE mat3<T>& mat3<T>::set_scale(vec3<T> const& s)
{
	m[0] = s.x;
	m[4] = s.y;
	m[8] = s.z;
	return *this;
}

template <typename T>
MATH_FORCE_INLINE mat3<T>& mat3<T>::post_scale(vec3<T> const& s)
{
	m[0] *= s.x;
	m[4] *= s.y;
	m[8] *= s.z;
	return *this;
}

//////////////////////////////////////////////////////////////////////////

template <typename T>
MATH_FORCE_INLINE bool mat3<T>::operator==(mat3<T> const& m) const
{
	return memcmp(this->m, m.m, sizeof(T) * elementCount) == 0;
}
template <typename T>
MATH_FORCE_INLINE bool mat3<T>::operator!=(mat3<T> const& m) const
{
	return !operator==(m);
}

///////////////////////////////////////////////////////////////////////////////
// indexing operators
///////////////////////////////////////////////////////////////////////////////

template <typename T>
MATH_FORCE_INLINE T* mat3<T>::data()
{
	return m;
}
template <typename T>
MATH_FORCE_INLINE T const* mat3<T>::data() const
{
	return m;
}

template <typename T>
MATH_FORCE_INLINE T& mat3<T>::operator()(uint8_t column, uint8_t row)
{
	assert(column < columnCount && row < rowCount);
	return m[column*rowCount + row];
}

template <typename T>
MATH_FORCE_INLINE T const& mat3<T>::operator()(uint8_t column, uint8_t row) const
{
	assert(column < columnCount && row < rowCount);
	return m[column*rowCount + row];
}

///////////////////////////////////////////////////////////////////////////////
// scalar operators
///////////////////////////////////////////////////////////////////////////////

// template <typename T>
// MATH_FORCE_INLINE mat3<T> mat3<T>::operator*(T scalar) const
// {
// 	mat3<T> temp(uninitialized);
// #ifdef XXXUSE_NEON_ASM
// 	asm volatile
// 		(		 
// 		"vldmia %0, { q0-q3 }	\n\t"
// 		"vdup.float q4, %1    	\n\t" // vdup splates a scalar into a vector
// 
// 		"vmul.float q0, q0, q4	\n\t" 	
// 		"vmul.float q1, q1, q4	\n\t"
// 		"vmul.float q2, q2, q4	\n\t"
// 		"vmul.float q3, q3, q4	\n\t"
// 
// 		"vstmia %2, { q0-q3 }	\n\t"
// 		: // no output
// 	: "r" (m), "r" (scalar), "r" (temp.m)     // input
// 		: "memory", 
// 		"q0", "q1", "q2", "q3", "q4"
// 		);
// #else
// 	temp.m[ 0] = m[ 0]*scalar;
// 	temp.m[ 1] = m[ 1]*scalar;
// 	temp.m[ 2] = m[ 2]*scalar;
// 	temp.m[ 3] = m[ 3]*scalar;
// 	temp.m[ 4] = m[ 4]*scalar;
// 	temp.m[ 5] = m[ 5]*scalar;
// 	temp.m[ 6] = m[ 6]*scalar;
// 	temp.m[ 7] = m[ 7]*scalar;
// 	temp.m[ 8] = m[ 8]*scalar;
// #endif
// 	return temp;
// }
// 
// template <typename T>
// MATH_FORCE_INLINE mat3<T>& mat3<T>::operator*=(T scalar)
// {
// #ifdef XXXUSE_NEON_ASM
// 	asm volatile
// 		(		 
// 		"vldmia %0, { q0-q3 }	\n\t"
// 		"vdup.float q4, %1    	\n\t" // vdup splates a scalar into a vector
// 
// 		"vmul.float q0, q0, q4	\n\t" 	
// 		"vmul.float q1, q1, q4	\n\t"
// 		"vmul.float q2, q2, q4	\n\t"
// 		"vmul.float q3, q3, q4	\n\t"
// 
// 		"vstmia %0, { q0-q3 }	\n\t"
// 		: // no output
// 	: "r" (m), "r" (scalar)     // input
// 		: "memory", 
// 		"q0", "q1", "q2", "q3", "q4"
// 		);
// #else
// 	m[ 0] *= scalar;
// 	m[ 1] *= scalar;
// 	m[ 2] *= scalar;
// 	m[ 3] *= scalar;
// 	m[ 4] *= scalar;
// 	m[ 5] *= scalar;
// 	m[ 6] *= scalar;
// 	m[ 7] *= scalar;
// 	m[ 8] *= scalar;
// #endif
// 	return *this;
// }

///////////////////////////////////////////////////////////////////////////////
// arithmetic operators
///////////////////////////////////////////////////////////////////////////////

// template <typename T>
// inline mat3<T> mat3<T>::operator*(const mat3<T>& other) const
// {
// 	mat3<T> ret(uninitialized);
// //	const T *__restrict m1 = m;
// //	const T *__restrict m2 = other.m;
// //	T *__restrict m3 = ret.m;
// 	const T* m1 = m;
// 	const T* m2 = other.m;
// 	T* m3 = ret.m;
// 	// using this code only on release and when on the iphone
// 	m3[0] = m1[0]*m2[0] + m1[3]*m2[1] + m1[6]*m2[2];
// 	m3[1] = m1[1]*m2[0] + m1[4]*m2[1] + m1[7]*m2[2];
// 	m3[2] = m1[2]*m2[0] + m1[5]*m2[1] + m1[8]*m2[2];
// 
// 	m3[3] = m1[0]*m2[3] + m1[3]*m2[4] + m1[6]*m2[5];
// 	m3[4] = m1[1]*m2[3] + m1[4]*m2[4] + m1[7]*m2[5];
// 	m3[5] = m1[2]*m2[3] + m1[5]*m2[4] + m1[8]*m2[5];
// 
// 	m3[6] = m1[0]*m2[6] + m1[3]*m2[7] + m1[6]*m2[8];
// 	m3[7] = m1[1]*m2[6] + m1[4]*m2[7] + m1[7]*m2[8];
// 	m3[8] = m1[2]*m2[6] + m1[5]*m2[7] + m1[8]*m2[8];
// 
// 	return ret;
// }
// 
// template <typename T>
// MATH_FORCE_INLINE mat3<T> mat3<T>::operator+(const mat3<T>& other) const
// {
// 	mat3<T> temp(uninitialized);
// 	temp[ 0] = m[ 0]+other[ 0];
// 	temp[ 1] = m[ 1]+other[ 1];
// 	temp[ 2] = m[ 2]+other[ 2];
// 	temp[ 3] = m[ 3]+other[ 3];
// 	temp[ 4] = m[ 4]+other[ 4];
// 	temp[ 5] = m[ 5]+other[ 5];
// 	temp[ 6] = m[ 6]+other[ 6];
// 	temp[ 7] = m[ 7]+other[ 7];
// 	temp[ 8] = m[ 8]+other[ 8];
// 	return temp;
// }
// 
// template <typename T>
// MATH_FORCE_INLINE mat3<T> mat3<T>::operator-(const mat3<T>& other) const
// {
// 	mat3<T> temp(uninitialized);
// 	temp.m[ 0] = m[ 0]-other.m[ 0];
// 	temp.m[ 1] = m[ 1]-other.m[ 1];
// 	temp.m[ 2] = m[ 2]-other.m[ 2];
// 	temp.m[ 3] = m[ 3]-other.m[ 3];
// 	temp.m[ 4] = m[ 4]-other.m[ 4];
// 	temp.m[ 5] = m[ 5]-other.m[ 5];
// 	temp.m[ 6] = m[ 6]-other.m[ 6];
// 	temp.m[ 7] = m[ 7]-other.m[ 7];
// 	temp.m[ 8] = m[ 8]-other.m[ 8];
// 	return temp;
// }

// template <typename T>
// MATH_FORCE_INLINE vec2<T> mat3<T>::operator*(const vec2<T>& p) const
// {
// 	T px = p.x;
// 	T py = p.y;
// 	T x = px*m[0] + py*m[3] + m[6];
// 	T y = px*m[1] + py*m[4] + m[7];
// 	return vec2<T>(x, y);
// }

// template <typename T>
// MATH_FORCE_INLINE vec3<T> mat3<T>::operator*(const vec3<T>& p) const
// {
// 	T px = p.x;
// 	T py = p.y;
// 	T pz = p.z;
// 	T x = px*m[0] + py*m[3] + m[6];
// 	T y = px*m[1] + py*m[4] + m[7];
// 	T z = px*m[2] + py*m[5] + m[8];
// 	return vec3<T>(x, y, z);
// }

// template <typename T>
// mat3<T>& mat3<T>::operator+=(const mat3<T>& other)
// {
// 	for (uint8_t i = 0; i < 9; i++)
// 	{
// 		m[i] += other.m[i];
// 	}
// 	return *this;
// }
// 
// template <typename T>
// mat3<T>& mat3<T>::operator-=(const mat3<T>& other)
// {
// 	for (uint8_t i = 0; i < 9; i++)
// 	{
// 		m[i] -= other.m[i];
// 	}
// 	return *this;
// }


template <typename T>
MATH_FORCE_INLINE mat3<T> mat3<T>::operator*(mat3<T> const& other) const
{
	mat3<T> ret;
	return multiply(ret, *this, other);
}
template <typename T>
MATH_FORCE_INLINE mat3<T> mat3<T>::operator+(mat3<T> const& other) const
{
	mat3<T> ret;
	return cwise::add(ret, *this, other);
}
template <typename T>
MATH_FORCE_INLINE mat3<T> mat3<T>::operator-(mat3<T> const& other) const
{
	mat3<T> ret;
	return cwise::substract(ret, *this, other);
}
template <typename T>
MATH_FORCE_INLINE mat3<T>& mat3<T>::operator*=(mat3<T> const& other)
{
	mat3<T> a(*this);
	return multiply(*this, a, other);
}
template <typename T>
MATH_FORCE_INLINE mat3<T>& mat3<T>::operator+=(mat3<T> const& other)
{
	return cwise::add(*this, *this, other);
}
template <typename T>
MATH_FORCE_INLINE mat3<T>& mat3<T>::operator-=(mat3<T> const& other)
{
	return cwise::substract(*this, *this, other);
}

template <typename T>
MATH_FORCE_INLINE mat3<T> mat3<T>::operator*(T scalar) const
{
	mat3<T> ret;
	return cwise::multiply(ret, *this, scalar);
}
template <typename T>
MATH_FORCE_INLINE mat3<T> mat3<T>::operator+(T scalar) const
{
	mat3<T> ret;
	return cwise::add(ret, *this, scalar);
}
template <typename T>
MATH_FORCE_INLINE mat3<T> mat3<T>::operator-(T scalar) const
{
	mat3<T> ret;
	return cwise::substract(ret, *this, scalar);
}
template <typename T>
MATH_FORCE_INLINE mat3<T>& mat3<T>::operator*=(T scalar)
{
	return cwise::multiply(*this, *this, scalar);
}
template <typename T>
MATH_FORCE_INLINE mat3<T>& mat3<T>::operator+=(T scalar)
{
	return cwise::add(*this, *this, scalar);
}
template <typename T>
MATH_FORCE_INLINE mat3<T>& mat3<T>::operator-=(T scalar)
{
	return cwise::substract(*this, *this, scalar);
}


}