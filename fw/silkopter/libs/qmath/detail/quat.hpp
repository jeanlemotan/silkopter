namespace math
{


template<typename T>
quat<T> const quat<T>::zero(0, 0, 0, 1);

template<typename T>
quat<T> const quat<T>::one(1, 1, 1, 1);

template<typename T>
quat<T> const quat<T>::identity;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
// constructors
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <typename T> 
MATH_FORCE_INLINE quat<T>::quat() : x(0), y(0), z(0), w(1) {}

template <typename T> 
MATH_FORCE_INLINE quat<T>::quat(ZUninitialized) {}

template <typename T>
MATH_FORCE_INLINE quat<T>::quat(T _x, T _y, T _z, T _w) : x(_x), y(_y), z(_z), w(_w) {}

template <typename T>
MATH_FORCE_INLINE quat<T>::quat(quat<T> const& q) : x(q.x), y(q.y), z(q.z), w(q.w) {}

template <typename T>
MATH_FORCE_INLINE quat<T>::quat(mat3<T> const& mat)
{
	set_from_mat3(mat);
}

template <typename T>
MATH_FORCE_INLINE quat<T>::quat(mat4<T> const& mat)
{
	set_from_mat4(mat);
}

template <typename T>
MATH_FORCE_INLINE quat<T>::quat(trans3d<T> const& mat)
{
	set_from_trans3d(mat);
}

template <typename T>
MATH_FORCE_INLINE quat<T>::quat(ZAxisX, T angle) : y(0), z(0)
{
	T const halfa = angle * T(0.5);
	sin_cos(halfa, x, w);
}
template <typename T>
MATH_FORCE_INLINE quat<T>::quat(ZAxisY, T angle) : x(0), z(0)
{
	T const halfa = angle * T(0.5);
	sin_cos(halfa, y, w);
}
template <typename T>
MATH_FORCE_INLINE quat<T>::quat(ZAxisZ, T angle) : x(0), y(0)
{
	T const halfa = angle * T(0.5);
	sin_cos(halfa, z, w);
}
template <typename T>
MATH_FORCE_INLINE quat<T>::quat(ZAngleAxis, T angle, vec3<T> const& axis)
{
	set_from_angle_axis(angle, axis);
}
template <typename T>
MATH_FORCE_INLINE quat<T>::quat(ZEulerXYZ, vec3<T> const& angles)
{
	set_from_euler_xyz(angles);
}
template <typename T>
MATH_FORCE_INLINE quat<T>::quat(ZEulerXZY, vec3<T> const& angles)
{
	set_from_euler_xzy(angles);
}
template <typename T>
MATH_FORCE_INLINE quat<T>::quat(ZEulerYXZ, vec3<T> const& angles)
{
	set_from_euler_yxz(angles);
}
template <typename T>
MATH_FORCE_INLINE quat<T>::quat(ZEulerYZX, vec3<T> const& angles)
{
	set_from_euler_yzx(angles);
}
template <typename T>
MATH_FORCE_INLINE quat<T>::quat(ZEulerZXY, vec3<T> const& angles)
{
	set_from_euler_zxy(angles);
}
template <typename T>
MATH_FORCE_INLINE quat<T>::quat(ZEulerZYX, vec3<T> const& angles)
{
	set_from_euler_zyx(angles);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
// member functions
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

// sets new quat
template <typename T>
MATH_FORCE_INLINE quat<T>& quat<T>::set(T _x, T _y, T _z, T _w)
{
	x = _x;
	y = _y;
	z = _z;
	w = _w;
	return *this;
}

template <typename T>
MATH_FORCE_INLINE quat<T>& quat<T>::normalize()
{
	T n = x*x + y*y + z*z + w*w;
	assert(n != 0);
	n = 1/(T)sqrt(n);
	return (*this *= n);
}

template <typename T>
MATH_FORCE_INLINE quat<T>& quat<T>::invert()
{
	x = -x;
	y = -y;
	z = -z;
	return *this;
}

template <typename T>
MATH_FORCE_INLINE quat<T>& quat<T>::set_identity()
{
	x = (T)0;
	y = (T)0;
	z = (T)0;
	w = (T)1;
	return *this;
}

template<typename T>
inline quat<T>& quat<T>::set_from_mat3(mat3<T> const& mat)
{
	T trace = mat(0,0) + mat(1,1) + mat(2,2);
	if (trace > 0)
	{
		// |w| > 1/2, may as well choose w > 1/2
		T fRoot = sqrt(trace + 1);  // 2w
		w = T(0.5)*fRoot;
		fRoot = T(0.5)/fRoot;  // 1/(4w)
		x = (mat(1,2)-mat(2,1))*fRoot;
		y = (mat(2,0)-mat(0,2))*fRoot;
		z = (mat(0,1)-mat(1,0))*fRoot;
	}
	else
	{
		// |w| <= 1/2
		int i = 0;
		if (mat(1,1) > mat(0,0))
		{
			i = 1;
		}
		if (mat(2,2) > mat(i,i))
		{
			i = 2;
		}
		int j = (i+1) % 3;
		int k = (j+1) % 3;

		T fRoot = sqrt(mat(i,i) - mat(j,j) - mat(k,k)+1);
		T* apfQuat[3] = { &x, &y, &z };
		*apfQuat[i] = T(0.5)*fRoot;
		fRoot = T(0.5)/fRoot;
		w = (mat(j,k)-mat(k,j))*fRoot;
		*apfQuat[j] = (mat(i,j)+mat(j,i))*fRoot;
		*apfQuat[k] = (mat(i,k)+mat(k,i))*fRoot;
	}

	return *this;
}

template <typename T> 
inline mat3<T> quat<T>::get_as_mat3() const
{
	mat3<T> ret(mat3<T>::uninitialized);
	T *m = ret.data();

	T const tx = x*T(2);
	T const ty = y*T(2);
	T const tz = z*T(2);
	T const txx = tx*x;
	T const tyy = ty*y;
	T const tzz = tz*z;
	T const txy = tx*y;
	T const txz = tx*z;
	T const txw = tx*w;
	T const tyz = ty*z;
	T const tyw = ty*w;
	T const tzw = tz*w;

	m[0] = 1 -	tyy - tzz;
	m[3] =		txy - tzw;
	m[6] =		txz + tyw;

	m[1] =		txy + tzw;
	m[4] = 1 -	txx - tzz;
	m[7] =		tyz - txw;

	m[2] =		txz - tyw;
	m[5] =		tyz + txw;
	m[8] = 1 -	tyy - txx;

	return ret;
}

template <typename T>
MATH_FORCE_INLINE quat<T>& quat<T>::set_from_mat4(mat4<T> const& m)
{
	return set_from_mat3(mat3<T>(m));
}

template <typename T>
MATH_FORCE_INLINE mat4<T> quat<T>::get_as_mat4() const
{
	return mat4<T>(get_as_mat3());
}

template <typename T>
MATH_FORCE_INLINE quat<T>& quat<T>::set_from_trans3d(trans3d<T> const& m)
{
	return set_from_mat3(mat3<T>(m.mat));
}

template <typename T>
MATH_FORCE_INLINE trans3d<T> quat<T>::get_as_trans3d() const
{
	trans3d<T> t(trans3d<T>::uninitialized);
	t.mat = get_as_mat3();
	t.repair();
	return t;
}

template <typename T>
MATH_FORCE_INLINE quat<T>& quat<T>::set_from_euler_xyz(vec3<T> const& angles)
{
	vec3<T> r(angles*T(0.5));
	vec3<T> sr, cr;
	sin_cos(r, sr, cr);
	x = cr.z*sr.x*cr.y + sr.z*cr.x*sr.y;
	y = cr.z*cr.x*sr.y - sr.z*sr.x*cr.y;
	z = sr.z*cr.x*cr.y + cr.z*sr.x*sr.y;
	w = cr.z*cr.x*cr.y - sr.z*sr.x*sr.y;
	return *this;
}
template <typename T>
MATH_FORCE_INLINE quat<T>& quat<T>::set_from_euler_xzy(vec3<T> const& angles)
{
	vec3<T> r(angles*T(0.5));
	vec3<T> sr, cr;
	sin_cos(r, sr, cr);
	x = cr.z*sr.x*cr.y - sr.z*cr.x*sr.y;
	y = cr.z*cr.x*sr.y - sr.z*sr.x*cr.y;
	z = sr.z*cr.x*cr.y + cr.z*sr.x*sr.y;
	w = cr.z*cr.x*cr.y + sr.z*sr.x*sr.y;
	return *this;
}
template <typename T>
MATH_FORCE_INLINE quat<T>& quat<T>::set_from_euler_yxz(vec3<T> const& angles)
{
	vec3<T> r(angles*T(0.5));
	vec3<T> sr, cr;
	sin_cos(r, sr, cr);
	x = cr.z*sr.x*cr.y + sr.z*cr.x*sr.y;
	y = cr.z*cr.x*sr.y - sr.z*sr.x*cr.y;
	z = sr.z*cr.x*cr.y - cr.z*sr.x*sr.y;
	w = cr.z*cr.x*cr.y + sr.z*sr.x*sr.y;
	return *this;
}
template <typename T>
MATH_FORCE_INLINE quat<T>& quat<T>::set_from_euler_yzx(vec3<T> const& angles)
{
	vec3<T> r(angles*T(0.5));
	vec3<T> sr, cr;
	sin_cos(r, sr, cr);
	x = cr.z*sr.x*cr.y + sr.z*cr.x*sr.y;
	y = cr.z*cr.x*sr.y + sr.z*sr.x*cr.y;
	z = sr.z*cr.x*cr.y - cr.z*sr.x*sr.y;
	w = cr.z*cr.x*cr.y - sr.z*sr.x*sr.y;
	return *this;
}
template <typename T>
MATH_FORCE_INLINE quat<T>& quat<T>::set_from_euler_zxy(vec3<T> const& angles)
{
	vec3<T> r(angles*T(0.5));
	vec3<T> sr, cr;
	sin_cos(r, sr, cr);
	x = cr.z*sr.x*cr.y - sr.z*cr.x*sr.y;
	y = cr.z*cr.x*sr.y + sr.z*sr.x*cr.y;
	z = sr.z*cr.x*cr.y + cr.z*sr.x*sr.y;
	w = cr.z*cr.x*cr.y - sr.z*sr.x*sr.y;
	return *this;
}
template <typename T>
MATH_FORCE_INLINE quat<T>& quat<T>::set_from_euler_zyx(vec3<T> const& angles)
{
	vec3<T> r(angles*T(0.5));
	vec3<T> sr, cr;
	sin_cos(r, sr, cr);
	x = cr.z*sr.x*cr.y - sr.z*cr.x*sr.y;
	y = cr.z*cr.x*sr.y + sr.z*sr.x*cr.y;
	z = sr.z*cr.x*cr.y - cr.z*sr.x*sr.y;
	w = cr.z*cr.x*cr.y + sr.z*sr.x*sr.y;
	return *this;
}

template <typename T>
inline vec3<T> quat<T>::get_as_euler_xyz() const
{
	vec3<T> res;
	T const s = T(2)*(z*x + w*y);
	if (s < T(0.9999))
	{
		if (s > -T(0.9999))
		{
			res.y = asin(s);
			res.x = atan2(-T(2)*(z*y - w*x), T(1) - T(2)*(x*x + y*y));
			res.z = atan2(-T(2)*(x*y - w*z), T(1) - T(2)*(z*z + y*y));
		}
		else
		{
			res.y = -(T)kPI2;
			res.x = -atan2(T(2)*(x*y + w*z), T(1) - T(2)*(z*z + x*x));
			res.z = 0;
		}
	}
	else
	{
		res.y = (T)kPI2;
		res.x = atan2(T(2)*(x*y + w*z), T(1) - T(2)*(z*z + x*x));
		res.z = 0;
	} 
	return res;
}
template <typename T>
inline vec3<T> quat<T>::get_as_euler_xzy() const
{
	vec3<T> res;
	T const s = -T(2)*(x*y - w*z);
	if (s < T(0.9999))
	{
		if (s > -T(0.9999))
		{
			res.y = atan2(T(2)*(z*x + w*y), T(1) - T(2)*(z*z + y*y));
			res.x = atan2(T(2)*(z*y + w*x), T(1) - T(2)*(z*z + x*x));
			res.z = asin(s);
		}
		else
		{
			res.y = -atan2(T(2)*(z*x - w*y), T(1) - T(2)*(x*x + y*y));
			res.x = 0;
			res.z = -(T)kPI2;
		}
	}
	else
	{
		res.y = -atan2(T(2)*(z*x - w*y), T(1) - T(2)*(x*x + y*y));
		res.x = 0;
		res.z = (T)kPI2;
	} 
	return res;
}
template <typename T>
inline vec3<T> quat<T>::get_as_euler_yxz() const
{
	vec3<T> res;
	T const s = -T(2)*(z*y - w*x);
	if (s < T(0.9999))
	{
		if (s > -T(0.9999))
		{
			res.y = atan2(T(2)*(z*x + w*y), T(1) - T(2)*(x*x + y*y));
			res.x = asin(s);
			res.z = atan2(T(2)*(x*y + w*z), T(1) - T(2)*(z*z + x*x));
		}
		else
		{
			res.y = 0;
			res.x = -(T)kPI2;
			res.z = atan2(-T(2)*(y*x - w*z), T(1) - T(2)*(y*y + z*z));
		}
	}
	else
	{
		res.y = 0;
		res.x = (T)kPI2;
		res.z = atan2(-T(2)*(y*x - w*z), T(1) - T(2)*(y*y + z*z));
	} 
	return res;
}
template <typename T>
inline vec3<T> quat<T>::get_as_euler_yzx() const
{
	vec3<T> res;
	T const s = T(2)*(x*y + w*z);
	if (s < T(0.9999))
	{
		if (s > -T(0.9999))
		{
			res.y = atan2(-T(2)*(z*x - w*y), T(1) - T(2)*(z*z + y*y));
			res.x = atan2(-T(2)*(z*y - w*x), T(1) - T(2)*(z*z + x*x));
			res.z = asin(s);
		}
		else
		{
			res.y = -atan2(T(2)*(z*y + w*x), T(1) - T(2)*(x*x + y*y));
			res.x = 0;
			res.z = -(T)kPI2;
		}
	}
	else
	{
		res.y = atan2(T(2)*(z*y + w*x), T(1) - T(2)*(x*x + y*y));
		res.x = 0;
		res.z = (T)kPI2;
	} 
	return res;
}

template <typename T>
inline vec3<T> quat<T>::get_as_euler_zxy() const
{
	vec3<T> res;
	T const s = T(2)*(z*y + w*x);
	if (s < T(0.9999))
	{
		if (s > -T(0.9999))
		{
			res.y = atan2(-T(2)*(z*x - w*y), T(1) - T(2)*(x*x + y*y));
			res.x = asin(s);
			res.z = atan2(-T(2)*(x*y - w*z), T(1) - T(2)*(z*z + x*x));
		}
		else
		{
			res.y = 0;
			res.x = -(T)kPI2;
			res.z = -atan2(T(2)*(z*x + w*y), T(1) - T(2)*(z*z + y*y));
		}
	}
	else
	{
		res.y = 0;
		res.x = (T)kPI2;
		res.z = atan2(T(2)*(z*x + w*y), T(1) - T(2)*(z*z + y*y));
	} 
	return res;
}

template <typename T>
inline vec3<T> quat<T>::get_as_euler_zyx() const
{
	vec3<T> res;
	T const s = -T(2)*(z*x - w*y);
	if (s < T(0.9999))
	{
		if (s > -T(0.9999))
		{
			res.y = asin(s);
			res.x = atan2(T(2)*(z*y + w*x), T(1) - T(2)*(x*x + y*y));
			res.z = atan2(T(2)*(x*y + w*z), T(1) - T(2)*(z*z + y*y));
		}
		else
		{
			res.y = -(T)kPI2;
			res.x = 0;
			res.z = atan2(-T(2)*(z*y - w*x), T(1) - T(2)*(z*z + x*x));
		}
	}
	else
	{
		res.y = (T)kPI2;
		res.x = 0;
		res.z = -atan2(-T(2)*(z*y - w*x), T(1) - T(2)*(z*z + x*x));
	} 
	return res;
}

//! axis must be unit length
//! angle in radians
template <typename T>
inline quat<T>& quat<T>::set_from_angle_axis(T angle, vec3<T> const& axis)
{
	T const fHalfAngle = T(0.5)*angle;
	T fSin;
	sin_cos(fHalfAngle, fSin, w);
	x = fSin*axis.x;
	y = fSin*axis.y;
	z = fSin*axis.z;
	return *this;
}

template <typename T>
inline void quat<T>::get_as_angle_axis(T& angle, vec3<T> &axis) const
{
	T const safeW = clamp(w, (T)-1, (T)1);
	T const scale = sqrt((T)1 - safeW * safeW);
	angle = 2 * acos(safeW);
	if (scale < std::numeric_limits<T>::epsilon())
	{
		axis.x = x;
		axis.y = y;
		axis.z = z;
	}
	else
	{
		T const invscale = 1 / scale;
		axis.x = x * invscale;
		axis.y = y * invscale;
		axis.z = z * invscale;
	}
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////
// operators
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename T>
MATH_FORCE_INLINE bool quat<T>::operator==(quat<T> const& v) const
{
	return x == v.x && y == v.y && z == v.z && w == v.w;
}

template<typename T>
MATH_FORCE_INLINE bool quat<T>::operator!=(quat<T> const& v) const
{
	return !operator==(v);
}

template <typename T>
MATH_FORCE_INLINE quat<T>& quat<T>::operator=(quat<T> const& other)
{
	x = other.x;
	y = other.y;
	z = other.z;
	w = other.w;
	return *this;
}

template <typename T>
MATH_FORCE_INLINE quat<T> quat<T>::operator+(quat<T> const& b) const
{
	return quat<T>(x + b.x, y + b.y, z + b.z, w + b.w);
}

template <typename T>
MATH_FORCE_INLINE quat<T> quat<T>::operator-(quat<T> const& b) const
{
	return quat<T>(x - b.x, y - b.y, z - b.z, w - b.w);
}


template <typename T>
MATH_FORCE_INLINE quat<T> quat<T>::operator*(quat<T> const& other) const
{
	quat<T> tmp(uninitialized);

	tmp.x = (other.w * x) + (other.x * w) + (other.y * z) - (other.z * y);
	tmp.y = (other.w * y) + (other.y * w) + (other.z * x) - (other.x * z);
	tmp.z = (other.w * z) + (other.z * w) + (other.x * y) - (other.y * x);
	tmp.w = (other.w * w) - (other.x * x) - (other.y * y) - (other.z * z);

	return tmp;
}

template <typename T>
template <typename U>
MATH_FORCE_INLINE quat<T> quat<T>::operator*(U s) const
{
	return quat<T>((T)s*x, (T)s*y, (T)s*z, (T)s*w);
}

template <typename T>
template <typename U>
MATH_FORCE_INLINE quat<T>& quat<T>::operator*=(U s)
{
	x*=s;
	y*=s;
	z*=s;
	w*=s;
	return *this;
}

template <typename T>
MATH_FORCE_INLINE quat<T>& quat<T>::operator*=(quat<T> const& other)
{
	return (*this = (*this) * other);
}

template <typename T>
MATH_FORCE_INLINE quat<T> quat<T>::operator-() const
{
	return quat<T>(-x, -y, -z, -w);
}

template <typename T>
MATH_FORCE_INLINE quat<T> quat<T>::operator~() const
{
	return quat<T>(-x, -y, -z, +w);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
// global methods
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
template <typename T>
MATH_FORCE_INLINE T dot(quat<T> const& q1, quat<T> const& q2)
{
	return (q1.x * q2.x) + (q1.y * q2.y) + (q1.z * q2.z) + (q1.w * q2.w);
}




}