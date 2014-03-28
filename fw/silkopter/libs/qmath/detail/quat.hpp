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
inline quat<T>::quat() : x(0), y(0), z(0), w(1) {}

template <typename T> 
inline quat<T>::quat(ZUninitialized) {}

template <typename T>
inline quat<T>::quat(T _x, T _y, T _z, T _w) : x(_x), y(_y), z(_z), w(_w) {}

template <typename T>
inline quat<T>::quat(quat<T> const& q) : x(q.x), y(q.y), z(q.z), w(q.w) {}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
// member functions
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

// sets new quat
template <typename T>
inline void quat<T>::set(T _x, T _y, T _z, T _w)
{
	x = _x;
	y = _y;
	z = _z;
	w = _w;
}

template <typename T>
template <class Policy>
inline void quat<T>::normalize()
{
	T n = x*x + y*y + z*z + w*w;
	ASSERT(n != 0);
	n = inv_sqrt<Policy>(n);
	x *= n;
	y *= n;
	z *= n;
	w *= n;
}

template <typename T>
template <class Policy>

inline void quat<T>::invert()
{
	x = -x;
	y = -y;
	z = -z;
}

template <typename T>
inline void quat<T>::set_identity()
{
	x = (T)0;
	y = (T)0;
	z = (T)0;
	w = (T)1;
}

template <typename T>
template <class Policy>
inline void quat<T>::set_from_mat3(mat3<T> const& mat)
{
	T trace = mat(0,0) + mat(1,1) + mat(2,2);
	if (trace > 0)
	{
		// |w| > 1/2, may as well choose w > 1/2
		T fRoot = sqrt<Policy>(trace + 1);  // 2w
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

		T fRoot = sqrt<Policy>(mat(i,i) - mat(j,j) - mat(k,k)+1);
		T* apfQuat[3] = { &x, &y, &z };
		*apfQuat[i] = T(0.5)*fRoot;
		fRoot = T(0.5)/fRoot;
		w = (mat(j,k)-mat(k,j))*fRoot;
		*apfQuat[j] = (mat(i,j)+mat(j,i))*fRoot;
		*apfQuat[k] = (mat(i,k)+mat(k,i))*fRoot;
	}
}

template <typename T>
template <class Policy>
inline void quat<T>::get_as_mat3(mat3<T>& ret) const
{
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
}

template <typename T>
template <class Policy>
inline void quat<T>::set_from_mat4(mat4<T> const& m)
{
	set_from_mat3<Policy>(mat3<T>(m));
}

template <typename T>
template <class Policy>
inline void quat<T>::get_as_mat4(mat4<T>& ret) const
{
	mat3<T> ret3(mat3<T>::uninitialized);
	get_as_mat3<Policy>(ret3);
	ret = ret3;
}

template <typename T>
template <class Policy>
inline void quat<T>::set_from_trans3d(trans3d<T> const& m)
{
	set_from_mat3<Policy>(mat3<T>(m.mat));
}

template <typename T>
template <class Policy>
inline void quat<T>::get_as_trans3d(trans3d<T>& trans) const
{
	get_as_mat4<Policy>(trans.mat);
	trans.repair();
}

template <typename T>
template <class Policy>
inline void quat<T>::set_from_euler_xyz(vec3<T> const& angles)
{
	vec3<T> r(angles*T(0.5));
	vec3<T> sr(vec3<T>::uninitialized), cr(vec3<T>::uninitialized);
	sin_cos<T, Policy>(r, sr, cr);
	x = cr.z*sr.x*cr.y + sr.z*cr.x*sr.y;
	y = cr.z*cr.x*sr.y - sr.z*sr.x*cr.y;
	z = sr.z*cr.x*cr.y + cr.z*sr.x*sr.y;
	w = cr.z*cr.x*cr.y - sr.z*sr.x*sr.y;
}
template <typename T>
template <class Policy>
inline void quat<T>::set_from_euler_xzy(vec3<T> const& angles)
{
	vec3<T> r(angles*T(0.5));
	vec3<T> sr(vec3<T>::uninitialized), cr(vec3<T>::uninitialized);
	sin_cos<Policy>(r, sr, cr);
	x = cr.z*sr.x*cr.y - sr.z*cr.x*sr.y;
	y = cr.z*cr.x*sr.y - sr.z*sr.x*cr.y;
	z = sr.z*cr.x*cr.y + cr.z*sr.x*sr.y;
	w = cr.z*cr.x*cr.y + sr.z*sr.x*sr.y;
}
template <typename T>
template <class Policy>
inline void quat<T>::set_from_euler_yxz(vec3<T> const& angles)
{
	vec3<T> r(angles*T(0.5));
	vec3<T> sr(vec3<T>::uninitialized), cr(vec3<T>::uninitialized);
	sin_cos<Policy>(r, sr, cr);
	x = cr.z*sr.x*cr.y + sr.z*cr.x*sr.y;
	y = cr.z*cr.x*sr.y - sr.z*sr.x*cr.y;
	z = sr.z*cr.x*cr.y - cr.z*sr.x*sr.y;
	w = cr.z*cr.x*cr.y + sr.z*sr.x*sr.y;
}
template <typename T>
template <class Policy>
inline void quat<T>::set_from_euler_yzx(vec3<T> const& angles)
{
	vec3<T> r(angles*T(0.5));
	vec3<T> sr(vec3<T>::uninitialized), cr(vec3<T>::uninitialized);
	sin_cos<Policy>(r, sr, cr);
	x = cr.z*sr.x*cr.y + sr.z*cr.x*sr.y;
	y = cr.z*cr.x*sr.y + sr.z*sr.x*cr.y;
	z = sr.z*cr.x*cr.y - cr.z*sr.x*sr.y;
	w = cr.z*cr.x*cr.y - sr.z*sr.x*sr.y;
}
template <typename T>
template <class Policy>
inline void quat<T>::set_from_euler_zxy(vec3<T> const& angles)
{
	vec3<T> r(angles*T(0.5));
	vec3<T> sr(vec3<T>::uninitialized), cr(vec3<T>::uninitialized);
	sin_cos<Policy>(r, sr, cr);
	x = cr.z*sr.x*cr.y - sr.z*cr.x*sr.y;
	y = cr.z*cr.x*sr.y + sr.z*sr.x*cr.y;
	z = sr.z*cr.x*cr.y + cr.z*sr.x*sr.y;
	w = cr.z*cr.x*cr.y - sr.z*sr.x*sr.y;
}
template <typename T>
template <class Policy>
inline void quat<T>::set_from_euler_zyx(vec3<T> const& angles)
{
	vec3<T> r(angles*T(0.5));
	vec3<T> sr(vec3<T>::uninitialized), cr(vec3<T>::uninitialized);
	sin_cos<Policy>(r, sr, cr);
	x = cr.z*sr.x*cr.y - sr.z*cr.x*sr.y;
	y = cr.z*cr.x*sr.y + sr.z*sr.x*cr.y;
	z = sr.z*cr.x*cr.y - cr.z*sr.x*sr.y;
	w = cr.z*cr.x*cr.y + sr.z*sr.x*sr.y;
}

template <typename T>
template <class Policy>
inline void quat<T>::get_as_euler_xyz(vec3<T>& res) const
{
	T const s = T(2)*(z*x + w*y);
	if (s < T(0.9999))
	{
		if (s > -T(0.9999))
		{
			res.y = asin<Policy>(s);
			res.x = atan2<Policy>(-T(2)*(z*y - w*x), T(1) - T(2)*(x*x + y*y));
			res.z = atan2<Policy>(-T(2)*(x*y - w*z), T(1) - T(2)*(z*z + y*y));
		}
		else
		{
			res.y = -(T)angle<T>::pi2;
			res.x = -atan2<Policy>(T(2)*(x*y + w*z), T(1) - T(2)*(z*z + x*x));
			res.z = 0;
		}
	}
	else
	{
		res.y = (T)angle<T>::pi2;
		res.x = atan2<Policy>(T(2)*(x*y + w*z), T(1) - T(2)*(z*z + x*x));
		res.z = 0;
	} 
}
template <typename T>
template <class Policy>
inline void quat<T>::get_as_euler_xzy(vec3<T>& res) const
{
	T const s = -T(2)*(x*y - w*z);
	if (s < T(0.9999))
	{
		if (s > -T(0.9999))
		{
			res.y = atan2<Policy>(T(2)*(z*x + w*y), T(1) - T(2)*(z*z + y*y));
			res.x = atan2<Policy>(T(2)*(z*y + w*x), T(1) - T(2)*(z*z + x*x));
			res.z = asin<Policy>(s);
		}
		else
		{
			res.y = -atan2<Policy>(T(2)*(z*x - w*y), T(1) - T(2)*(x*x + y*y));
			res.x = 0;
			res.z = -(T)angle<T>::pi2;
		}
	}
	else
	{
		res.y = -atan2<Policy>(T(2)*(z*x - w*y), T(1) - T(2)*(x*x + y*y));
		res.x = 0;
		res.z = (T)angle<T>::pi2;
	} 
}
template <typename T>
template <class Policy>
inline void quat<T>::get_as_euler_yxz(vec3<T>& res) const
{
	T const s = -T(2)*(z*y - w*x);
	if (s < T(0.9999))
	{
		if (s > -T(0.9999))
		{
			res.y = atan2<Policy>(T(2)*(z*x + w*y), T(1) - T(2)*(x*x + y*y));
			res.x = asin<Policy>(s);
			res.z = atan2<Policy>(T(2)*(x*y + w*z), T(1) - T(2)*(z*z + x*x));
		}
		else
		{
			res.y = 0;
			res.x = -(T)angle<T>::pi2;
			res.z = atan2<Policy>(-T(2)*(y*x - w*z), T(1) - T(2)*(y*y + z*z));
		}
	}
	else
	{
		res.y = 0;
		res.x = (T)angle<T>::pi2;
		res.z = atan2<Policy>(-T(2)*(y*x - w*z), T(1) - T(2)*(y*y + z*z));
	} 
}
template <typename T>
template <class Policy>
inline void quat<T>::get_as_euler_yzx(vec3<T>& res) const
{
	T const s = T(2)*(x*y + w*z);
	if (s < T(0.9999))
	{
		if (s > -T(0.9999))
		{
			res.y = atan2<Policy>(-T(2)*(z*x - w*y), T(1) - T(2)*(z*z + y*y));
			res.x = atan2<Policy>(-T(2)*(z*y - w*x), T(1) - T(2)*(z*z + x*x));
			res.z = asin<Policy>(s);
		}
		else
		{
			res.y = -atan2<Policy>(T(2)*(z*y + w*x), T(1) - T(2)*(x*x + y*y));
			res.x = 0;
			res.z = -(T)angle<T>::pi2;
		}
	}
	else
	{
		res.y = atan2<Policy>(T(2)*(z*y + w*x), T(1) - T(2)*(x*x + y*y));
		res.x = 0;
		res.z = (T)angle<T>::pi2;
	} 
}

template <typename T>
template <class Policy>
inline void quat<T>::get_as_euler_zxy(vec3<T>& res) const
{
	T const s = T(2)*(z*y + w*x);
	if (s < T(0.9999))
	{
		if (s > -T(0.9999))
		{
			res.y = atan2<Policy>(-T(2)*(z*x - w*y), T(1) - T(2)*(x*x + y*y));
			res.x = asin<Policy>(s);
			res.z = atan2<Policy>(-T(2)*(x*y - w*z), T(1) - T(2)*(z*z + x*x));
		}
		else
		{
			res.y = 0;
			res.x = -(T)angle<T>::pi2;
			res.z = -atan2<Policy>(T(2)*(z*x + w*y), T(1) - T(2)*(z*z + y*y));
		}
	}
	else
	{
		res.y = 0;
		res.x = (T)angle<T>::pi2;
		res.z = atan2<Policy>(T(2)*(z*x + w*y), T(1) - T(2)*(z*z + y*y));
	} 
}

template <typename T>
template <class Policy>
inline void quat<T>::get_as_euler_zyx(vec3<T>& res) const
{
	T const s = -T(2)*(z*x - w*y);
	if (s < T(0.9999))
	{
		if (s > -T(0.9999))
		{
			res.y = asin<Policy>(s);
			res.x = atan2<Policy>(T(2)*(z*y + w*x), T(1) - T(2)*(x*x + y*y));
			res.z = atan2<Policy>(T(2)*(x*y + w*z), T(1) - T(2)*(z*z + y*y));
		}
		else
		{
			res.y = -(T)angle<T>::pi2;
			res.x = 0;
			res.z = atan2<Policy>(-T(2)*(z*y - w*x), T(1) - T(2)*(z*z + x*x));
		}
	}
	else
	{
		res.y = (T)angle<T>::pi2;
		res.x = 0;
		res.z = -atan2<Policy>(-T(2)*(z*y - w*x), T(1) - T(2)*(z*z + x*x));
	} 
}

//! axis must be unit length
//! angle in radians
template <typename T>
template <class Policy>
inline void quat<T>::set_from_angle_axis(T angle, vec3<T> const& axis)
{
	T const fHalfAngle = T(0.5)*angle;
	T fSin;
	sin_cos<Policy>(fHalfAngle, fSin, w);
	x = fSin*axis.x;
	y = fSin*axis.y;
	z = fSin*axis.z;
}

template <typename T>
template <class Policy>
inline void quat<T>::get_as_angle_axis(T& angle, vec3<T> &axis) const
{
	T const safeW = clamp(w, (T)-1, (T)1);
	T const scale = sqrt<Policy>((T)1 - safeW * safeW);
	angle = 2 * acos<Policy>(safeW);
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
inline bool quat<T>::operator==(quat<T> const& v) const
{
	return x == v.x && y == v.y && z == v.z && w == v.w;
}

template<typename T>
inline bool quat<T>::operator!=(quat<T> const& v) const
{
	return !operator==(v);
}

template <typename T>
inline quat<T>& quat<T>::operator=(quat<T> const& other)
{
	x = other.x;
	y = other.y;
	z = other.z;
	w = other.w;
	return *this;
}

template <typename T>
inline quat<T> quat<T>::operator+(quat<T> const& b) const
{
	return quat<T>(x + b.x, y + b.y, z + b.z, w + b.w);
}

template <typename T>
inline quat<T> quat<T>::operator-(quat<T> const& b) const
{
	return quat<T>(x - b.x, y - b.y, z - b.z, w - b.w);
}


template <typename T>
inline quat<T> quat<T>::operator*(quat<T> const& other) const
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
inline quat<T> quat<T>::operator*(U s) const
{
	return quat<T>((T)s*x, (T)s*y, (T)s*z, (T)s*w);
}

template <typename T>
template <typename U>
inline quat<T>& quat<T>::operator*=(U s)
{
	x*=s;
	y*=s;
	z*=s;
	w*=s;
	return *this;
}

template <typename T>
inline quat<T>& quat<T>::operator*=(quat<T> const& other)
{
	return (*this = (*this) * other);
}

template <typename T>
inline quat<T> quat<T>::operator-() const
{
	return quat<T>(-x, -y, -z, -w);
}

template <typename T>
inline quat<T> quat<T>::operator~() const
{
	return quat<T>(-x, -y, -z, +w);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
// global methods
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
template <typename T>
inline T dot(quat<T> const& q1, quat<T> const& q2)
{
	return (q1.x * q2.x) + (q1.y * q2.y) + (q1.z * q2.z) + (q1.w * q2.w);
}




}